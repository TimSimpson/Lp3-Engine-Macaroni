import("Lp3", "Lp3Project", "DEV")
require "Lp3Project"


Lp3Project{
  group="Lp3",
  project="Lp3.Engine.Input",
  version="DEV",
  src="src",
  target="target",
  libShortName="Lp3Input",
  dependencies = {
    load("Lp3", "Lp3.Engine.Core", "DEV"):Target("lib"),
    --load("Macaroni", "Windows", "1.0.0"):Target("lib"),
  },
  bjamFlags={
      projectVersion=project,
      output=output,
      flags={ DxDemo=[[<define>UNICODE<define>_UNICODE
        <toolset>msvc:<linkflags>/SUBSYSTEM:WINDOWS
        <toolset>msvc:<define>LP3_COMPILE_TARGET_WINDOWS
        # :3 KITTY <toolset-msvc:version>11.0
        ]],
      },
      -- requirements = [[
      --  <toolset>msvc:<define>LP3_COMPILE_TARGET_WINDOWS
      -- ]],
  },
};




-- require "os"

-- --------------------------------------------------------------------------------
-- -- Plugins
-- --------------------------------------------------------------------------------
-- bjam = plugins:Get("BoostBuild2")
-- cpp = plugins:Get("Cpp")
-- porg = plugins:Get("Porg")


-- --------------------------------------------------------------------------------
-- -- Dependencies
-- --------------------------------------------------------------------------------
-- boost  = loadBoostLib("headers")
-- --boostPython = load("Macaroni", "Boost-Python", "1.51"):Target("lib")
-- cppstd = load("Macaroni", "CppStd", "2003"):Target("lib")
-- windows  = load("Macaroni", "Windows", "1.0.0"):Target("lib")
-- lp3Core = load("Lp3", "Lp3.Engine.Core", "DEV"):Target("lib")
-- lp3Math = load("Lp3", "Lp3.Math", "0.1.0"):Target("lib")



-- --------------------------------------------------------------------------------
-- -- File Paths
-- --------------------------------------------------------------------------------
-- sourceRoot = "src"
-- outputPath = "target"


-- --------------------------------------------------------------------------------
-- -- Project Model Information
-- --------------------------------------------------------------------------------
-- -- Project Definition
-- project = context:Group("Lp3")
--                :Project("Lp3.Input")
--                :Version("0.1.0");

-- lib = project:Library{
--     name = "lib",
--     headers = pathList{sourceRoot, "target" },
--     sources = pathList{sourceRoot, ".mcpp$" },
--     dependencies = {
--         boost,
--         --boostPython,
--         cppstd,
--         lp3Core,
--         lp3Math,
--         windows
--     }
-- }
-- porg:Run("Generate", {target=lib})


-- --------------------------------------------------------------------------------
-- -- Actions
-- --------------------------------------------------------------------------------
-- generated = false
-- built = false
-- installed = false

-- function clean()
--     local targetDir = Macaroni.IO.Path.New(outputPath)
--     targetDir:ClearDirectoryContents();
-- end

-- function generate()
--   if generated then return end
--   local outputPath = filePath("target") --ath.New("target")
--   cpp:Run("Generate", { projectVersion=project, path=outputPath })
--   bjam:Run("Generate", { jamroot=outputPath:NewPath("/jamroot.jam"),
--                          projectVersion=project,
--                          output=output,
--                          flags={ DxDemo=[[<define>UNICODE<define>_UNICODE
--                             <toolset>msvc:<linkflags>/SUBSYSTEM:WINDOWS
--                             <toolset>msvc:<define>LP3_COMPILE_TARGET_WINDOWS
--                             ]],
--                          },
--                          requirements = [[
--                           <toolset>msvc:<define>LP3_COMPILE_TARGET_WINDOWS
--                          ]]
--   });
--   generated = true
-- end

-- function build()
--   if built then return end
--   generate()
--   if 0 == os.execute("bjam -d+2 target") then
--     os.execute("target\\exe\\DxDemo.exe")
--   end
-- end
