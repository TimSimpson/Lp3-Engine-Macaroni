local Path = require "Macaroni.IO.Path"
local cmfw = require "Macaroni.Generators.CMake.CMakeFileWriter"


project = context:Group("Lp3"):Project("Lp3-Engine"):Version("DEV")
core = project:Library{
    name="Lp3_Core",
    shortName="Lp3_Core",
    headers=pathList{"src/Core", "target"},
    sources=pathList{"src/Core", "src/Dependencies.mcpp"},
    usesBoost=true,
    excludeFiles = pathList{
        "src/Core/Demos/AssertDemo.cpp",
        "src/Core/Demos/Demo.cpp",
        "src/Core/Lp3/PrimitivesTest.cpp",
        "src/Core/Lp3/Engine/Memory/AllocatorTest.cpp",
        "src/Core/Lp3/Engine/Coroutine/CoroutineTest.cpp",
        "src/Core/Lp3/Engine/Memory/MemoryStackTest.cpp",
    }
}
input = project:Library{
    name="Lp3_Input",
    shortName="Lp3_Input",
    headers=pathList{"src/Input", "target"},
    sources=pathList{"src/Input", "src/Dependencies.mcpp"},
    usesBoost=true,
}
gfx = project:Library{
    name="Lp3_Gfx",
    shortName="Lp3_Gfx",
    headers=pathList{"src/Gfx", "target"},
    sources=pathList{"src/Gfx", "src/Dependencies.mcpp"},
    usesBoost=true,
    excludeFiles = pathList{
        "src/Gfx/Demos/Emscripten/Simple.cpp",
        "src/Gfx/Demos/PvrTextureDemo.cpp",
        "src/Gfx/Demos/Pvr2DShapesDemo.cpp",
        "src/Gfx/Demos/Pvr2DTexturedShapesDemo.cpp",
        "src/Gfx/Lp3/Engine/Gfx/PixelTest.cpp",
    }
}

function generate()
  local cpp = plugins:Get("Cpp")
  cpp:Run("Generate",
          { projectVersion=project, path=filePath("target"), output=output})

  local root = Path.New(getWorkingDirectory());
  cmfw.ProjectInfo(
      project,
      root:NewPathForceSlash("target/macaroni.cmake"))
end
