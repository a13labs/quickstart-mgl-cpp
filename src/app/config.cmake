target_link_libraries(
    ${MGL_CURRENT_TARGET}
    mgl_core::mgl_core
    mgl_opengl::mgl_opengl
    mgl_window::mgl_window
)

include_directories(
    ${MGL_CURRENT_TARGET}
    mgl_core
    mgl_opengl
    mgl_window
)