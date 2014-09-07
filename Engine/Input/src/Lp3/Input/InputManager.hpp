#ifndef _FILE_LP3_ENGINE_INPUT_INPUTMANAGER_H
#define _FILE_LP3_ENGINE_INPUT_INPUTMANAGER_H

#include <Lp3/LE.h>

#ifdef LP3_COMPILE_TARGET_DREAMCAST
    #include <Lp3/Engine/Dreamcast/DreamcastDeviceSource.h>
#else
    #include <Lp3/Engine/DirectInput/DirectInputDeviceSource.h>

    namespace Lp3 { namespace Input {
        typedef ::Lp3::Engine::Gfx::OpenGl::OpenGlManager GfxManager;
    }}}
#endif

#endif
