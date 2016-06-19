import("Lp3", "Lp3Project", "DEV")
require "Lp3Project"


Lp3Project{
  group="Lp3",
  project="Lp3.Engine.Gfx",
  version="DEV",
  src="src",
  target="target",
  libShortName="Lp3Gfx",
  dependencies = {
    load("Lp3", "Lp3.Engine.Core", "DEV"):Target("lib"),
    --load("Macaroni", "Windows", "1.0.0"):Target("lib"),
    load("Lp3", "Lp3Project", "DEV"):Target("Lp3GfxDeps"),
  },
   exes={
    "Demos/Emscripten/Simple.cpp",
    --"Demos/OpenGL/OglPlusDemo.cpp",
    "Demos/PvrTextureDemo.cpp",
    "Demos/Pvr2DShapesDemo.cpp",
   -- "Demos/PvrSpritesDemo.cpp",
    "Demos/Pvr2DTexturedShapesDemo.cpp",
    --"Demos/OpenGL/HelloWorld.cpp",
  },
   tests={
     "Lp3/Engine/Gfx/PixelTest.cpp",
  },

  -- bjamFlags={
  --     -- projectVersion=project,
  --     -- output=output,
  --     flags={ OpenGlDemo =[[<define>UNICODE<define>_UNICODE
  --       <toolset>msvc:<linkflags>/SUBSYSTEM:WINDOWS
  --       <toolset>msvc:<define>LP3_COMPILE_TARGET_WINDOWS
  --       # :3 KITTY <toolset-msvc:version>11.0
  --       ]],
  --     },
  --     requirements = [[
  --      <toolset>msvc:<define>LP3_COMPILE_TARGET_WINDOWS
  --     ]],
  -- },
};

