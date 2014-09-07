#ifndef _FILE_LP3_ENGINE_RESOURCES_INPUTFILESTREAM_H
#define _FILE_LP3_ENGINE_RESOURCES_INPUTFILESTREAM_H

#include <Lp3/LE.h>

#ifdef LP3_COMPILE_TARGET_DREAMCAST
    #include <Lp3/Engine/Resources/DreamcastInputFileStream.h>

    namespace Lp3 { namespace Engine { namespace Resources {
        typedef ::Lp3::Engine::Resources::DreamcastInputFileStream InputFileStream;
    }}}
    //#include <Lp3/Engine/Resources/PcInputFileStream.h>

    //namespace Lp3 { namespace Engine { namespace Resources {
    //    typedef ::Lp3::Engine::Resources::PcInputFileStream InputFileStream;
    //}}}
#else
    #include <Lp3/Engine/Resources/PcInputFileStream.h>

    namespace Lp3 { namespace Engine { namespace Resources {
        typedef ::Lp3::Engine::Resources::PcInputFileStream InputFileStream;
    }}}
#endif

#endif
