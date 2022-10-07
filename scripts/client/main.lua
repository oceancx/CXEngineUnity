SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
DefaultMapID = 1001


script_system_dofile('../share/enums.lua')
script_system_dofile('../share/enums_protocol.lua')
script_system_dofile('../share/vfs.lua')
script_system_dofile('../share/utils.lua')
script_system_dofile('../share/content_system.lua')
script_system_dofile('../share/actor_metatable.lua')

script_system_dofile 'actor_metatable.lua'

script_system_dofile('net_manager.lua')

script_system_dofile('../combat/combat_system.lua')
script_system_dofile 'scene_manager.lua'
script_system_dofile 'actor_manager.lua'
script_system_dofile('event_system.lua')
script_system_dofile('input_manager.lua')

script_system_dofile('addon_manager.lua')

script_system_dofile('module/team.lua')
script_system_dofile('module/dialog.lua')
script_system_dofile('module/summon.lua')


SERVER_HOST = command_arg_opt_str('host','127.0.0.1')
SERVER_PORT = command_arg_opt_int('port', 45000)
DBG_PORT = command_arg_opt_int('dbg_port', 9601)
 
local anim = nil
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

    anim = animation_create(WADDONWDF,0xCA8FDEAD)         --0xCA8FDEAD为was在wdf里的hash值,CA8FDEAD可以在res\tables\ini\waddon.wdf.ini找到对应的动画名字
    anim:SetPos(200,100)                --设置动画在屏幕上的位置,屏幕的原点(0,0)在左上角
    anim:SetLoop(0)                     --设置动画的循环次数,0表示无限循环
    anim:Play()        
end

function on_script_system_update()
    -- net_manager_update()
    -- timer_manager_update(window_system_get_dt())
    -- scene_manager_update()
    -- scene_manager_draw()

    if anim then
        anim:Update()                   --在游戏主循环里调用anim的Update函数, Update里面会根据这一帧的delta time还有自己的默认换帧时间去更新当前的帧数
    end
end

function on_script_system_draw()
    if anim then
        anim:Draw()                  
    end
end

function on_script_system_deinit()
    -- net_manager_deinit()
    -- timer_manager_deinit()
    -- input_manager_deinit()
    -- resource_manager_deinit()
    -- scene_manager_deinit()
    -- actor_manager_deinit()
end

