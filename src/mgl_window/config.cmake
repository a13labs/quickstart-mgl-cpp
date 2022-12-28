# SDL - Simple DirectMedia Layer
# https://www.libsdl.org/
target_link_libraries( 
    ${MGL_CURRENT_TARGET}
    PUBLIC
    SDL2::SDL2main SDL2::SDL2
)

# AppCore
target_link_libraries(
    ${MGL_CURRENT_TARGET}
    PRIVATE
    mgl_core::mgl_core
)

# mgl_opengl
target_link_libraries(
    ${MGL_CURRENT_TARGET}
    PRIVATE
    mgl_opengl::mgl_opengl
)

# Export library
set(MGL_WINDOW_LIB mgl_window PARENT_SCOPE)
set(MGL_WINDOW_INCLUDE ${MGL_CURRENT_SOURCE} PARENT_SCOPE)