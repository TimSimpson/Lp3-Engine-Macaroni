/************************************************************************/
/*  Abort method.                                                       */
/*  Calls "abort" in Debug, throws in Release mode.                     */
/************************************************************************/
#ifndef FILE_LP3_ABORT_H
#define FILE_LP3_ABORT_H

#include <Lp3/Log.h>
#include <string>

#ifdef LP3_COMPILE_WITH_DEBUGGING
    #include <stdlib.h>
#else
    #include <Lp3/Exception.h>
#endif

#ifdef LP3_COMPILE_WITH_DEBUGGING
    #define LP3_ABORT(m, ...)    \
    {   \
        LP3_LOG_CRITICAL(m, #__VA_ARGS__); \
        abort(); \
    }
#else
    #define LP3_ABORT(m, ...)    \
    {   \
        LP3_LOG_CRITICAL(m, #__VA_ARGS__); \
        LP3_THROW(Lp3::Exception, m); \
    }
#endif

#endif // File Include Guard
