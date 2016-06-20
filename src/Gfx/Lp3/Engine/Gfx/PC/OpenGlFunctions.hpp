/*
 *  Defines versions of OpenGL functions which, if compiled in debug mode,
 *  will always check for errors after being invoked.
 *  This is necessary as some horrible bugs can be easily discovered this way.
 */
#ifndef FILE_LP3_ENGINE_GFX_PC_OPENGLFUNCTIONS_HPP
#define FILE_LP3_ENGINE_GFX_PC_OPENGLFUNCTIONS_HPP
#include <Lp3/LE.h>
#include <Lp3/Assert.h>
#include <Lp3/Log.h>
#ifndef LP3_COMPILE_TARGET_WINDOWS
    #include <GL/glut.h>
#else
    #pragma comment(lib, "Gdi32.lib")
    #pragma comment (lib, "user32.lib")
    #include <windows.h>
    #include <GL/gl.h>
    #include <gl/glu.h>
#endif



namespace Lp3 { namespace Engine { namespace Gfx { namespace OpenGl {

#ifdef LP3_COMPILE_WITH_DEBUGGING

inline const char * glErrorToString(const GLenum error)
{
    switch(error){
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_NO_ERROR:
            return "GL_NO_ERROR";
        case GL_STACK_OVERFLOW:
            return "GL_STACK_OVERFLOW";
        case GL_STACK_UNDERFLOW:
            return "GL_STACK_UNDERFLOW";
        case GL_OUT_OF_MEMORY:
            return "GL_OUT_OF_MEMORY";
        default:
            return "Mystery of Unknown Error";
    }
}

inline void LP3_OPENGL_FUNC_CHECK()
{
    GLenum error = glGetError();
    if (GL_NO_ERROR != error)
    {
        LP3_LOG_ERROR(glErrorToString(error));
        LP3_FAIL(glErrorToString(error));
    }
}

#else

#define LP3_OPENGL_FUNC_CHECK() /*  */

#endif


#define SAFE_NAME(name) safeGl##name

#define GL_NAME(name) gl##name

#define DEF_SAFE(name) \
    template<typename... Parameters> \
    inline void SAFE_NAME(name) (Parameters... args) \
    { \
        GL_NAME(name) (args...); \
        LP3_OPENGL_FUNC_CHECK(); \
    }

// Define "safe" (subjective, I know) versions of these functions which will
// halt the program in debug mode if any errors are detected.
DEF_SAFE(AlphaFunc);
DEF_SAFE(BindTexture);
DEF_SAFE(BlendFunc);
DEF_SAFE(Clear);
DEF_SAFE(DeleteTextures);
DEF_SAFE(Disable);
DEF_SAFE(Enable);
DEF_SAFE(End);
DEF_SAFE(FrontFace);
DEF_SAFE(GenTextures);
DEF_SAFE(LoadIdentity);
DEF_SAFE(MatrixMode);
DEF_SAFE(Ortho);
DEF_SAFE(ShadeModel);
DEF_SAFE(TexImage2D);
DEF_SAFE(TexSubImage2D);
DEF_SAFE(TexParameteri);
DEF_SAFE(SubImage2D);
DEF_SAFE(Viewport);


#define DEF_FAKE_SAFE(name) \
    template<typename... Parameters> \
    inline void SAFE_NAME(name) (Parameters... args) \
    { \
        GL_NAME(name) (args...); \
    }

// Also define "safe" versions of functions which don't even report errors,
// so things will be consistent. Note: OpenGL won't let you check for errors
// in between Begin / End.
DEF_FAKE_SAFE(Begin);
DEF_FAKE_SAFE(Color4f);
DEF_FAKE_SAFE(TexCoord2f);
DEF_FAKE_SAFE(Vertex3f);


#undef SAFE_NAME
#undef GL_NAME
#undef DEF_SAFE
#undef DEF_FAKE_SAFE


#ifdef LP3_COMPILE_WITH_DEBUGGING

#undef LP3_OPENGL_FUNC_CHECK

#endif

} } } }

#endif
