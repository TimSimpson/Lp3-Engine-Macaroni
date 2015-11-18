project = context:Group("Lp3")
                 :Project("Lp3Project")
                 :Version("DEV");

-- This is a trick; the library exists only as an alias, defined in the
-- Boost Build file.

lib = project:Library{
    name="Lp3Test",
    shortName="Lp3Test",
    headers=pathList{},
    sources=pathList{},
    dependencies={},
    excludeFiles=pathList{},
};

lib = project:Library{
    name="Lp3GfxDeps",
    shortName="Lp3GfxDeps",
    headers=pathList{},
    sources=pathList{},
    dependencies={},
    excludeFiles=pathList{},
};

-- This gives access to the Project Template script.

project:Resource{name = "scripts", type="lua", files = pathList{"src"}}

function nothing()
end

clean = nothing
generate = nothing
build = nothing

local runCMake = function(cmd)
    output:WriteLine(cmd)
    local success, exit, number = os.execute(cmd)
    if (not success or exit ~= "exit" or number ~= 0) then
        output:ErrorLine("Failure running CMake!")
        output:ErrorLine(cmd)
        error("Failure running CMake!")
    end
end

function install()
    sinstall(project, filePath("./"))
    local installPath = findInstallPath(project):NewPathForceSlash("target");
    -- local cmakeListTxt = installPath:NewPathForceSlash("CMakeLists.txt");
    -- runCMake('cmake ' .. cmakeListTxt.AbsolutePathForceSlash
    --          .. ' -B' .. installPath.AbsolutePathForceSlash);
    -- runCMake('cmake --build ' .. installPath.AbsolutePathForceSlash);
end
