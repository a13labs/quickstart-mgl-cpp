cmake_minimum_required(VERSION 3.21)
project(app
        VERSION 0.0.1
        LANGUAGES CXX
       )

# Enable the FetchContent module
include(FetchContent)

# Build options
option(APP_BUILD_TESTS "Enable Application unit tests" ON)
option(APP_BUILD_DOCS "Build Application documentation" OFF)

# Set the C23 standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Detect the compiler and set APP_COMPILER_DEFINITION
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(APP_COMPILER_DEFINITION "-DAPP_COMPILER_GNU")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(APP_COMPILER_DEFINITION "-DAPP_COMPILER_MSVC")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(APP_COMPILER_DEFINITION "-DAPP_COMPILER_CLANG")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    set(APP_COMPILER_DEFINITION "-DAPP_COMPILER_APPLECLANG")
else()
    message(FATAL_ERROR "Unsupported compiler! ${CMAKE_CXX_COMPILER_ID}" )
endif()

if (CMAKE_BUILD_TYPE MATCHES "^[Rr]elease")
    set(APP_BUILD_TYPE_DEFINITIONS "-DAPP_RELEASE")
    set(APP_BUILD_DOCS ON)
else()
    set(APP_BUILD_TYPE_DEFINITIONS "-DAPP_DEBUG")
endif()

set(APP_LINUX_OS FALSE)
set(APP_WINDOWS_OS FALSE)
set(APP_OSX_OS FALSE)

# Detect the operating system and set APP_OS_DEFINITION
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    message(STATUS "Platform: Linux detected")
    set(APP_LINUX_OS TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    message(STATUS "Platform: Windows detected")
    set(APP_WINDOWS_OS TRUE)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    message(STATUS "Platform: MacOS detected")
    set(APP_OSX_OS TRUE)
else()
    message(FATAL_ERROR "Unsupported operating system!")
endif()

# Configure the version.hpp file using version.hpp
configure_file(version.hpp.in ${CMAKE_BINARY_DIR}/inc/version.hpp)
                    
if (APP_BUILD_TESTS)
    # TODO: Setup testing on windows and macOS
    # Add unit tests
    message(STATUS "Application tests: ON")
    enable_testing()
else()
    message(STATUS "Application tests: OFF")
endif()

if (APP_BUILD_DOCS)
    message(STATUS "Application documentation: ON")
    find_package(Doxygen REQUIRED)
    find_package(Doxygen)
    if (DOXYGEN_FOUND)
        # set input and output files
        set(DOXYGEN_IN "${CMAKE_SOURCE_DIR}/doc/Doxyfile.in")
        set(DOXYGEN_OUT "${CMAKE_BINARY_DIR}/Doxyfile")

        # request to configure the file
        configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
        message("Doxygen build started")

        # note the option ALL which allows to build the docs together with the application
        add_custom_target(gamelib_documentation ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM 
            )
    else (DOXYGEN_FOUND)
        message("Doxygen need to be installed to generate the doxygen documentation")
    endif (DOXYGEN_FOUND)
else()
    message(STATUS "Application documentation: OFF")
endif()

# # Include CMakeLists.txt files for the targets
add_subdirectory(vendors)
add_subdirectory(app)
