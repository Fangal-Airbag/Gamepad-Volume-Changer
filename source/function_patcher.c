#include "main.h"

#include <vpad/input.h>

#include <wups.h>

extern uint32_t VPADSetAudioVolumeOverride(VPADChan chan, bool override, int vol); 

DECL_FUNCTION(int32_t, VPADRead, VPADChan chan, VPADStatus *buffers, uint32_t count, VPADReadError *err)
{
    int32_t result = real_VPADRead(chan, buffers, count, err);

    if (enable) 
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, true, (volume * 17));
    else
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, false, 0);

    return result;
}

DECL_FUNCTION(int32_t, HBM_VPADRead, VPADChan chan, VPADStatus *buffers, uint32_t count, VPADReadError *err)
{
    int32_t result = real_HBM_VPADRead(chan, buffers, count, err);

    if (enable) 
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, true, (volume * 17));
    else
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, false, 0);

    return result;
}

DECL_FUNCTION(int32_t, TVII_VPADRead, VPADChan chan, VPADStatus *buffers, uint32_t count, VPADReadError *err)
{
    int32_t result = real_TVII_VPADRead(chan, buffers, count, err);

    if (enable) 
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, true, (volume * 17));
    else
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, false, 0);

    return result;
}

DECL_FUNCTION(int32_t, EMANUAL_VPADRead, VPADChan chan, VPADStatus *buffers, uint32_t count, VPADReadError *err)
{
    int32_t result = real_EMANUAL_VPADRead(chan, buffers, count, err);

    if (enable) 
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, true, (volume * 17));
    else
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, false, 0);

    return result;
}

DECL_FUNCTION(int32_t, ERROR_VPADRead, VPADChan chan, VPADStatus *buffers, uint32_t count, VPADReadError *err)
{
    int32_t result = real_ERROR_VPADRead(chan, buffers, count, err);

    if (enable) 
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, true, (volume * 17));
    else
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, false, 0);

    return result;
}

DECL_FUNCTION(int32_t, MINI_MIIVERSE_VPADRead, VPADChan chan, VPADStatus *buffers, uint32_t count, VPADReadError *err)
{
    int32_t result = real_MINI_MIIVERSE_VPADRead(chan, buffers, count, err);

    if (enable) 
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, true, (volume * 17));
    else
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, false, 0);

    return result;
}

DECL_FUNCTION(int32_t, BROWSER_VPADRead, VPADChan chan, VPADStatus *buffers, uint32_t count, VPADReadError *err)
{
    int32_t result = real_BROWSER_VPADRead(chan, buffers, count, err);

    if (enable) 
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, true, (volume * 17));
    else
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, false, 0);

    return result;
}

DECL_FUNCTION(int32_t, MIIVERSE_VPADRead, VPADChan chan, VPADStatus *buffers, uint32_t count, VPADReadError *err)
{
    int32_t result = real_MIIVERSE_VPADRead(chan, buffers, count, err);

    if (enable) 
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, true, (volume * 17));
    else
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, false, 0);

    return result;
}

DECL_FUNCTION(int32_t, ESHOP_VPADRead, VPADChan chan, VPADStatus *buffers, uint32_t count, VPADReadError *err)
{
    int32_t result = real_ESHOP_VPADRead(chan, buffers, count, err);

    if (enable) 
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, true, (volume * 17));
    else
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, false, 0);

    return result;
}

DECL_FUNCTION(int32_t, FRL_VPADRead, VPADChan chan, VPADStatus *buffers, uint32_t count, VPADReadError *err)
{
    int32_t result = real_FRL_VPADRead(chan, buffers, count, err);

    if (enable) 
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, true, (volume * 17));
    else
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, false, 0);

    return result;
}

DECL_FUNCTION(int32_t, DWLD_MNG_VPADRead, VPADChan chan, VPADStatus *buffers, uint32_t count, VPADReadError *err)
{
    int32_t result = real_DWLD_MNG_VPADRead(chan, buffers, count, err);

    if (enable) 
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, true, (volume * 17));
    else
        VPADSetAudioVolumeOverride(VPAD_CHAN_0, false, 0);

    return result;
}

WUPS_MUST_REPLACE(VPADRead, WUPS_LOADER_LIBRARY_VPAD, VPADRead);
WUPS_MUST_REPLACE_FOR_PROCESS(HBM_VPADRead, WUPS_LOADER_LIBRARY_VPAD, VPADRead, WUPS_FP_TARGET_PROCESS_HOME_MENU);
WUPS_MUST_REPLACE_FOR_PROCESS(TVII_VPADRead, WUPS_LOADER_LIBRARY_VPAD, VPADRead, WUPS_FP_TARGET_PROCESS_TVII);
WUPS_MUST_REPLACE_FOR_PROCESS(EMANUAL_VPADRead, WUPS_LOADER_LIBRARY_VPAD, VPADRead, WUPS_FP_TARGET_PROCESS_E_MANUAL);
WUPS_MUST_REPLACE_FOR_PROCESS(ERROR_VPADRead, WUPS_LOADER_LIBRARY_VPAD, VPADRead, WUPS_FP_TARGET_PROCESS_ERROR_DISPLAY);
WUPS_MUST_REPLACE_FOR_PROCESS(MINI_MIIVERSE_VPADRead, WUPS_LOADER_LIBRARY_VPAD, VPADRead, WUPS_FP_TARGET_PROCESS_MINI_MIIVERSE);
WUPS_MUST_REPLACE_FOR_PROCESS(BROWSER_VPADRead, WUPS_LOADER_LIBRARY_VPAD, VPADRead, WUPS_FP_TARGET_PROCESS_BROWSER);
WUPS_MUST_REPLACE_FOR_PROCESS(MIIVERSE_VPADRead, WUPS_LOADER_LIBRARY_VPAD, VPADRead, WUPS_FP_TARGET_PROCESS_MIIVERSE);
WUPS_MUST_REPLACE_FOR_PROCESS(ESHOP_VPADRead, WUPS_LOADER_LIBRARY_VPAD, VPADRead, WUPS_FP_TARGET_PROCESS_ESHOP);
WUPS_MUST_REPLACE_FOR_PROCESS(FRL_VPADRead, WUPS_LOADER_LIBRARY_VPAD, VPADRead, WUPS_FP_TARGET_PROCESS_PFID_11);
WUPS_MUST_REPLACE_FOR_PROCESS(DWLD_MNG_VPADRead, WUPS_LOADER_LIBRARY_VPAD, VPADRead, WUPS_FP_TARGET_PROCESS_DOWNLOAD_MANAGER);