#include "Log.h"

#ifdef LP3_COMPILE_TARGET_WINDOWS
    #include <Lp3/Engine/Log/ErrOutPipe.h>
#endif
#ifdef LP3_COMPILE_TARGET_DREAMCAST
    #include <kos.h> 
#endif
#if defined(LP3_COMPILE_TARGET_LINUX) || defined(LP3_COMPILE_TARGET_EMSCRIPTEN)
    #include <stdio.h>
#endif

namespace Lp3 { namespace Log {

#ifdef LP3_COMPILE_TARGET_WINDOWS
    namespace {
        int priority(const LogLevel & level) {
            switch(level) {
                case LogLevel::Critical:
                    return 0;
                case LogLevel::Debug:
                    return 255;
                case LogLevel::Error:
                    return 63;
                case LogLevel::Info:
                    return 191;
                case LogLevel::Warning:
                    return 127;
                default:
                    return 0;
            }
        }
    }
    
    void WriteLog(const char * const filename, const int lineNumber,
                  const LogLevel & level, const char * const message)
    {
        Lp3::Engine::Log::ErrOutPipe::WriteLinePL(filename, lineNumber,
                                                  priority(level), message);
    }
#endif

#if defined(LP3_COMPILE_TARGET_DREAMCAST) || defined(LP3_COMPILE_TARGET_LINUX) || defined(LP3_COMPILE_TARGET_EMSCRIPTEN)
    void WriteLog(const char * const filename, const int lineNumber,
                  const LogLevel & level, const char * const message)
    {
        ::printf("%s %d : ", filename, lineNumber);
        ::printf(message);
        ::printf("\n");
    }
#endif

} }
