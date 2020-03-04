local fs = require("lfs")

function new_child(parent_node, path)
	local attrs = fs.attributes(path)

	local iter, dir
	if attrs.mode == "directory" then
		iter, dir = fs.dir(path)
	end

	return {
		dir = dir,
		path = path,
		iter = iter,
		parent = parent_node,
		attrs = attrs
	}
end

function next_child(node)
	local relative_path
	repeat
		relative_path = node.iter(node.dir)
	until (relative_path ~= "." and relative_path ~= "..")

	if not relative_path then
		return nil
	end

	local path = node.path.."/"..relative_path

	return new_child(node, path)
end

fs.bottomup_tree = function(path)
	local node = new_child(nil, path)
	return function()
		if not node then return nil end

		local child_node = next_child(node)
		while child_node ~= nil and child_node.dir do
			node = child_node
			child_node = next_child(node)
		end

		if not child_node then
			child_node = node
			node = child_node.parent
		end

		return child_node.path, child_node.attrs
	end
end
fs.tree = fs.bottomup_tree

fs.rm = function(path, attrs)
	attrs = attrs or fs.attributes(path)
	if attrs.mode == "directory" then
		return fs.rmdir(path)
	else
		return os.remove(path)
	end
end

fs.rmrecur = function(path)
	local success, error_msg
	for file, attrs in fs.tree(path) do
		success, error_msg = fs.rm(file, attrs)
		if not success then
			return success, error_msg
		end
	end

	return true
end

return fs

