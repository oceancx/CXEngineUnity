cmake_minimum_required (VERSION 3.10)
project (CXEngineUnity)
include(cmake/HelperMacroFunction.cmake)
include(cmake/cotire.cmake)
include(cmake/FindOpenGLES3.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
list(INSERT CMAKE_PREFIX_PATH 0  ${CMAKE_SOURCE_DIR}/NativeProject)

add_subdirectory(NativeProject)

set(ENGINE_DIR ${PROJECT_SOURCE_DIR})
set(UNITY_ASSETS_DIR ${PROJECT_SOURCE_DIR}/UnityProject/Assets)
add_subdirectory(common)
add_subdirectory(client)
# add_subdirectory(server)
# add_subdirectory(cxluax)


