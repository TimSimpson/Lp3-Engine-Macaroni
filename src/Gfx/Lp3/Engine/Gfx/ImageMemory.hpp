#ifndef _FILE_LP3_GFX_IMAGEMEMORY_H
#define _FILE_LP3_GFX_IMAGEMEMORY_H

#include <Lp3/LE.h>

#ifdef LP3_COMPILE_TARGET_DREAMCAST
    #include <Lp3/Engine/Gfx/Dreamcast/DreamcastImageMemory.h>

    namespace Lp3 { namespace Engine { namespace Gfx {
        typedef ::Lp3::Engine::Gfx::Dreamcast::DreamcastImageMemory ImageMemory;
    }}}
#else
    #include <Lp3/Engine/Gfx/OpenGl/OpenGlImageMemory.h>

    namespace Lp3 { namespace Engine { namespace Gfx {
        typedef ::Lp3::Engine::Gfx::OpenGl::OpenGlImageMemory ImageMemory;
    }}}
#endif

#endif
