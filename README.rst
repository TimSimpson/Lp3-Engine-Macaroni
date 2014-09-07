LP3::Engine
===========

This is a rudimentary game engine designed to compile against Windows, the Dreamcast using KallistiOS, and eventually Emscripten and Linux.

Because it runs on such a low common denominator certain aspects are very primitive, such as the graphics API (which maps closely to the Dreamcast's PVR library).


Prereqs
=======
This requires the bleeding edge version of Macaroni as well as Boost and Boost Build. Dreamcast support also requires Cygwin and the Dreamcast tool chain included as part of KallistiOS. Debug output in Windows requires the Squirrel Console.

Building
========
Download the repo.

1. Enter the ./Dependencies/Lp3Project directory and run "macaroni install".
2. Enter the ./Engine directory and run macaroni install.

Demos
=====
By convention, Demos exist in the "src/Demos" directory of each project.


