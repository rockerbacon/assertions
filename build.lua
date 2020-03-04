#!/usr/bin/env lua5.3

local dirs = require("dependency_manager.directories")
local fs = require("dependency_manager.filesystem")

local description = "Build targets to ./build/release or ./build/tests depending on whether the target is a test or not"
local usage_help = "./build.lua [ACTION]\nIf no ACTION is provided then all possible targets will be built"
local args_help = [[
	target TARGET_LIST
		build specific targets.
		Each target is a source file,
		with or without the .cpp prefix.
		Targets should be separated with spaces
	clean
		delete all built binaries and auto-generated configurations
		(everything inside ./build)
	cmake
		execute cmake to auto-generate configurations,
		but do not build anything
	compile-commands
		execute cmake and link compile commands
		to the root of the project.
		Useful for integrating the project with certain external tools
]]

function print_help()
	print(description)
	print()
	print("Usage:", usage_help)
	print()
	print("ACTION:")
	print(args_help)
end

local action_map = {
	["clean"] = function()
		local success, error_msg = fs.rmrecur(dirs.build)
		if not success then
			error(error_msg)
		end
	end,

	["cmake"] = function()

	end,

	["compile-commands"] = function()

	end,

	["all"] = function()

	end,

	["target"] = function()

	end,

	["--help"] = function()
		print_help()
	end
}

function main()
	local action_name = arg[1]

	assert(action_name, "please specify an action to be performed")

	local action = action_map[action_name]

	assert(action, "invalid action '"..action_name.."'")
    -- os.execute(directories.root.."/dependencies.sh install")

    -- action(parsed_args.target_list)
	action()
end

local successful_execution, error_message = pcall(main)

if not successful_execution then
	local error_message_without_trace = error_message:gsub("^.*:%d+:", "", 1)
    print("Error:", error_message_without_trace)
	print()
	print("Help:")
	print_help()
end

