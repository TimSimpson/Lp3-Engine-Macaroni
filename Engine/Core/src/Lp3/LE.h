#ifndef LE_H
#define LE_H

/*******************************************************************************
//MDOC-BEGIN CompilerFlags
  Custom flags I defined for this project years ago.

  Note that the tricks below which determine the platform may need to be
  updated as time goes by.

 Flags:

  LP3_COMPILE_TARGET_WINDOWS
  LP3_COMPILE_TARGET_DREAMCAST
  LP3_COMPILE_TARGET_LINUX
  LP3_COMPILE_TARGET_PC

  LP3_COMPILE_WITH_DEBUGGING
    If true, debug stuff is on, so feel free to take your time.

  LP3_COMPILE_WITHOUT_LOGGING
    If true, don't use logging even if in debug mode.

//MDOC-END CompilerFlags
 ******************************************************************************/

#define COMPILE_TARGET_WINDOWS syntax_error
#define COMPILE_TARGET_DREAMCAST syntax_error
#define COMPILE_TARGET_LINUX syntax_error
#define COMPILE_TARGET_PC syntax_error


#ifdef _WIN32
#ifndef LP3_COMPILE_TARGET_WINDOWS
    #define LP3_COMPILE_TARGET_WINDOWS
    // _DEBUG is a Microsoft specific flag for debug builds. Boost
    // seems to add it for any Windows builds.
    #ifdef _DEBUG
        #define LP3_COMPILE_WITH_DEBUGGING
    #endif
#endif
#endif

#ifdef LP3_COMPILE_TARGET_WINDOWS
    #define LP3_COMPILE_TARGET_WINDOWS_THEN(...) __VA_ARGS__
#else
    #define LP3_COMPILE_TARGET_WINDOWS_THEN(...) /* __VA_ARGS__ */
#endif

#ifdef _arch_dreamcast
  #ifndef LP3_COMPILE_TARGET_DREAMCAST
  	#define LP3_COMPILE_TARGET_DREAMCAST
  #endif
  // The following is used to turn off the asserts in stdlib. It's
  // GCC specific but that's the only compiler for Dreamcast anyway.
  #ifndef NDEBUG
        #define LP3_COMPILE_WITH_DEBUGGING
  #endif
#endif

#ifdef EMSCRIPTEN
  #define COMPILE_TARGET_EMSCRIPTEN
#endif

#ifndef LP3_COMPILE_TARGET_DREAMCAST
  #ifndef LP3_COMPILE_TARGET_WINDOWS
    #ifndef COMPILE_TARGET_EMSCRIPTEN
      #ifndef LP3_COMPILE_TARGET_LINUX
        #define LP3_COMPILE_TARGET_LINUX
        #ifndef NDEBUG
          #define LP3_COMPILE_WITH_DEBUGGING
        #endif
      #endif
    #endif
  #endif
#endif

#ifndef LP3_COMPILE_TARGET_PC
  #if defined(LP3_COMPILE_TARGET_LINUX) || defined(LP3_COMPILE_TARGET_WINDOWS)
    #define LP3_COMPILE_TARGET_PC
  #endif
#endif

#ifndef LP3_COMPILE_WITH_DEBUGGING
    #define LP3_COMPILE_LOG_DISABLE
#endif


#ifdef LP3_COMPILE_TARGET_DREAMCAST
  // Define size_t... suspicious its not otherwise defined. :/
  #include <cstddef>
#endif

#ifdef BOOST_TEST_MODULE
  #define LP3_COMPILE_TARGET_TEST
#endif

// nullptr stuff
#ifdef _MSC_VER
// Visual C++
  #if (_MSC_VER >= 1700)
  	// Don't define nullptr
  #else
	// In C++0x (C++ 2009) nullptr will become officially adopted by C++.
	#ifndef nullptr
		#define nullptr 0
	#endif
  #endif
#else
// Non Windows

#endif

#ifndef size_t
  #include <cstddef>
#endif


// Typedefs for byte length integer types.
namespace Lp3
{
	/* A single byte. Assume only values 0-127 can be used because it may or
	 * may not be signed. */
	typedef char Byte;
  static_assert(sizeof(Byte) == 1, "Wrong sizeof Byte.");

	/** An unsigned eight bit integer. */
	typedef unsigned char   UInt8;
  static_assert(sizeof(UInt8) == 1, "Wrong sizeof UInt8.");

	/** A signed eight bit integer. */
	typedef signed char      Int8;
  static_assert(sizeof(Int8) == 1, "Wrong sizeof Int8.");

	/** An unsigned 16 bit integer. */
	typedef unsigned short  UInt16;
  static_assert(sizeof(UInt16) == 2, "Wrong sizeof UInt16.");

	/** A signed 16 bit integer. */
	typedef signed short     Int16;
  static_assert(sizeof(Int16) == 2, "Wrong sizeof Int16.");

	/** An unsigned 32 bit integer. */
	typedef unsigned int    UInt32;
  static_assert(sizeof(UInt32) == 4, "Wrong sizeof UInt32.");

	/** A signed 32 bit integer. */
	typedef signed int       Int32;
  static_assert(sizeof(Int32) == 4, "Wrong sizeof Int32.");

	/** An unsigned 64 bit integer. */
	typedef unsigned long long UInt64;
  static_assert(sizeof(UInt64) == 8, "Wrong sizeof UInt64.");

	/** A signed 64 bit integer. */
	typedef signed long long Int64;
  static_assert(sizeof(Int64) == 8, "Wrong sizeof Int64.");

	typedef float          Float32;
  static_assert(sizeof(Float32) == 4, "Wrong sizeof Float32.");

    #ifdef LP3_COMPILE_TARGET_DREAMCAST
	   typedef long double Float64;
    #else
        typedef double     Float64;
    #endif
    static_assert(sizeof(Float64) == 8, "Wrong sizeof Float64.");
}

//// "textChar" is for eventual multi-lingual support.
//// textChar != CHAR or T_CHAR in Windows.
//// In other words, each character may eventually be more than 1 byte.
//typedef wchar_t textChar;
//typedef wchar_t * textCharPtr;
//typedef const wchar_t * constTextCharPtr;

/* Macros to make namespace definitions a bit cleaner in normal C++. */
#define BEGIN_NAMESPACE(a, b, c) namespace a{ namespace b{ namespace c{
#define END_NAMESPACE } } }

#define BEGIN_NAMESPACE2(a, b) namespace a{ namespace b{
#define END_NAMESPACE2 } }

#define BEGIN_NAMESPACE4(a, b, c, d) namespace a{ namespace b{ namespace c{ namespace d{
#define END_NAMESPACE4 } } } }

//#include "Engine/ErrOut.h"

// Throws an exception and includes source location.
#ifndef LP3_COMPILE_TARGET_DREAMCAST
  #define LP3_THROW(t, ...) { throw t(__VA_ARGS__); }
#else
  // Things are much more complicated in Dreamcast land... :(
  #define LP3_THROW_EXCEPTION_TO_STRING(type) #type
  #ifndef LP3_COMPILE_TARGET_TEST
     #define LP3_THROW(t, ...) { \
        printf("\n!!!!\tCRITICAL ERROR:\n!!!!\t\t\t%s\n", LP3_THROW_EXCEPTION_TO_STRING(t)); \
        printf("!!!!\tOn %s, line %d\n\n", __FILE__, __LINE__); \
        abort(); \
     }
  #else
     #define LP3_THROW(t, ...) { \
        Lp3::DreamcastTestAsserts::lastExceptionType=LP3_THROW_EXCEPTION_TO_STRING(t); \
      }
  #endif
#endif

#endif
