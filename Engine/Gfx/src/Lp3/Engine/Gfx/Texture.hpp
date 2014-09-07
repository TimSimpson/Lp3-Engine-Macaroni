#ifndef _FILE_LP3_GFX_TEXTURE_H
#define _FILE_LP3_GFX_TEXTURE_H

#include <Lp3/LE.h>

#ifdef LP3_COMPILE_TARGET_DREAMCAST
    #include <Lp3/Engine/Gfx/Dreamcast/DreamcastTexture.h>

    namespace Lp3 { namespace Engine { namespace Gfx {
        typedef ::Lp3::Engine::Gfx::Dreamcast::DreamcastTexture Texture;
    }}}
#else
    #include <Lp3/Engine/Gfx/OpenGl/OpenGlTexture.h>

    namespace Lp3 { namespace Engine { namespace Gfx {
        typedef ::Lp3::Engine::Gfx::OpenGl::OpenGlTexture Texture;
    }}}

#endif

#endif
