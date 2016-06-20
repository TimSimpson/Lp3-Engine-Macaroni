// This header file is designed to make it easy to create platform specific
// executables that can be safely built on other platforms.
#ifdef FILE_LP3_ENGINE_NO_OPT_MAIN_H
    #error This file is meant to be included only once, from a single unit.
#endif
#ifndef PLATFORM_NAME
    #error Specify the string literal PLATFORM_NAME before including this file.
#endif
#define FILE_LP3_ENGINE_NO_OPT_MAIN_H

#ifdef LP3_COMPILE_TARGET_WINDOWS
    #include <Lp3/Log/LogSystem.h>
#endif
#include <Lp3/Log.h>

int main(int argc, char **argv) {
    #ifdef LP3_COMPILE_TARGET_WINDOWS
        Lp3::Log::LogSystem log;
    #endif
    LP3_LOG_DEBUG("This exe is intended soley for the " PLATFORM_NAME ".");
    return 0;
}

