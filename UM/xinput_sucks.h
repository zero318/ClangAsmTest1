#pragma once
#pragma clang diagnostic ignored "-Wdll-attribute-on-redeclaration"

// Nasty hack header to force clang to properly emit indirect calls
// by spamming __declspec(dllimport) on every function.

#if !__INTELLISENSE__
extern "C" {

__declspec(dllimport)
DWORD WINAPI XInputGetState
(
    __in  DWORD         dwUserIndex,  // Index of the gamer associated with the device
    __out XINPUT_STATE* pState        // Receives the current state
);

__declspec(dllimport)
DWORD WINAPI XInputSetState
(
    __in DWORD             dwUserIndex,  // Index of the gamer associated with the device
    __in XINPUT_VIBRATION* pVibration    // The vibration information to send to the controller
);

__declspec(dllimport)
DWORD WINAPI XInputGetCapabilities
(
    __in  DWORD                dwUserIndex,   // Index of the gamer associated with the device
    __in  DWORD                dwFlags,       // Input flags that identify the device type
    __out XINPUT_CAPABILITIES* pCapabilities  // Receives the capabilities
);

__declspec(dllimport)
void WINAPI XInputEnable
(
    __in BOOL enable     // [in] Indicates whether xinput is enabled or disabled. 
);

__declspec(dllimport)
DWORD WINAPI XInputGetDSoundAudioDeviceGuids
(
    __in  DWORD dwUserIndex,          // Index of the gamer associated with the device
    __out GUID* pDSoundRenderGuid,    // DSound device ID for render
    __out GUID* pDSoundCaptureGuid    // DSound device ID for capture
);

#ifndef XINPUT_USE_9_1_0

__declspec(dllimport)
DWORD WINAPI XInputGetBatteryInformation
(
    __in  DWORD                       dwUserIndex,        // Index of the gamer associated with the device
    __in  BYTE                        devType,            // Which device on this user index
    __out XINPUT_BATTERY_INFORMATION* pBatteryInformation // Contains the level and types of batteries
);

__declspec(dllimport)
DWORD WINAPI XInputGetKeystroke
(
    __in       DWORD dwUserIndex,              // Index of the gamer associated with the device
    __reserved DWORD dwReserved,               // Reserved for future use
    __out      PXINPUT_KEYSTROKE pKeystroke    // Pointer to an XINPUT_KEYSTROKE structure that receives an input event.
);

#endif //!XINPUT_USE_9_1_0
}
#endif