Lp3 Dependencies
----------------

This directory contains tools used for developing Lp3.


Lp3Project
----------

This maps platform specific dependencies to stand in libraries that everything
else can use to avoid worrying about the platform specific stuff.

This includes things like using Boost Test only on the PC or GLUT only on Unix.

The PC uses Boost Test to get all the advantages of the memory leak checker,
while the Dreamcast runs using a custom Python script. The Dreamcast is
the limiting factor.


Requirements
------------

This project uses all of the C++11 features available in gcc 4.7. The goal is
to target the Dreamcast but use the latest compiler available there.

The following environment variables are necessary:

DC_TOOL_PATH - This should be the absolute path to dc-tool.exe.
DREAMCAST_IP - The IP address of the Dreamcast. This is what is typically
               passed as the -t argument to dc-tool.
LIB          - On Windows, this may need to also include the path to Boost test.
LP3_ROOT_PATH- The root path to all media files (see "Media").

Additionally, the file "dctest.py" should be accessible on the path.


Dreamcast Support
-----------------

Install KOS for MinGW.
Set up the Boost Build helper scripts for it, as outline on my GitHub repo.
Basically, you'll need to update your site-config with a "dreamcast" toolset
for gcc, then build all the projects using "--toolset=gcc-dreamcast".


Media
-----

To simplify Media, all Lp3 projects use an environment variable called
"LP3_ROOT_PATH". This should point to some directory where all media files
(such as graphics and music) are located. This way, those files can live
outside of the Git managed code repository. The expectation is that each
project has its own directory that all files start with, so if the name of
the project is "DuckMystery" then all files for it would live in a directory
named "DuckMystery." There may be a better way to manage media files in
concert with source code but I do not know it.


Windows PATH Weirdness
----------------------

I've been using it for years, yet still Boost Test is one of the hardest
dependencies on Earth to make link correctly on Windows, even when you're using
Boost Build. As a result, you may need to add the Boost test library files
to the path, or to the "LIB" variable.


Environment Variable Review
---------------------------
DC_TOOL_PATH - See ''
DREAMCAST_IP

