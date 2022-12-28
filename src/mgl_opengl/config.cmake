if(WIN32)
    add_definitions(-DMGL_OPENGL_WGL)
elseif (UNIX)
    if (OpenGL_EGL_FOUND)
        add_definitions(-DMGL_OPENGL_EGL)
    endif()
    if (OpenGL_GLX_FOUND)
        add_definitions(-DMGL_OPENGL_GLX)
    endif()
endif()

# GLM Library setup
# https://github.com/g-truc/glm
target_link_libraries( 
    ${MGL_CURRENT_TARGET}
    PUBLIC
    glm::glm
)

# OpenGL Support
# https://www.opengl.org/
target_link_libraries(
    ${MGL_CURRENT_TARGET}
    PUBLIC
    OpenGL::OpenGL
)

# AppCore
target_link_libraries(
    ${MGL_CURRENT_TARGET}
    PRIVATE
    mgl_core::mgl_core
)

# Export library
set(MGL_OPENGL_LIB mgl_opengl PARENT_SCOPE)
set(MGL_OPENGL_INCLUDE ${MGL_CURRENT_SOURCE} PARENT_SCOPE)