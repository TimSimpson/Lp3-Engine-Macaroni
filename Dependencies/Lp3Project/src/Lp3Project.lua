import("Macaroni", "ProjectTemplates", "1")
require "SimpleProject"


local cmake = plugins:Get("CMake")

function Lp3Project(args)
    local newArgs = {}
    for k, v in pairs(args) do
        newArgs[k] = v
    end

    local testDeps = newArgs.testDependencies or {}
    testDeps[#testDeps + 1] = load("Lp3", "Lp3Project", "DEV"):Target("Lp3Test")
    newArgs.testDependencies = testDeps

    -- Make sure tests always include the following variant, which uses
    -- the Dreamcast launcher.
    newArgs.bjamFlags = args.bjamFlags or {}
    local testVariants = newArgs.bjamFlags.testVariants or {}
    if #testVariants == 0 then
        testVariants[#testVariants + 1] = ""
    end
    testVariants[#testVariants + 1] =  [[<toolset-gcc:version>dreamcast
            <testing.launcher>"dctest " ]];
    testVariants[#testVariants + 1] =  [[<toolset-clang:version>emscripten
            <testing.launcher>"node " ]];
    newArgs.bjamFlags.testVariants = testVariants;

    newArgs.bjamFlags.platformRules = {
        dreamcast = [[ <toolset-gcc:version>dreamcast ]],
        windows = [[ <toolset-msvc:version>12.0 ]],
    };
    newArgs.dependencies = newArgs.dependencies or {}
    newArgs.dependencies[#newArgs.dependencies + 1] = loadBoostLib("headers");

    local runCMake = function(cmd)
        output:WriteLine(cmd)
        if (os.execute(cmd) ~= 0) then
            output:ErrorLine("Failure running CMake!")
            output:ErrorLine(cmd)
            error("Failure running CMake!")
        end
    end

    -- Don't build with Boost.
    -- newArgs.buildWithBoost = false

    -- local outputPath = filePath(args.target or "target");

    newArgs.cmakeFlags = args.cmakeFlags or {}
    newArgs.cmakeFlags.invoke = false;
    if properties.invoke_cmake then
        newArgs.cmakeFlags.invoke = true;
    end
    newArgs.cmakeFlags.preBuildCommand = { name='macaroni', args={"--generateLess"}}
    local p = SimpleProject(newArgs);
    -- Save functions created by SimpleProject.
    -- local simpleGenerate = generate;
    -- -- local simpleBuild = build;

    -- -- -- Override with CMake goodness.
    -- generate = function()
    --     simpleGenerate()
    --     runCMake('cmake ' .. cmakeListsPath.AbsolutePathForceSlash
    --              .. ' -B' .. outputPath.AbsolutePathForceSlash))
    -- end
    -- build = function()
    --     simpleBuild()

    -- end
    return p;
end
