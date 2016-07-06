project(Lp3-Engine CXX)
cmake_minimum_required(VERSION 2.8)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(OpenGL REQUIRED)

include_directories(
    src/Core
    src/Gfx
    src/Input
    target
    ${CONAN_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS})



include(target/macaroni.cmake)

if (MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
endif()

###############################################################################
#  Run Macaroni in Visual Studio
###############################################################################

add_custom_target(
    invoke_macaroni
    COMMAND macaroni ${CMAKE_CURRENT_LIST_DIR} -g
)

###############################################################################
#  Extra Functions
###############################################################################
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})

function(create_exe name src other_libs)
    add_executable(${name} ${src})
    target_link_libraries(${name} ${other_libs} ${CONAN_LIBS})
endfunction()

function(create_test name src other_libs)
    create_exe(${name} ${src} ${other_libs})
add_test(test_${name} ${EXECUTABLE_OUTPUT_PATH}/bin/${name})
    add_dependencies(check ${name})
endfunction()


###############################################################################
#  Core
###############################################################################

add_library(Lp3_Core
    ${MACARONI_LIBS_Lp3_Core_CPP_FILES}
    ${MACARONI_LIBS_Lp3_Core_SRC_FILES})
set_source_files_properties(
    ${MACARONI_LIBS_Lp3_Core_SRC_FILES}
    PROPERTIES HEADER_FILE_ONLY TRUE)
target_link_libraries(Lp3_Core ${CONAN_LIBS})

# If project.lua is found, invoke Macaroni. Otherwise ignore for package builds.
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/project.lua")
    add_dependencies(Lp3_Core invoke_macaroni)
endif()

create_test(PrimitivesTest src/Core/Lp3/PrimitivesTest.cpp Lp3_Core)
create_test(AllocatorTest src/Core/Lp3/Engine/Memory/AllocatorTest.cpp Lp3_Core)
create_test(CoroutineTest src/Core/Lp3/Engine/Coroutine/CoroutineTest.cpp Lp3_Core)
create_test(MemoryStackTest src/Core/Lp3/Engine/Memory/MemoryStackTest.cpp Lp3_Core)
create_test(ResourceDemo target/ResourceDemo.cpp Lp3_Core)

create_exe(AssertDemo src/Core/Demos/AssertDemo.cpp Lp3_Core)
create_exe(Demo src/Core/Demos/Demo.cpp Lp3_Core)
create_exe(MicroMainDemo target/MicroMainDemo.cpp Lp3_Core)


###############################################################################
#  Gfx
###############################################################################
add_library(Lp3_Gfx
    ${MACARONI_LIBS_Lp3_Gfx_CPP_FILES}
    ${MACARONI_LIBS_Lp3_Gfx_SRC_FILES})
set_source_files_properties(
    ${MACARONI_LIBS_Lp3_Gfx_SRC_FILES}
    PROPERTIES HEADER_FILE_ONLY TRUE)
target_link_libraries(Lp3_Gfx Lp3_Core ${CONAN_LIBS}
                      ${OPENGL_LIBRARIES})

create_test(PixelTest src/Gfx/Lp3/Engine/Gfx/PixelTest.cpp Lp3_Gfx)

create_exe(DrawText target/DrawText.cpp Lp3_Gfx)
create_exe(EmscriptenSimple src/Gfx/Demos/Emscripten/Simple.cpp Lp3_Gfx)
create_exe(HighResolution target/HighResolution.cpp Lp3_Gfx)
create_exe(LoadImage target/LoadImage.cpp Lp3_Gfx)
create_exe(MotivatingExample target/MotivatingExample.cpp Lp3_Gfx)
create_exe(OpenGlDemo target/OpenGlDemo.cpp Lp3_Gfx)
create_exe(ResizeWindow target/ResizeWindow.cpp Lp3_Gfx)
create_exe(ShowTexture target/ShowTexture.cpp Lp3_Gfx)
create_exe(Pvr2DShapesDemo src/Gfx/Demos/Pvr2DShapesDemo.cpp Lp3_Gfx)
create_exe(Pvr2DTexturedShapesDemo src/Gfx/Demos/Pvr2DTexturedShapesDemo.cpp
           Lp3_Gfx)
create_exe(PvrTextureDemo src/Gfx/Demos/PvrTextureDemo.cpp Lp3_Gfx)



###############################################################################
#  Windows
###############################################################################
add_library(Lp3_Windows
    ${MACARONI_LIBS_Lp3_Windows_CPP_FILES}
    ${MACARONI_LIBS_Lp3_Windows_SRC_FILES})
set_source_files_properties(
    ${MACARONI_LIBS_Lp3_Windows_SRC_FILES}
    PROPERTIES HEADER_FILE_ONLY TRUE)
target_link_libraries(Lp3_Windows Lp3_Gfx Lp3_Core ${CONAN_LIBS}
                      ${OPENGL_LIBRARIES})

create_exe(ToolsDemo target/ToolsDemo.cpp Lp3_Windows)

###############################################################################
#  Input
###############################################################################

add_library(Lp3_Input
    ${MACARONI_LIBS_Lp3_Input_CPP_FILES}
    ${MACARONI_LIBS_Lp3_Input_SRC_FILES})
set_source_files_properties(
    ${MACARONI_LIBS_Lp3_Input_SRC_FILES}
    PROPERTIES HEADER_FILE_ONLY TRUE)
target_link_libraries(Lp3_Input Lp3_Core ${CONAN_LIBS})

create_exe(InputDemo target/Lp3_Engine_Input_Demo.cpp Lp3_Input)
create_exe(DreamcastDemo target/DreamcastDemo.cpp Lp3_Input)
create_exe(DxDemo target/DxDemo.cpp Lp3_Input)
set_target_properties(DxDemo
    PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
create_exe(XInputDemo target/XInputDemo.cpp Lp3_Input)
set_target_properties(XInputDemo
    PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")



###############################################################################
#  Sfx
###############################################################################
add_library(Lp3_Sfx
    ${MACARONI_LIBS_Lp3_Sfx_CPP_FILES}
    ${MACARONI_LIBS_Lp3_Sfx_SRC_FILES})
set_source_files_properties(
    ${MACARONI_LIBS_Lp3_Sfx_SRC_FILES}
    PROPERTIES HEADER_FILE_ONLY TRUE)
target_link_libraries(Lp3_Sfx Lp3_Core ${CONAN_LIBS})


create_exe(SoundDemo target/SoundDemo.cpp Lp3_Gfx)
target_link_libraries(SoundDemo Lp3_Input Lp3_Sfx Xaudio2)
