#ifndef _FILE_LP3_ENGINE_GFX_GFXMANAGER_H
#define _FILE_LP3_ENGINE_GFX_GFXMANAGER_H

#include <Lp3/LE.h>

#ifdef LP3_COMPILE_TARGET_DREAMCAST
    #include <Lp3/Engine/Gfx/Dreamcast/DreamcastGfxManager.h>

    namespace Lp3 { namespace Engine { namespace Gfx {
        typedef ::Lp3::Engine::Gfx::Dreamcast::DreamcastGfxManager GfxManager;
    }}}
#else
    #include <Lp3/Engine/Gfx/OpenGl/OpenGlManager.h>

    namespace Lp3 { namespace Engine { namespace Gfx {
        typedef ::Lp3::Engine::Gfx::OpenGl::OpenGlManager GfxManager;
    }}}
#endif

#endif
