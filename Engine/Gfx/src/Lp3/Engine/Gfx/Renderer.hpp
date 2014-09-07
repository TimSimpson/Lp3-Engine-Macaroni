#ifndef _FILE_LP3_GFX_RENDERER_H
#define _FILE_LP3_GFX_RENDERER_H

#include <Lp3/LE.h>

#ifdef LP3_COMPILE_TARGET_DREAMCAST
    #include <Lp3/Engine/Gfx/Dreamcast/DreamcastRenderer.h>

    namespace Lp3 { namespace Engine { namespace Gfx {
        typedef ::Lp3::Engine::Gfx::Dreamcast::DreamcastRenderer Renderer;
    }}}
#else
    #include <Lp3/Engine/Gfx/OpenGl/OpenGlRenderer.h>

    namespace Lp3 { namespace Engine { namespace Gfx {
        typedef ::Lp3::Engine::Gfx::OpenGl::OpenGlRenderer Renderer;
    }}}
#endif

#endif
