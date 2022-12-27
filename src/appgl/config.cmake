if(WIN32)
    add_definitions(-DAPPGL_WGL)
elseif (UNIX)
    if (OpenGL_EGL_FOUND)
        add_definitions(-DAPPGL_EGL)
    endif()
    if (OpenGL_GLX_FOUND)
        add_definitions(-DAPPGL_GLX)
    endif()
endif()

# GLM Library setup
# https://github.com/g-truc/glm
target_include_directories( 
    ${APP_CURRENT_TARGET}
    PUBLIC
    "${GLM_DIR}"
)

# OpenGL Support
# https://www.opengl.org/
target_link_libraries(
    ${APP_CURRENT_TARGET}
    PUBLIC
    OpenGL::OpenGL
)

# AppCore
target_link_libraries(
    ${APP_CURRENT_TARGET}
    PRIVATE
    ${APPCORE_LIB}
)

# Export library
set(APPGL_LIB appgl PARENT_SCOPE)
set(APPGL_INCLUDE ${APP_CURRENT_SOURCE} PARENT_SCOPE)