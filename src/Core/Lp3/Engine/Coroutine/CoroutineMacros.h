#ifdef LP3_COROUTINE_FILE_INCLUDED
#error "This file should be included *not* in headers but in .cpp files."
#endif

#define LP3_COROUTINE_FILE_INCLUDED

#include <Lp3/LE.h>
#include <Lp3/Log.h>
#include <Lp3/Engine/Coroutine/CoroutineState.h>
#include <Lp3/Engine/Coroutine/CoroutineFinishedException.h>
#include <Lp3/Engine/Coroutine/CoroutineInvalidStateException.h>

#define LP3_COROUTINE_BEGIN(variable_name) \
    { const ::Lp3::Engine::Coroutine::CoroutineState & lp3CoroutineState = variable_name; } \
    int & lp3CoroutineCodePointer = variable_name.codePointer; \
    switch(lp3CoroutineCodePointer) \
    { \
        case(0):

#define LP3_LABELL(LABEL_NAME, VALUE) \
        LABEL_NAME: \
            lp3CoroutineCodePointer=VALUE; \
        case(VALUE):


#define LP3_YIELDL(VALUE, ...) \
            lp3CoroutineCodePointer=VALUE; \
            return __VA_ARGS__;                     \
        case(VALUE):


#define LP3_LABEL(LABEL_NAME) LP3_LABELL(LABEL_NAME, __LINE__)
#define LP3_YIELD(...) LP3_YIELDL(__LINE__, __VA_ARGS__)

#define LP3_COROUTINE_END() \
            lp3CoroutineCodePointer = -1; \
            break; \
        case(-1): \
            LP3_LOG_ERROR("Coroutine has already ended!"); \
            LP3_THROW(::Lp3::Engine::Coroutine::CoroutineFinishedException); \
            break; \
        default: \
            LP3_LOG_ERROR("Coroutine is in an invalid state!"); \
            LP3_THROW(::Lp3::Engine::Coroutine::CoroutineInvalidStateException); \
            break; \
    }

// TODO: Consider using BOOST_PP_SLOT - http://www.boost.org/doc/libs/1_54_0/libs/preprocessor/doc/ref/slot.html
//       Should be possible to get range within one byte limit, which could
//       help.
