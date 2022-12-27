# Set project configuration here

set(APP_NAME "CPP Application")
set(APP_DESCRIPTION "CPP Application Template" )
set(APP_VERSION_MAJOR 0)
set(APP_VERSION_MINOR 0)
set(APP_VERSION_RELEASE 1)
set(APP_SEMANTIC_VERSION "${APP_VERSION_MAJOR}.${APP_VERSION_MINOR}.${APP_VERSION_RELEASE}")
set(APP_HOMEPAGE_URL "https://example.org")
set(APP_SHORT "app")
set(APP_LANG_STD cxx_std_17)
set(APP_BUILD_TESTS ON)

# Add external packages, expacted value 3 values per entry: Package_Name GIT_REPO GIT_TAG
set(
    APP_EXTERNAL_PROJECTS 
    glm     "https://github.com/g-truc/glm"                                      0.9.9.8
    spdlog  "https://github.com/gabime/spdlog.git"                               v1.9.0   
    json    "https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent" v3.9.1 
)

set(APP_LIBRARIES "appcore" "appgl" "appwindow")
set(APP_EXECUTABLES "app")
set(APP_EXAMPLES 
    "basic_simple_color_triangle"
    "basic_alpha_blending"
   )

set(APP_SHOW_CMAKE_VARS OFF)