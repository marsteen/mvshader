cmake_minimum_required(VERSION 3.10)
project("mvshader")

include_directories(./include  ./source)


message("System: ${CMAKE_SYSTEM_NAME}")

#
# GLEW
#
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message("found GLEW")
    include_directories($(GLEW_INCLUDE_DIRS))
    link_libraries(${GLEW_LIBRARIES})
endif()

#
# SDL2
#
find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
    message("found SDL2")
    include_directories($(SDL2_INCLUDE_DIRS))
    link_libraries(${SDL2_LIBRARIES})
endif()

#
# OPENGL
#
#find_package(OPENGL REQUIRED)
find_package(OpenGL REQUIRED COMPONENTS OpenGL)
if (OPENGL_FOUND)
    message("found OPENGL")
    include_directories($(OPENGL_INCLUDE_DIRS))
    link_libraries(${OPENGL_LIBRARIES})
endif()


add_executable(mvshader
    source/mainSdl.cpp
    source/CSDLblurline.cpp
    source/CContextSphere.cpp
    lib/CSdlApp.cpp
    lib/NStringTool.cpp
    lib/CFileIO.cpp
    lib/NGlobalLog-Desktop.cpp
    lib/CGL_Context.cpp
    lib/CGL_Shader.cpp
    lib/CGL_Shader_Sphere.cpp
    lib/CGL_Basic.cpp
    lib/CGL_Texture.cpp
    lib/CFrameBuffer.cpp
    lib/CDataRect.cpp
    lib/NUtf.cpp
    lib/CPixel24.cpp
    lib/CPixel32.cpp
    lib/CBlockOperations.cpp
    lib/shader/shader_sphere_vert.cpp
    lib/shader/shader_sphere_frag.cpp)


if (CMAKE_SYSTEM_NAME STREQUAL "MSYS")
    #
    # Windows
    #
    target_link_libraries(mvshader
        opengl32)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    #
    # Linux
    #
    target_link_libraries(mvshader
        SDL2 
        GL
        GLEW)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    #
    # Mac OS
    #
    # *TODO*
endif()


