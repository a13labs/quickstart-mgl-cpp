option(MGL_BUILD_TESTS "Enable ModernGL-Cpp unit tests" OFF)
option(MGL_BUILD_DOCS "Build ModernGL-Cpp documentation" OFF)
option(MGL_BUILD_EXAMPLES "Build ModernGL-Cpp documentation" OFF)

# Set the path where you want to download Google Test
set(MGL_INSTALL_DIR "${CMAKE_BINARY_DIR}/mgl" CACHE PATH "Path to mgl installation directory")

# Use FetchContent to download mgl
FetchContent_Declare(
    mgl
    GIT_REPOSITORY https://github.com/a13labs/moderngl-cpp.git
    GIT_TAG        main
)

# FetchContent will download mgl and build it at configure time
FetchContent_MakeAvailable(mgl)

# Add paths to modules
list( APPEND CMAKE_MODULE_PATH "${MGL_BINARY_DIR}/cmake/" )       
