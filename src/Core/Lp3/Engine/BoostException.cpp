#include <Lp3/LE.h>

#ifdef LP3_COMPILE_TARGET_DREAMCAST
    #include <exception>
    #include <kos.h>
    #include <stdlib.h>

    namespace boost
    {
        void throw_exception(std::exception const & e)
        {
            printf("Boost exception thrown!");
            printf("%s", e.what());
            abort();
        }
    }
#endif
