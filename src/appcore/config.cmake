# GLAD 
set(GLAD_DIR "${APP_CURRENT_SOURCE}/glad")
set(GLAD_INCLUDE_DIR "${GLAD_DIR}/include")

add_library(glad "${GLAD_DIR}/src/glad.c")

target_include_directories(
    glad
    PUBLIC 
    "${GLAD_DIR}/include"
)

# GLAD Platform specific configuration
if (UNIX)
    target_link_libraries(glad ${CMAKE_DL_LIBS})
endif()

# Logging Library setup
# https://github.com/gabime/spdlog.git
target_link_libraries( 
    ${APP_CURRENT_TARGET}
    PUBLIC
    spdlog::spdlog
)

# GLM Library setup
# https://github.com/g-truc/glm
target_include_directories( 
    ${APP_CURRENT_TARGET}
    PUBLIC
    "${GLM_DIR}"
)

# JSON Library setup
# https://github.com/nlohmann/json.git
target_link_libraries( 
    ${APP_CURRENT_TARGET}
    PUBLIC
    nlohmann_json::nlohmann_json
)

# GLAD Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs.
# https://glad.dav1d.de/
target_include_directories( 
    ${APP_CURRENT_TARGET}
    PUBLIC
    "${GLAD_INCLUDE_DIR}"
)
target_link_libraries(
    ${APP_CURRENT_TARGET}
    PUBLIC
    glad
)

# SDL - Simple DirectMedia Layer
# https://www.libsdl.org/
target_link_libraries( 
    ${APP_CURRENT_TARGET}
    PUBLIC
    SDL2::SDL2main SDL2::SDL2
)

set(APPCORE_LIB appcore PARENT_SCOPE)
set(APPCORE_INCLUDE ${APP_CURRENT_SOURCE} PARENT_SCOPE)