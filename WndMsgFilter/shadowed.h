#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define XINPUT_DEVTYPE_GAMEPAD          0x01

#define XINPUT_DEVSUBTYPE_GAMEPAD           0x01
#define XINPUT_DEVSUBTYPE_UNKNOWN           0x00
#define XINPUT_DEVSUBTYPE_WHEEL             0x02
#define XINPUT_DEVSUBTYPE_ARCADE_STICK      0x03
#define XINPUT_DEVSUBTYPE_FLIGHT_STICK      0x04
#define XINPUT_DEVSUBTYPE_DANCE_PAD         0x05
#define XINPUT_DEVSUBTYPE_GUITAR            0x06
#define XINPUT_DEVSUBTYPE_GUITAR_ALTERNATE  0x07
#define XINPUT_DEVSUBTYPE_DRUM_KIT          0x08
#define XINPUT_DEVSUBTYPE_GUITAR_BASS       0x0B
#define XINPUT_DEVSUBTYPE_ARCADE_PAD        0x13

#define XINPUT_CAPS_VOICE_SUPPORTED     0x0004
#define XINPUT_CAPS_FFB_SUPPORTED       0x0001
#define XINPUT_CAPS_WIRELESS            0x0002
#define XINPUT_CAPS_PMD_SUPPORTED       0x0008
#define XINPUT_CAPS_NO_NAVIGATION       0x0010

#define XINPUT_GAMEPAD_DPAD_UP          0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define XINPUT_GAMEPAD_START            0x0010
#define XINPUT_GAMEPAD_BACK             0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000

#define XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE  7849
#define XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE 8689
#define XINPUT_GAMEPAD_TRIGGER_THRESHOLD    30

#define XINPUT_FLAG_GAMEPAD             0x00000001

#define BATTERY_DEVTYPE_GAMEPAD         0x00
#define BATTERY_DEVTYPE_HEADSET         0x01

#define BATTERY_TYPE_DISCONNECTED       0x00    // This device is not connected
#define BATTERY_TYPE_WIRED              0x01    // Wired device, no battery
#define BATTERY_TYPE_ALKALINE           0x02    // Alkaline battery source
#define BATTERY_TYPE_NIMH               0x03    // Nickel Metal Hydride battery source
#define BATTERY_TYPE_UNKNOWN            0xFF    // Cannot determine the battery type

#define BATTERY_LEVEL_EMPTY             0x00
#define BATTERY_LEVEL_LOW               0x01
#define BATTERY_LEVEL_MEDIUM            0x02
#define BATTERY_LEVEL_FULL              0x03

#define XUSER_MAX_COUNT                 4

#define XUSER_INDEX_ANY                 0x000000FF

#define VK_PAD_A                        0x5800
#define VK_PAD_B                        0x5801
#define VK_PAD_X                        0x5802
#define VK_PAD_Y                        0x5803
#define VK_PAD_RSHOULDER                0x5804
#define VK_PAD_LSHOULDER                0x5805
#define VK_PAD_LTRIGGER                 0x5806
#define VK_PAD_RTRIGGER                 0x5807

#define VK_PAD_DPAD_UP                  0x5810
#define VK_PAD_DPAD_DOWN                0x5811
#define VK_PAD_DPAD_LEFT                0x5812
#define VK_PAD_DPAD_RIGHT               0x5813
#define VK_PAD_START                    0x5814
#define VK_PAD_BACK                     0x5815
#define VK_PAD_LTHUMB_PRESS             0x5816
#define VK_PAD_RTHUMB_PRESS             0x5817

#define VK_PAD_LTHUMB_UP                0x5820
#define VK_PAD_LTHUMB_DOWN              0x5821
#define VK_PAD_LTHUMB_RIGHT             0x5822
#define VK_PAD_LTHUMB_LEFT              0x5823
#define VK_PAD_LTHUMB_UPLEFT            0x5824
#define VK_PAD_LTHUMB_UPRIGHT           0x5825
#define VK_PAD_LTHUMB_DOWNRIGHT         0x5826
#define VK_PAD_LTHUMB_DOWNLEFT          0x5827

#define VK_PAD_RTHUMB_UP                0x5830
#define VK_PAD_RTHUMB_DOWN              0x5831
#define VK_PAD_RTHUMB_RIGHT             0x5832
#define VK_PAD_RTHUMB_LEFT              0x5833
#define VK_PAD_RTHUMB_UPLEFT            0x5834
#define VK_PAD_RTHUMB_UPRIGHT           0x5835
#define VK_PAD_RTHUMB_DOWNRIGHT         0x5836
#define VK_PAD_RTHUMB_DOWNLEFT          0x5837

#define XINPUT_KEYSTROKE_KEYDOWN        0x0001
#define XINPUT_KEYSTROKE_KEYUP          0x0002
#define XINPUT_KEYSTROKE_REPEAT         0x0004

struct XINPUT_GAMEPAD
{
    WORD  wButtons;
    BYTE  bLeftTrigger;
    BYTE  bRightTrigger;
    SHORT sThumbLX;
    SHORT sThumbLY;
    SHORT sThumbRX;
    SHORT sThumbRY;
};

struct XINPUT_STATE
{
    DWORD          dwPacketNumber;
    XINPUT_GAMEPAD Gamepad;
};

