# Logging Library setup
# https://github.com/gabime/spdlog.git
target_link_libraries( 
    ${APP_CURRENT_TARGET}
    PUBLIC
    spdlog::spdlog
)

# JSON Library setup
# https://github.com/nlohmann/json.git
target_link_libraries( 
    ${APP_CURRENT_TARGET}
    PUBLIC
    nlohmann_json::nlohmann_json
)

# Export library
set(APPCORE_LIB appcore PARENT_SCOPE)
set(APPCORE_INCLUDE ${APP_CURRENT_SOURCE} PARENT_SCOPE)