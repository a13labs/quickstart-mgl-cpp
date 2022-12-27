# SDL - Simple DirectMedia Layer
# https://www.libsdl.org/
target_link_libraries( 
    ${APP_CURRENT_TARGET}
    PUBLIC
    SDL2::SDL2main SDL2::SDL2
)

# AppCore
target_link_libraries(
    ${APP_CURRENT_TARGET}
    PRIVATE
    ${APPCORE_LIB}
)

# AppGL
target_link_libraries(
    ${APP_CURRENT_TARGET}
    PRIVATE
    ${APPGL_LIB}
)

# Export library
set(APPWINDOW_LIB appwindow PARENT_SCOPE)
set(APPWINDOW_INCLUDE ${APP_CURRENT_SOURCE} PARENT_SCOPE)