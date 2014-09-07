// This header file is designed to make it easy to create multiplatform
// executables. Nothing huge, of course, but enough to make a demo.

#ifdef FILE_LP3_ENGINE_MICROMAIN_H
    #error This file is meant to be included only once, from a single unit.
#endif
#define FILE_LP3_ENGINE_MICROMAIN_H

#include <Lp3/LE.h>
#include <string>
#include <vector>

#ifdef LP3_COMPILE_TARGET_WINDOWS
    #pragma comment(lib, "Gdi32.lib")
    #pragma comment(lib, "shell32.lib")
    #pragma comment(lib, "user32.lib")
    #pragma comment(linker, "/SUBSYSTEM:WINDOWS")
    #include <windows.h>
    #include <shellapi.h>
    #include <Lp3/Log/LogSystem.h>
    #include <Lp3/Engine/Misc/WCharToCharConverter.h>
#endif

#include <Lp3/Log.h>
#include <Lp3/Engine/DemoUtils.h>


#ifndef LP3_COMPILE_TARGET_WINDOWS

#if defined(LP3_COMPILE_TARGET_LINUX) && defined(LP3_MICROMAIN_WITH_GFX_DEPS)

#include <GL/glut.h>


const unsigned int window_width  = 640;
const unsigned int window_height = 480;

template<typename MicroMainType>
int main(int argc, char **argv) {
    static MicroMainType * microMainPtr = nullptr;
    struct CallBack {
        void main_loop_function()
        {
            if (microMainPtr) {
                microMainPtr->Iterate();
            }
        }
    };

    glutInit(&argc, argv);
    glutInitWindowSize(window_width, window_height);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutCreateWindow("GLUT Example!!!");
    glutIdleFunc(CallBack::main_loop_function);

    std::vector<std::string> args;
    for (int index = 0; index < argc; index ++)
    {
        args.push_back(argv[index]);
    }

    MicroMainType microMain(args);
    microMainPtr = &microMain;
    glutMainLoop();
    return 0;
}

#define LP3_MICROMAIN(T, N) \
    int main(int argc, char **argv) { \
        return GenericMain<T>(argc, argv); \
    }

#define LP3_MICROMAIN_WINDOWS_ARGS(...)

#else  // Not Linux, but not Windows

template<typename MicroMainType>
int GenericMain(int argc, char **argv) {
    std::vector<std::string> args;
    for (int index = 0; index < argc; index ++)
    {
        args.push_back(argv[index]);
    }
    MicroMainType microMain(args);
    bool quit = false;
    while(!quit) {
        microMain.Iterate();
        #ifdef LP3_COMPILE_TARGET_DREAMCAST
            using Lp3::Engine::DemoUtils;
            if (DemoUtils::DreamcastStartIsPressed()) {
                quit = true;
            }
        #endif
    }
    return 0;
}

#define LP3_MICROMAIN(T, N) \
    int main(int argc, char **argv) { \
        return GenericMain<T>(argc, argv); \
    }

#define LP3_MICROMAIN_WINDOWS_ARGS(...)

#endif

#else // Windows

template<typename MicroMainType>
int WINAPI Lp3WinMain(const char * const programName,
                      HINSTANCE hInstance, HINSTANCE hPrevInstance,
                      LPSTR lpCmdLine, int nCmdShow)
{
    static MicroMainType * microMainPtr = nullptr;

    struct CallBack {

        static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
        {
            switch(message)
            {
                case WM_DESTROY:
                    {
                        microMainPtr = nullptr;
                        PostQuitMessage(0);
                        return 0;
                    } break;
            }
            if (microMainPtr)
            {   // I don't *think* this is a race condition. :/
                microMainPtr->WindowsMessage(hWnd, message, wParam, lParam);
            }

            return DefWindowProc (hWnd, message, wParam, lParam);
        }
    };

    using Lp3::Engine::Misc::WCharToCharConverter;
    Lp3::Log::LogSystem logSystem;
    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = CallBack::WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = "WindowClass";

    RegisterClassEx(&wc);

    hWnd = CreateWindowEx(NULL,
                          "WindowClass",
                          programName,
                          WS_OVERLAPPEDWINDOW,
                          0, 0,
                          640, 480,
                          NULL,
                          NULL,
                          hInstance,
                          NULL);

    int argLength;
    LPWSTR * windowsString = CommandLineToArgvW(GetCommandLineW(),
                                                &argLength);
    if (NULL == windowsString)
    {
        LP3_LOG_ERROR("Error converting command line arguments.");
        return 1;
    }
    std::vector<std::string> arguments;
    for(int i = 0; i < argLength; i ++)
    {
        WCharToCharConverter original(windowsString[i]);
        arguments.push_back(original.GetCharPtr());
    }
    LocalFree(windowsString);

    MSG msg;
    {
        MicroMainType microMain(arguments, hInstance, hWnd);
        microMainPtr = &microMain;

        ShowWindow(hWnd, nCmdShow);

        // enter the main loop:
        while(true)
        {
            while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }

            if(msg.message == WM_QUIT)
            {
                break;
            }
            else
            {
                microMain.Iterate();
            }
        }

        microMainPtr = nullptr;
    }  // Farewell, microMain.

    DestroyWindow(hWnd);
    return msg.wParam;
}


#define LP3_MICROMAIN(T, N) \
    int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, \
                       LPSTR lpCmdLine, int nCmdShow) \
    {   \
        return Lp3WinMain<T>(N, hInstance, hPrevInstance, lpCmdLine, nCmdShow); \
    }

#define LP3_MICROMAIN_WINDOWS_ARGS(...) __VA_ARGS__

#endif
