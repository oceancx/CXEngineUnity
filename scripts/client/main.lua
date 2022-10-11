SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
DefaultMapID = 1001

function script_system_dofile(relative_path)
    local path = vfs_get_luapath(relative_path)
    print(path)
    local subpaths = {} 
    path = path..'/'
    for subpath in path:gmatch('(.-)[\\/]') do
        if #subpaths == 0 then
            subpath = subpath:lower()
        end
        if subpath == '..' then
            table.remove(subpaths)
        else
            table.insert(subpaths,subpath)
        end
    end
	path = table.concat(subpaths,'\\')
    local err = dofile(path)
    if err then
        print(err)
    end
end


-- script_system_dofile('../share/enums.lua')
-- script_system_dofile('../share/enums_protocol.lua')
-- script_system_dofile('../share/vfs.lua')
-- script_system_dofile('../share/utils.lua')
-- script_system_dofile('../share/content_system.lua')
-- script_system_dofile('../share/actor_metatable.lua')

-- script_system_dofile 'actor_metatable.lua'

-- script_system_dofile('net_manager.lua')

-- script_system_dofile('../combat/combat_system.lua')
-- script_system_dofile 'scene_manager.lua'
-- script_system_dofile 'actor_manager.lua'
-- script_system_dofile('event_system.lua')
-- script_system_dofile('input_manager.lua')

-- script_system_dofile('addon_manager.lua')

-- script_system_dofile('module/team.lua')
-- script_system_dofile('module/dialog.lua')
-- script_system_dofile('module/summon.lua')


-- SERVER_HOST = command_arg_opt_str('host','127.0.0.1')
-- SERVER_PORT = command_arg_opt_int('port', 45000)
-- DBG_PORT = command_arg_opt_int('dbg_port', 9601)
imgui = ImGuiNET.ImGui

script_system_dofile('../share/enums.lua')
script_system_dofile('../share/vfs.lua')
script_system_dofile('../share/utils.lua')
script_system_dofile('../share/content_system.lua')
script_system_dofile('../share/actor_metatable.lua')

local actor
function on_script_system_init()
    cxlog_info('on_game_start')
    -- luadbg_listen(DBG_PORT)
    -- iw_init(SCREEN_WIDTH,SCREEN_HEIGHT)   
    -- iw_set_font(vfs_get_workdir()..'/assets/font/simsun.ttc',14)
    -- content_system_init()
    -- timer_manager_init()
    -- resource_manager_init()
    -- sprite_renderer_init()
	-- combat_system_init()
    -- net_manager_init(SERVER_HOST, SERVER_PORT)
    -- load_all_addons()
    content_system_init()           --这个函数在content_system.lua里面定义，会加载各种数据表(tables/*.tsv)，游戏内容基本上是由表来定义的
    actor = actor_manager_create_actor(5)     --lua_create_actor的参数是actor的id，这里就随便填了个1
    actor:ResetASM()                --ResetASM是重新设置actor的动作状态机，目前需要这么手动调用一下
    actor:SetPos(500, 400)                  --设置玩家的世界坐标
    actor_manager_set_local_player(actor:GetID())
end

function on_script_system_update()
    -- net_manager_update()
    -- timer_manager_update(window_system_get_dt())
    -- scene_manager_update()
    -- scene_manager_draw()
    if not actor then return end
    actor:Update()

    if imgui.IsMouseReleased(0) then
        actor:MoveTo(Game.mouseX, Game.mouseY) 
    end
end
 
function on_script_system_deinit()
    -- net_manager_deinit()
    -- timer_manager_deinit()
    -- input_manager_deinit()
    -- resource_manager_deinit()
    -- scene_manager_deinit()
    -- actor_manager_deinit()
    if not actor then return end
    actor_manager_destroy_actor(actor:GetID())
end

function on_script_system_draw_ui()
    -- if imgui.Button("w") then                   --imgui.Button返回true表示按钮被点击
    --     local x, y = anim:GetPos()
    --     anim:SetPos(x, y - 5)
    -- end
end
