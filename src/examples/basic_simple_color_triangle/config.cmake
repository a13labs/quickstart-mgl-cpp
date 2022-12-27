target_link_libraries(
    ${APP_CURRENT_TARGET}
    PRIVATE
    ${MGL_CORE_LIB}
)

target_link_libraries(
    ${APP_CURRENT_TARGET}
    PRIVATE
    ${MGL_LIB}
)

target_link_libraries(
    ${APP_CURRENT_TARGET}
    PRIVATE
    ${MGL_WINDOW_LIB}
)
