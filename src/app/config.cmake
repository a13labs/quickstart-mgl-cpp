target_include_directories( 
    ${APP_CURRENT_TARGET}
    PRIVATE
    ${APPCORE_INCLUDE}
)

target_link_libraries(
    ${APP_CURRENT_TARGET}
    PRIVATE
    ${APPCORE_LIB}
)

# OpenGL Support
# https://www.opengl.org/
target_link_libraries(
    ${APP_CURRENT_TARGET}
    PRIVATE
    OpenGL::OpenGL
)
