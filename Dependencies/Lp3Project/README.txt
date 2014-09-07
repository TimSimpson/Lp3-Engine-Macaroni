Lp3 Project
===========
This project assists in creating Lp3 style projects which need to work
on both PC and the Dreamcast.

Cross Platform Dependency Tracking
----------------------------------
Macaroni's current support for cross-platform dependency mapping is
non-existent, so this project gets around it by using hand-crafted files in
the "target" directory that resemble what Macaroni might have created using
the Boost Build and CMake plugins. These files map dependencies for Windows,
Unix, Emscripten and Dreamcast appropriately so other projects don't have to
worry about them.

Lp3Project Lua Code
----------------------
This project also includes some Lua code which expands on the "SimpleProject"
template found as part of Macaroni's included libraries to create "Lp3Project"
which simply includes all necessary base and test dependencies.
