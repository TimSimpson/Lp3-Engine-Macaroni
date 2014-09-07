#ifndef FILE_LP3_ENGINE_BINARY
#define FILE_LP3_ENGINE_BINARY


#ifndef LP3_COMPILE_TARGET_LINUX
    #define LP3_ENGINE_BINARY_STRUCT_BEGIN(A) \
        struct A { \
        #pragma pack(push, 1)

    #define LP3_ENGINE_BINARY_STRUCT_END() \
        #pragma pack(pop) \
        };
#endif

#ifdef LP3_COMPILE_TARGET_LINUX
    #define LP3_ENGINE_BINARY_STRUCT_BEGIN(A) \
        struct A {

    #define LP3_ENGINE_BINARY_STRUCT_END() \
        } \
        __attribute__((packed)) \
        ;
#endif


#endif
