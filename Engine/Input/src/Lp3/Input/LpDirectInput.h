#ifdef LPDIRECTINPUT_H
#ifdef LP3_COMPILE_TARGET_WINDOWS
#define LPDIRECTINPUT_H


#include <Lp3/LE.h>
#include <Lp3/Engine/Input/PlatformControlDevice.h>
#include <LpArray.h>
#define DIRECTINPUT_VESRION 0x0800
#include <dinput.h>

// A class to hold the DIDEVICEOBJECT, along with it's buttons.
class DiControlDevice : public Lp3::Engine::Input::PlatformControlDevice
{
public:
    DiControlDevice(DIDEVICEINSTANCE devInstance);

    bool DeviceReady();

    // Call after update.  Calls first key code hit.
    int FindPressedKeyCode();

    // States if control device has analog joystick capabilities at the designated keycode.
    bool IsAnalogKey(int keyCode);

    char * GetName();

    //void GetKeyCallBack(LPCDIDEVICEOBJECTINSTANCE objectInstance);
    int GetKeyCount();

    const char * GetKeyName(int keyCode);

    int InitDevice(LPDIRECTINPUT8 * di8, HWND hWnd);

    // !Call! after Update.
    // Retrieves analog version if available if useAnalog = true.
    Lp3::Float32 GetKeyState(int keyCode, bool useAnalog);

    int ReleaseDevice();

    //BOOL SetAxisCallBack(LPCDIDEVICEOBJECTINSTANCE objectInstance, LPVOID pvRef);

    void SwitchWindow(HWND newHwnd);

    void Update();

private:
    DIDEVICEINSTANCE deviceInstance;
    LPDIRECTINPUTDEVICE8 device;
    DIJOYSTATE2 js;
    char kbBuffer[256];
    Lp3::Float32 axisXAllowance;
    Lp3::Float32 axisYAllowance;
    bool supportsForceFeedBack;
    LPDIRECTINPUTEFFECT effect;
    //LpArray<DIDEVICEOBJECTINSTANCE> Keys;
    int userCount;
};


class BSDevice
{
public:
    BSDevice(DIDEVICEINSTANCE devInstance);
private:
    DIDEVICEINSTANCE deviceInstance;
    LPDIRECTINPUTDEVICE8 device;
    DIJOYSTATE2 js;
    char kbBuffer[256];
    //LpArray<DIDEVICEOBJECTINSTANCE> Keys;
    int userCount;
};
#endif
#endif
