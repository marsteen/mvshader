cmake_minimum_required(VERSION 3.10)
project("mvshader")

include_directories(./source/include  ./source)


message("System: ${CMAKE_SYSTEM_NAME}")

#find_package(PkgConfig REQUIRED QUIET)
#pkg_search_module(
#    OpenGL
#    REQUIRED
#      OpenGL
#    IMPORTED_TARGET
#    )


#
# OPENGL
#
#find_package(OpenGL REQUIRED COMPONENTS OpenGL)
#if (OPENGL_FOUND)
#    message("found OPENGL")
#    include_directories($(OPENGL_INCLUDE_DIRS))
#    link_libraries(${OPENGL_LIBRARIES})
#endif()

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
#find_package(SDL2 REQUIRED)
#if (SDL2_FOUND)
#    message("found SDL2")
#    include_directories($(SDL2_INCLUDE_DIRS))
#    link_libraries(${SDL2_LIBRARIES})
#endif()

include_directories(/Library/Frameworks/SDL2.framework/Versions/A/Headers /usr/local/include)
link_directories(
    /Library/Frameworks/SDL2.framework/Versions/A/Headers /usr/local/include
    /usr/local/lib
    ./lib-extern/mac
    )



add_executable(mvshader
    source/mainSdl.cpp
    source/CSDLblurline.cpp
    source/CContextSphere.cpp
    source/lib/CSdlApp.cpp
    source/lib/NStringTool.cpp
    source/lib/CFileIO.cpp
    source/lib/NGlobalLog-Desktop.cpp
    source/lib/CGL_Context.cpp
    source/lib/CGL_Shader.cpp
    source/lib/CGL_Shader_Sphere.cpp
    source/lib/CGL_Basic.cpp
    source/lib/CGL_Texture.cpp
    source/lib/CFrameBuffer.cpp
    source/lib/CDataRect.cpp
    source/lib/CPixel24.cpp
    source/lib/CPixel32.cpp
    source/lib/CBlockOperations.cpp
    source/lib/shader/shader_sphere_vert.cpp
    source/lib/shader/shader_sphere_frag.cpp)


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
    target_link_libraries(mvshader
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework Foundation"
        "-framework CoreAudio"
        "-framework Carbon"
        "-framework ForceFeedback"
        "-framework AudioUnit"
        SDL2
        GLEW
        iconv
        )
endif()


