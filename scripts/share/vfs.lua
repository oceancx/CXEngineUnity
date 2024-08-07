local work_dir = vfs_get_workdir()
function vfs_getpath()
	return work_dir
end
function vfs_makepath(rpath)
	return work_dir..rpath
end
function vfs_formatpath(path)
	return string.gsub(path,'\\','/')
end
function vfs_get_tsvpath(name)
	return work_dir..'assets/tables/'..name..'.tsv'
end
function vfs_get_tablepath(name)
	return work_dir..'assets/tables/'..name
end
function vfs_get_abspath(path)
	return work_dir..path
end
function vfs_get_resourcepath(path)
	return work_dir..'assets/'..path
end
function vfs_get_assetspath(path)
	return work_dir..'assets/assets/'..path
end
function vfs_get_shaderpath(path)
	return work_dir..'assets/shader'..path
end


function vfs_get_wdfpath(path)
	return work_dir..'assets/data/' ..path
end
function vfs_get_mappath(path)
	return work_dir..'assets/data/scene/'..path..'.map'
end
function vfs_get_fontpath(path)
	return work_dir..'assets/font/'..path
end
function vfs_get_iconpath(path)
	return work_dir..'assets/icon/'..path
end

function IsServer()
    return SIMPLE_SERVER and true or false
end

function IsClient()
    return SIMPLE_SERVER and false or true
end