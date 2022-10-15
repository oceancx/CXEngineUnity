cmake_minimum_required (VERSION 3.10)
project (CXEngine LANGUAGES C CXX)

find_package(CXLua)
#find_package(GLEW CONFIG)
#find_package(glfw3)
#find_package(freetype CONFIG)
find_package(IMGui)
#find_package(gsm)
#find_package(miniaudio)
#find_package(nanovg)

file(GLOB_RECURSE  SOURCES_CPP "src/*.cpp" )
file(GLOB_RECURSE  SOURCES_HEADER_PRI "src/*.h"  "src/*.hpp" "src/*.inl")
find_library( OPENGLES3_LIBRARY GLESv2 "OpenGL ES v3.0 library")
set( OPENGLES3_LIBRARY "D:\\Software\\Mali_OpenGL_ES_Emulator-v3.0.4-2-g8d905-Windows-64bit_unsigned\\libGLESv2.lib")
set( EGL_LIBRARY "D:\\Software\\Mali_OpenGL_ES_Emulator-v3.0.4-2-g8d905-Windows-64bit_unsigned\\libEGL.lib")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files"  FILES ${SOURCES_CPP})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files"  FILES ${SOURCES_HEADER_PRI})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../common/src PREFIX "Source Files"  FILES ${GAME_SOURCES_CPP})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/../common/src PREFIX "Source Files"  FILES ${GAME_SOURCES_HEADER})
add_library(CXEngine SHARED
    ${SOURCES_HEADER_PRI}
    src/animation/sprite.cpp
    src/camera.cpp
    src/graphics/shader.h
    src/graphics/shader.cpp
    src/graphics/texture.h
    src/graphics/texture.cpp
    src/graphics/bitmap.h
    src/graphics/bitmap.cpp
    src/sprite_renderer.cpp
    src/sprite_renderer.h
    src/graphics/ui_renderer.cpp
    src/game.cpp
    src/input_manager.cpp
    src/net.cpp
    src/math/vector.cpp    
    src/resource_manager.cpp
    src/sprite_batch.cpp
    src/sprite_renderer.cpp
    src/text_renderer.cpp
    src/texture_manager.cpp
    src/ui.cpp
    src/window.cpp
    src/audio/audio_manager.cpp
    ${GAME_SOURCES_HEADER}
    ${GAME_COMMON_SRCS}
    src/actor/move.cpp
    src/scene/base_scene.cpp
    src/scene/game_map.cpp
    src/scene/scene_manager.cpp
    src/Unity/RenderAPI.cpp
    src/Unity/RenderingPlugin.cpp
    src/Unity/RenderAPI_D3D11.cpp
    src/Unity/RenderAPI_D3D12.cpp
    src/Unity/RenderAPI_OpenGLCoreES.cpp
    src/gl3w/gl3w.c
)

if(MSVC)
    target_compile_options(CXEngine PRIVATE /wd4819 )
#   target_compile_options(CXEngine PRIVATE /W4 /WX /wd4324 /wd4201 /wd4819 /wd4267)
else()
#   target_compile_options(CXEngine PRIVATE -Wall -Wextra -pedantic -Werror)
endif(MSVC)

target_include_directories(CXEngine
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../common/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


target_compile_definitions(CXEngine 
  PRIVATE
    -D_CRT_SECURE_NO_WARNINGS
	-DIMGUI_IMPL_OPENGL_LOADER_GLEW
    -DWORK_DIR="${ENGINE_DIR}"
    -DNODEFAULTLIB
    -DSIMPLE_ENGINE 
    -DWIN32
    -DRENDERINGPLUGIN_EXPORTS
) 
if(WIN32)
target_compile_definitions(CXEngine 
  PRIVATE
    -DZLIB_WINAPI
    -DNOMINMAX
    -DUNICODE
    -D_UNICODE
    -DWIN32_LEAN_AND_MEAN
    -D_WIN32_WINNT=0x0501
)
endif(WIN32)

set_target_properties(CXEngine PROPERTIES COTIRE_CXX_PREFIX_HEADER_INIT "src/pch/precompile.h")
# cotire(CXEngine)

target_link_libraries(CXEngine
  PRIVATE 
    CXLua::CXLua
    opengl32
    IMGui::IMGui
)

add_custom_command(TARGET CXEngine
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:CXEngine>"  ${UNITY_ASSETS_DIR}/Plugins
    COMMAND ${CMAKE_COMMAND} -E echo "copy $<TARGET_FILE:CXEngine> to ${UNITY_ASSETS_DIR}/Plugins"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:lua>"  ${UNITY_ASSETS_DIR}/Plugins
    COMMAND ${CMAKE_COMMAND} -E echo "copy $<TARGET_FILE:lua> to ${UNITY_ASSETS_DIR}/Plugins"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_PDB_FILE:CXEngine>"  ${UNITY_ASSETS_DIR}/Plugins
    COMMAND ${CMAKE_COMMAND} -E echo "copy $<TARGET_PDB_FILE:CXEngine> to ${UNITY_ASSETS_DIR}/Plugins"
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_PDB_FILE:lua>"  ${UNITY_ASSETS_DIR}/Plugins
    COMMAND ${CMAKE_COMMAND} -E echo "copy $<TARGET_PDB_FILE:lua> to ${UNITY_ASSETS_DIR}/Plugins"
    VERBATIM
)
