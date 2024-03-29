# LP3::Engine

[![Build status](https://ci.appveyor.com/api/projects/status/0h294w0e2mmrgu4y?svg=true)](https://ci.appveyor.com/project/TimSimpson/lp3-engine)

[![badge](https://img.shields.io/badge/conan.io-Lp3-Engine%2F0.0.4.0-green.svg?logo=data:image/png;base64%2CiVBORw0KGgoAAAANSUhEUgAAAA4AAAAOCAMAAAAolt3jAAAA1VBMVEUAAABhlctjlstkl8tlmMtlmMxlmcxmmcxnmsxpnMxpnM1qnc1sn85voM91oM11oc1xotB2oc56pNF6pNJ2ptJ8ptJ8ptN9ptN8p9N5qNJ9p9N9p9R8qtOBqdSAqtOAqtR%2BrNSCrNJ/rdWDrNWCsNWCsNaJs9eLs9iRvNuVvdyVv9yXwd2Zwt6axN6dxt%2Bfx%2BChyeGiyuGjyuCjyuGly%2BGlzOKmzOGozuKoz%2BKqz%2BOq0OOv1OWw1OWw1eWx1eWy1uay1%2Baz1%2Baz1%2Bez2Oe02Oe12ee22ujUGwH3AAAAAXRSTlMAQObYZgAAAAFiS0dEAIgFHUgAAAAJcEhZcwAACxMAAAsTAQCanBgAAAAHdElNRQfgBQkREyOxFIh/AAAAiklEQVQI12NgAAMbOwY4sLZ2NtQ1coVKWNvoc/Eq8XDr2wB5Ig62ekza9vaOqpK2TpoMzOxaFtwqZua2Bm4makIM7OzMAjoaCqYuxooSUqJALjs7o4yVpbowvzSUy87KqSwmxQfnsrPISyFzWeWAXCkpMaBVIC4bmCsOdgiUKwh3JojLgAQ4ZCE0AMm2D29tZwe6AAAAAElFTkSuQmCC)](http://www.conan.io/source/Lp3-Engine/0.0.4.0/TimSimpson/testing)

This is a rudimentary game engine designed to compile against Windows, the Dreamcast using KallistiOS, and eventually Emscripten and Linux.

Because it runs on such a low common denominator certain aspects are very primitive, such as the graphics API (which maps closely to the Dreamcast's PVR library).

NOTE: The code for the Dreamcast hasn't been touched in two years. I recently made some of it compile again but most of it is still broken.


## Prereqs

### Windows

Building this requires CMake, Conan, and Macaroni for C++.

This engine logs to [Squirrel Console](https://github.com/TimSimpson/SquirrelConsole) on Windows, and falls back to standard out otherwise. Logging
only happens in debug builds.

### Dreamcast

You'll need a typical KallistiOS toolchain, CMake, and Macaroni for C++.

## Building

### Windows

Assuming all of those things are installed and available from the path, try
this:

    $ mkdir build
    $ cd build
    $ conan install ..
    $ conan build ..

This code was Dreamcast compatible at some point in the past; now, it is
anyone's guess. Support for this will be resurrected the toolchain even gets
past GCC 4.7.

### Dreamcast

    $ mkdir build-dc
    $ cd build-dc
    $ cmake -H"../" -B"./" -DDREAMCAST=true -DCMAKE_TOOLCHAIN_FILE="../Dreamcast.cmake" -G "Unix Makefiles"
    $ make


## Media

To simplify Media, all Lp3 projects use an environment variable called
"LP3_ROOT_PATH". This should point to some directory where all media files
(such as graphics and music) are located. This way, those files can live
outside of the Git managed code repository. The expectation is that each
project has its own directory that all files start with, so if the name of
the project is "DuckMystery" then all files for it would live in a directory
named "DuckMystery." There may be a better way to manage media files in
concert with source code but I do not know it.

## Components

### Lp3 :: Core

Contains the following bits and pieces:

    * Assertions
    * Compiler constants, used for porting in the rest of the app
    * Logging
    * Resource loading
    * Stackless coroutines
    * Memory allocators
    * Time functions

### Lp3 :: Input

Abstracts controller and device input for use with games. The goal is to
provide access to any possible control, while allowing games to focus on
something more abstract.

Here are the important abstractions:

Device -
    Represents a hardware device, like a USB gamepad or a keyboard.
    These are obtained using the factory class DeviceSource's, of which there
    are several implementations for the different platforms.

ButtonMapping -
    Maps a Device to a Key on that device, and can query it's state. For
    example, this could map to a single button a USB gamepad.

Control -
    What a game would think of a controller. A collection of ButtonMappings
    which can be rebound easily, allowing for configurable controls.

### Lp3 :: Gfx

Very low level graphics API. Designed to support the Dreamcast PVR graphics
hardware using KallistiOS's low level functions.
