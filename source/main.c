#include <whb/log.h>
#include <whb/log_udp.h>
#include <whb/log_cafe.h>

#include <vpad/input.h>

#include <wups.h>
#include <wups/config/WUPSConfigItemIntegerRange.h>
#include <wups/config/WUPSConfigItemBoolean.h>

WUPS_PLUGIN_NAME("Gamepad Volume Changer");
WUPS_PLUGIN_DESCRIPTION("This plugin allows changes to the Gamepad's volume!");
WUPS_PLUGIN_VERSION("v2.0");
WUPS_PLUGIN_AUTHOR("Fangal");
WUPS_PLUGIN_LICENSE("GPLv3");

#define VOLUME_SET_CONFIG_ID "VolumeSet"
#define VOLUME_ENABLE_CONFIG_ID "VolumeEnable"

WUPS_USE_STORAGE("Gamepad_Volume_Changer");

bool enable = true;
int volume = 15;

bool headphone = false;
bool prevHeadphone = false;

extern uint32_t VPADSetAudioVolumeOverride(VPADChan chan, bool override, int vol); 
extern int32_t OSGetPFID();

void VolumeOverride(bool override, int vol) {
    uint32_t res;
    if ((res = VPADSetAudioVolumeOverride(VPAD_CHAN_0, override, (volume * 17))) != 0) {
        WHBLogPrintf("Error overriding volume. (%d)", res);
    }
}

void enableChanged(ConfigItemBoolean *item, bool newValue) {
    enable = newValue;
    WUPSStorageAPI_StoreBool(NULL, VOLUME_ENABLE_CONFIG_ID, enable);

    if (!headphone) {
        VolumeOverride(enable, volume);
    }
}

void volumeChanged(ConfigItemIntegerRange *item, int newValue) {
    volume = newValue;
    WUPSStorageAPI_StoreInt(NULL, VOLUME_SET_CONFIG_ID, volume);

    if (!headphone) {
        VolumeOverride(enable, volume);
    }
}

WUPSConfigAPICallbackStatus ConfigMenuOpenedCallback(WUPSConfigCategoryHandle root) {
    if (WUPSConfigItemBoolean_AddToCategory(root, VOLUME_ENABLE_CONFIG_ID, "Enable", true, enable, &enableChanged) != WUPSCONFIG_API_RESULT_SUCCESS) {
        WHBLogPrintf("Failed to add item");
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }

    if (WUPSConfigItemIntegerRange_AddToCategory(root, VOLUME_SET_CONFIG_ID, "Volume:", 15, volume, 0, 15, &volumeChanged) != WUPSCONFIG_API_RESULT_SUCCESS) {
        WHBLogPrintf("Failed to add item");
        return WUPSCONFIG_API_CALLBACK_RESULT_ERROR;
    }

    return WUPSCONFIG_API_CALLBACK_RESULT_SUCCESS;
}

void ConfigMenuClosedCallback() {
    WUPSStorageAPI_SaveStorage(false);
}

INITIALIZE_PLUGIN() {
    WHBLogUdpInit();
    WHBLogCafeInit();

    WHBLogPrintf("Shalom from Gamepad Volume Changer!");

    WUPSConfigAPIOptionsV1 configOptions = {.name = "Gamepad Volume Changer"};
    if (WUPSConfigAPI_Init(configOptions, ConfigMenuOpenedCallback, ConfigMenuClosedCallback) != WUPSCONFIG_API_RESULT_SUCCESS) {
        WHBLogPrintf("Failed to init config api");
    }

    WUPSStorageError storageRes;
    if ((storageRes = WUPSStorageAPI_GetBool(NULL, VOLUME_ENABLE_CONFIG_ID, &enable)) == WUPS_STORAGE_ERROR_NOT_FOUND) {
        if (WUPSStorageAPI_StoreBool(NULL, VOLUME_ENABLE_CONFIG_ID, enable) != WUPS_STORAGE_ERROR_SUCCESS) {
            WHBLogPrintf("Failed to store bool");
        }
    }
    else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS) {
        WHBLogPrintf("Failed to get bool %s (%d)", WUPSConfigAPI_GetStatusStr(storageRes), storageRes);
    }
    else {
        WHBLogPrintf("Successfully read the value from storage: %d %s (%d)", enable, WUPSConfigAPI_GetStatusStr(storageRes), storageRes);
    }

    if ((storageRes = WUPSStorageAPI_GetInt(NULL, VOLUME_SET_CONFIG_ID, &volume)) == WUPS_STORAGE_ERROR_NOT_FOUND) {
        if (WUPSStorageAPI_StoreBool(NULL, VOLUME_SET_CONFIG_ID, volume) != WUPS_STORAGE_ERROR_SUCCESS) {
            WHBLogPrintf("Failed to store bool");
        }
    }
    else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS) {
        WHBLogPrintf("Failed to get bool %s (%d)", WUPSConfigAPI_GetStatusStr(storageRes), storageRes);
    }
    else {
        WHBLogPrintf("Successfully read the value from storage: %d %s (%d)", volume, WUPSConfigAPI_GetStatusStr(storageRes), storageRes);
    }

    WUPSStorageAPI_SaveStorage(false);
}

ON_APPLICATION_START() {
    VPADStatus buf;
    VPADRead(VPAD_CHAN_0, &buf, 1, NULL);
    prevHeadphone = buf.usingHeadphones;

    if (!headphone) {
        VolumeOverride(enable, volume);    
    }
}

ON_ACQUIRED_FOREGROUND() {
    if (!headphone) {
        VolumeOverride(enable, volume);
    }
    else {
        VolumeOverride(false, 0);
    }
}

DECL_FUNCTION(int32_t, VPADRead, VPADChan chan, VPADStatus *buf, uint32_t count, VPADReadError *err) {
    int32_t res = real_VPADRead(chan, buf, count, err);

    headphone = buf->usingHeadphones;

    if (headphone != prevHeadphone) {
        if (headphone) {
            VolumeOverride(false, 0);
        }
        else {
            VolumeOverride(enable, volume);
        }

        prevHeadphone = headphone;
    }
    
    return res;
}

DECL_FUNCTION(void, FSInit) {
    real_FSInit();
    
    int32_t pfid = OSGetPFID();

    if (((pfid != 2) && (pfid != 15)) && !headphone) {
        VolumeOverride(enable, volume);
    }
}

// Slight hack to get this to work on the home button menu
DECL_FUNCTION(BOOL, OSIsMainCore) {
    if (!headphone) {
        VolumeOverride(enable, volume);
    }
    else {
        VolumeOverride(false, 0);
    }

    return real_OSIsMainCore();
}

WUPS_MUST_REPLACE_FOR_PROCESS(VPADRead, WUPS_LOADER_LIBRARY_VPAD, VPADRead, WUPS_FP_TARGET_PROCESS_ALL);
WUPS_MUST_REPLACE_FOR_PROCESS(FSInit, WUPS_LOADER_LIBRARY_COREINIT, FSInit, WUPS_FP_TARGET_PROCESS_ALL);
WUPS_MUST_REPLACE_FOR_PROCESS(OSIsMainCore, WUPS_LOADER_LIBRARY_COREINIT, OSIsMainCore, WUPS_FP_TARGET_PROCESS_HOME_MENU);