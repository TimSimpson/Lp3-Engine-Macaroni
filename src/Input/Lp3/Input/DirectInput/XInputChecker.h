#ifndef LP3_INPUT_DIRECTINPUT_XINPUTCHECKER_H
#define LP3_INPUT_DIRECTINPUT_XINPUTCHECKER_H

#include <Lp3/LE.h>
#ifdef LP3_COMPILE_TARGET_WINDOWS

#include <windows.h>

namespace Lp3 { namespace Engine { namespace Input { namespace DirectInput {

BOOL IsXInputDevice(const GUID* pGuidProductFromDirectInput);

} } } }

#endif
#endif
