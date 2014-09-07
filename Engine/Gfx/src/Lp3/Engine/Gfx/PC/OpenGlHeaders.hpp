#ifndef _FILE_LP3_ENGINE_GFX_PC_OPENGLHEADERS_HPP
#define _FILE_LP3_ENGINE_GFX_PC_OPENGLHEADERS_HPP

    #include <Lp3/LE.h>

    #if defined(LP3_COMPILE_TARGET_LINUX)
        #include <GL/glut.h>
    #elif defined(LP3_COMPILE_TARGET_WINDOWS)
        #include <windows.h>
        #include <GL/gl.h>
        #include <gl/glu.h>
    #endif

#endif
