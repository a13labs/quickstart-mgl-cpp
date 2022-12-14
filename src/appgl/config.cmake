# # GLAD 
# set(GLAD_DIR "${APP_CURRENT_SOURCE}/glad")
# set(GLAD_INCLUDE_DIR "${GLAD_DIR}/include")

# add_library(glad "${GLAD_DIR}/src/glad.c")

# target_include_directories(
#     glad
#     PUBLIC 
#     "${GLAD_DIR}/include"
# )

# # GLAD Platform specific configuration
# if (UNIX)
#     target_link_libraries(glad ${CMAKE_DL_LIBS})
# endif()

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

# # GLAD Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs.
# # https://glad.dav1d.de/
# target_include_directories( 
#     ${APP_CURRENT_TARGET}
#     PUBLIC
#     "${GLAD_INCLUDE_DIR}"
# )

# target_link_libraries(
#     ${APP_CURRENT_TARGET}
#     PUBLIC
#     glad
# )

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

set(APPGL_LIB appgl PARENT_SCOPE)
set(APPGL_INCLUDE ${APP_CURRENT_SOURCE} PARENT_SCOPE)