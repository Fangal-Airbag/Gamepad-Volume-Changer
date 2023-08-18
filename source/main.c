#include <vpad/input.h>

#include <wups.h>
#include <wups/config/WUPSConfigItemIntegerRange.h>
#include <wups/config/WUPSConfigItemBoolean.h>

WUPS_PLUGIN_NAME("Gamepad Volume Changer");
WUPS_PLUGIN_DESCRIPTION("This plugin allows changes to the Gamepad's volume!");
WUPS_PLUGIN_VERSION("v1.2");
WUPS_PLUGIN_AUTHOR("Fangal");
WUPS_PLUGIN_LICENSE("GPLv3");

#define VOLUME_SET_CONFIG_ID "VolumeSet"
#define VOLUME_ENABLE_CONFIG_ID "VolumeEnable"

WUPS_USE_STORAGE("Gamepad_Volume_Changer");

bool enable = true;
int volume = 15;

extern uint32_t VPADSetAudioVolumeOverride(VPADChan chan, bool override, int vol); 

INITIALIZE_PLUGIN() {
    // Open storage to read values
    WUPSStorageError storageRes = WUPS_OpenStorage();
    if (storageRes == WUPS_STORAGE_ERROR_SUCCESS) {
        if ((storageRes = WUPS_GetInt(NULL, VOLUME_SET_CONFIG_ID, &volume)) == WUPS_STORAGE_ERROR_NOT_FOUND) {
            // Add the value to the storage if it's missing.
            WUPS_StoreInt(NULL, VOLUME_SET_CONFIG_ID, volume);
        }

        if ((storageRes = WUPS_GetBool(NULL, VOLUME_ENABLE_CONFIG_ID, &enable)) == WUPS_STORAGE_ERROR_NOT_FOUND) {
            // Add the value to the storage if it's missing.
            WUPS_StoreBool(NULL, VOLUME_ENABLE_CONFIG_ID, enable);
        }

        // Close storage
        WUPS_CloseStorage();
    }
}

void volumeChanged(ConfigItemIntegerRange *item, int newValue) {
    volume = newValue;
    WUPS_StoreInt(NULL, VOLUME_SET_CONFIG_ID, volume);

    if (enable) {
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, true, (volume * 17));
    }
}

void enableChanged(ConfigItemBoolean *item, bool newValue) {
    enable = newValue;
    WUPS_StoreBool(NULL, VOLUME_ENABLE_CONFIG_ID, enable);

    if (enable) {
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, true, (volume * 17));
    }
    else {
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, false, 0);
    }   
}

WUPS_GET_CONFIG() {
    if (WUPS_OpenStorage() != WUPS_STORAGE_ERROR_SUCCESS) {
        return 0;
    }

    WUPSConfigHandle config;
    WUPSConfig_CreateHandled(&config, "Gamepad Volume Changer Plugin");

    WUPSConfigCategoryHandle cat;
    WUPSConfig_AddCategoryByNameHandled(config, "Volume", &cat);

    WUPSConfigItemBoolean_AddToCategoryHandled(config, cat, VOLUME_ENABLE_CONFIG_ID, "Enable", enable, &enableChanged);
    WUPSConfigItemIntegerRange_AddToCategoryHandled(config, cat, VOLUME_SET_CONFIG_ID, "Volume:", volume, 0, 15, &volumeChanged);

    return config;
}

WUPS_CONFIG_CLOSED() {
    WUPS_CloseStorage();
}

DECL_FUNCTION(void, FSInit) {
    real_FSInit();

    if (enable) {
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, true, (volume * 17));
    }
    else {
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, false, 0);
    }
}

// Slight hack to get this to work on the home button menu
DECL_FUNCTION(BOOL, OSIsMainCore) {
    if (enable) {
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, true, (volume * 17));
    }
    else {
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, false, 0);
    }

    return real_OSIsMainCore();
}

WUPS_MUST_REPLACE_FOR_PROCESS(FSInit, WUPS_LOADER_LIBRARY_COREINIT, FSInit, WUPS_FP_TARGET_PROCESS_ALL);
WUPS_MUST_REPLACE_FOR_PROCESS(OSIsMainCore, WUPS_LOADER_LIBRARY_COREINIT, OSIsMainCore, WUPS_FP_TARGET_PROCESS_HOME_MENU);