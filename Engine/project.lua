import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"

ParentProject{
  group="Lp3",
  project="Engine-All",
  version="0.1.0",
  children={
    "Core",
    "Input",
    "Gfx",
  }
}
