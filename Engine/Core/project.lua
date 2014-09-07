import("Lp3", "Lp3Project", "DEV") -- ProjectTemplates", "1")
require "Lp3Project"


Lp3Project{
  group="Lp3",
  project="Lp3.Engine.Core",
  version="DEV",
  libShortName="Lp3_Core",
  dependencies={
    load("Macaroni", "CppStd", "2003"):Target("lib"),
    loadBoostLib("headers"),
  },
  testDependencies={
    -- load("Lp3", "Lp3Project", "DEV"):Target("Lp3Test"),
    --loadBoostLib("unit_test_framework"),
    --loadBoostLib("test_exec_monitor"),
  },
  -- bjamFlags={
  --   testVariants = { "", [[<toolset-gcc:version>dreamcast
  --       <testing.launcher>"dctest " ]]}
  -- },
  exes={
    "Demos/AssertDemo.cpp",
    "Demos/Demo.cpp",
  },
  tests={
     "Lp3/PrimitivesTest.cpp",
     "Lp3/Engine/Memory/AllocatorTest.cpp",
     "Lp3/Engine/Coroutine/CoroutineTest.cpp",
     "Lp3/Engine/Memory/MemoryStackTest.cpp",
  },
};
