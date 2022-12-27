# Logging Library setup
# https://github.com/gabime/spdlog.git
target_link_libraries( 
    ${MGL_CURRENT_TARGET}
    PUBLIC
    spdlog::spdlog
)

# JSON Library setup
# https://github.com/nlohmann/json.git
target_link_libraries( 
    ${MGL_CURRENT_TARGET}
    PUBLIC
    nlohmann_json::nlohmann_json
)

# Export library
set(MGL_CORE_LIB mgl_core PARENT_SCOPE)
set(MGL_CORE_INCLUDE ${MGL_CURRENT_SOURCE} PARENT_SCOPE)