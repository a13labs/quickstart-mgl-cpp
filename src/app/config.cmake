target_link_libraries(
    ${MGL_CURRENT_TARGET}
    PRIVATE
    ${MGL_CORE_LIB}
)

target_link_libraries(
    ${MGL_CURRENT_TARGET}
    PRIVATE
    ${MGL_LIB}
)

target_link_libraries(
    ${MGL_CURRENT_TARGET}
    PRIVATE
    ${MGL_WINDOW_LIB}
)
