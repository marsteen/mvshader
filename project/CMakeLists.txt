cmake_minimum_required(VERSION 3.10)
project("mvshader")

include_directories(./include  ./source)

#find_package(OpenGL REQUIRED)
#find_package(GLEW REQUIRED)

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
    
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_link_libraries(mvshader
      SDL2 
      opengl32
      glew32)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_link_libraries(mvshader
      SDL2 
      GL
      GLEW)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
# *TODO*
endif()