struct XINPUT_VIBRATION
{
    WORD wLeftMotorSpeed;
    WORD wRightMotorSpeed;
};

struct XINPUT_CAPABILITIES
{
    BYTE             Type;
    BYTE             SubType;
    WORD             Flags;
    XINPUT_GAMEPAD   Gamepad;
    XINPUT_VIBRATION Vibration;
};

struct XINPUT_BATTERY_INFORMATION
{
    BYTE BatteryType;
    BYTE BatteryLevel;
};

struct XINPUT_KEYSTROKE
{
    WORD  VirtualKey;
    WCHAR Unicode;
    WORD  Flags;
    BYTE  UserIndex;
    BYTE  HidCode;
};

// A list of functions present in XInput
// Sourced from https://learn.microsoft.com/en-us/windows/win32/api/_xinput/

// The strategy is to implement and thus shadowing the symbol for all of them
// When called, detect if `dwUserIndex` (the gamepad ID) matches the one we're trying to override
// if so, return the values generated from our frontend
// if not, simply redirect to the actual XInput API

// Note that XInput has a variety of different versions: https://learn.microsoft.com/en-us/windows/win32/xinput/xinput-versions and the relevant functions differ. 
// We are targetting xinput1_4.dll as you would see on a typical Win10 installation

#define XI_API_FUNC extern "C" __declspec(dllexport)

// Deprecated in Win10
//
//XI_API_FUNC
//void WINAPI XInputEnable(
//    _In_ BOOL enable
//) WIN_NOEXCEPT;
//
//using Pfn_XInputEnable = std::add_pointer_t<decltype(XInputEnable)>;
//extern Pfn_XInputEnable pfn_XInputEnable;

XI_API_FUNC DWORD WINAPI XInputGetAudioDeviceIds(
    _In_ DWORD dwUserIndex,
    _Out_writes_opt_(*pRenderCount) LPWSTR pRenderDeviceId,
    _Inout_opt_ UINT* pRenderCount,
    _Out_writes_opt_(*pCaptureCount) LPWSTR pCaptureDeviceId,
    _Inout_opt_ UINT* pCaptureCount
) WIN_NOEXCEPT;

using Pfn_XInputGetAudioDeviceIds = std::add_pointer_t<decltype(XInputGetAudioDeviceIds)>;
extern Pfn_XInputGetAudioDeviceIds pfn_XInputGetAudioDeviceIds;

// Deprecated since Win8
// NOTE(rtk0c): the definition is gone alltogether from Win10, on my dev machine
//
//XI_API_FUNC DWORD WINAPI XInputGetDSoundAudioDeviceGuids(
//    DWORD dwUserIndex,
//    GUID* pDSoundRenderGuid,
//    GUID* pDSoundCaptureGuid
//) WIN_NOEXCEPT;
//
//using Pfn_XInputGetDSoundAudioDeviceGuids = std::add_pointer_t<decltype(XInputGetDSoundAudioDeviceGuids)>;
//extern Pfn_XInputGetDSoundAudioDeviceGuids pfn_XInputGetDSoundAudioDeviceGuids;

XI_API_FUNC DWORD WINAPI XInputGetBatteryInformation(
    _In_ DWORD dwUserIndex,
    _In_ BYTE devType,
    _Out_ XINPUT_BATTERY_INFORMATION* pBatteryInformation
) WIN_NOEXCEPT;

using Pfn_XInputGetBatteryInformation = std::add_pointer_t<decltype(XInputGetBatteryInformation)>;
extern Pfn_XInputGetBatteryInformation pfn_XInputGetBatteryInformation;

XI_API_FUNC DWORD WINAPI XInputGetCapabilities(
    _In_ DWORD dwUserIndex,
    _In_ DWORD dwFlags,
    _Out_ XINPUT_CAPABILITIES* pCapabilities
) WIN_NOEXCEPT;

using Pfn_XInputGetCapabilities = std::add_pointer_t<decltype(XInputGetCapabilities)>;
extern Pfn_XInputGetCapabilities pfn_XInputGetCapabilities;

XI_API_FUNC DWORD WINAPI XInputGetKeystroke(
    _In_ DWORD dwUserIndex,
    _Reserved_ DWORD dwReserved,
    _Out_ XINPUT_KEYSTROKE* pKeystroke
) WIN_NOEXCEPT;

using Pfn_XInputGetKeystroke = std::add_pointer_t<decltype(XInputGetKeystroke)>;
extern Pfn_XInputGetKeystroke pfn_XInputGetKeystroke;

XI_API_FUNC DWORD WINAPI XInputGetState(
    _In_ DWORD dwUserIndex,
    _Out_ XINPUT_STATE* pState
) WIN_NOEXCEPT;

using Pfn_XInputGetState = std::add_pointer_t<decltype(XInputGetState)>;
extern Pfn_XInputGetState pfn_XInputGetState;

XI_API_FUNC DWORD WINAPI XInputSetState(
    _In_ DWORD dwUserIndex,
    _In_ XINPUT_VIBRATION* pVibration
) WIN_NOEXCEPT;

using Pfn_XInputSetState = std::add_pointer_t<decltype(XInputSetState)>;
extern Pfn_XInputSetState pfn_XInputSetState;
