Dreamcast Test
--------------

This Python script executes dc-tool and monitors it's output for various
key strings which are interpretted as either test errors or test completion
(think TAP but a million times simpler). This way, it can check if the
tests actually passed and return a positive error code if it didn't.

It also sleeps after each run to keep the Dreamcast from getting confused.
It doesn't like getting hammered by dc-tool and will ignore it if you try.

The intention is to use this from Boost Build to run unit-tests compiled
with Lp3::Core, like follows:

unit-test my_test
    : test1.cpp
    :   <toolset-gcc:version>dreamcast
        <testing.launcher>"dctest "
    ;

This also involves "using" the Dreamcast version of GCC with Boost Build.


Requirements
------------

The following environment variables are necessary:

DC_TOOL_PATH - This should be the absolute path to dc-tool.exe.
DREAMCAST_IP - The IP address of the Dreamcast. This is what is typically
               passed as the -t argument to dc-tool.

Additionally, the file "dctest.py" should be accessible on the path to work
with Boost Build scripts.
