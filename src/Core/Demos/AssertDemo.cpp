#include <Lp3/Log.h>
#include <Lp3/Assert.h>
#include <Lp3/Log/LogSystem.h>



int main(int argc, char **argv)
{
    // In Windows, it's necessary to manually configure the logging system.
    Lp3::Log::LogSystem log;

    LP3_LOG_DEBUG("This is a test of the LP3 Asserts.");
    LP3_LOG_DEBUG("First, let's try some passing asserts...");

    LP3_ASSERT_TRUE_MESSAGE(true==true,
                            "This had better not fail (or be seen)!");

    LP3_LOG_DEBUG("Up next, let's try a failing assert. If this is a debug "
                  "build then the program needs to fail! :)");
    LP3_ASSERT_TRUE_MESSAGE(false,
                            "This had BETTER fail in debug builds!");
    LP3_LOG_DEBUG("If you're reading this, the assert above did not fire.");
}
