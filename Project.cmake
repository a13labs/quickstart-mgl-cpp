# Set project configuration here

set(MGL_APP_NAME "ModernGL CPP Application")
set(MGL_APP_DESCRIPTION "ModernGL CPP Application Template" )
set(MGL_APP_VERSION_MAJOR 0)
set(MGL_APP_VERSION_MINOR 0)
set(MGL_APP_VERSION_RELEASE 1)
set(MGL_APP_SEMANTIC_VERSION "${MGL_APP_VERSION_MAJOR}.${MGL_APP_VERSION_MINOR}.${MGL_APP_VERSION_RELEASE}")
set(MGL_APP_HOMEPAGE_URL "https://example.org")
set(MGL_APP_SHORT "app")
set(MGL_APP_LANG_STD cxx_std_17)
set(MGL_APP_BUILD_TESTS ON)

# Add external packages, expacted value 3 values per entry: Package_Name GIT_REPO GIT_TAG
set(
    MGL_APP_EXTERNAL_PROJECTS 
    mgl     "https://github.com/a13labs/moderngl-cpp.git"              main
)

set(MGL_APP_EXECUTABLES "app")

set(MGL_APP_SHOW_CMAKE_VARS ON)
