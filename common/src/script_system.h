#pragma once

#include "lua_bind.h"
#include <Unity/IUnityInterface.h>

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API script_system_prepare_init(lua_State* L);

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API script_system_run_main_script(lua_State* L);

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API script_system_init(lua_State* L);
extern "C" bool UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API script_system_update(float t);
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API script_system_draw();
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API script_system_draw_ui();
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API script_system_deinit();
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API script_system_test(lua_State* L);
extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API script_system_test2(void* L);

lua_State* script_system_get_luastate();

any script_system_get_globle(const char* name);

template <class T>
T script_system_get_globle(const char* name)
{
	any v = script_system_get_globle(name);
	return any_cast<T>(v);
}

