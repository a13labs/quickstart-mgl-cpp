cmake_minimum_required(VERSION 3.7...3.25)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

include(Project.cmake)
include(cmake/ExternalDeps.cmake)

project(
  ${MGL_APP_SHORT}
  VERSION ${MGL_APP_VERSION_MAJOR}.${MGL_APP_VERSION_MINOR}.${MGL_APP_VERSION_RELEASE}
  DESCRIPTION ${MGL_APP_DESCRIPTION}
  HOMEPAGE_URL ${MGL_APP_HOMEPAGE_URL}
  LANGUAGES CXX C
)

add_definitions(-DMGL_APP_SEM_VERSION=${MGL_APP_SEMANTIC_VERSION})
add_definitions(-DMGL_APP_VERSION_MAJOR=${MGL_APP_VERSION_MAJOR})
add_definitions(-DMGL_APP_VERSION_MINOR=${MGL_APP_VERSION_MINOR})
add_definitions(-DMGL_APP_VERSION_RELEASE=${MGL_APP_VERSION_RELEASE})

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
  add_definitions(-DMGL_APP_DEBUG)
endif()

# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
  set(BUILD_ARCH "x64")
else ()
  set(BUILD_ARCH "x86")
endif ()

# Platform specific configuration
if(WIN32)
  add_definitions(-DMGL_APP_PLATFORM_WINDOWS)
elseif (UNIX)
  add_definitions(-DMGL_APP_PLATFORM_LINUX)
endif()

if(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
  # POSIX Threads
  find_package(Threads REQUIRED)
else()
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W3")
endif()

# Specify build paths
set(PROJECT_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/output")
file(MAKE_DIRECTORY ${PROJECT_OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_OUTPUT_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_OUTPUT_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_OUTPUT_DIR}/bin")
set(CMAKE_INSTALL_PREFIX "${PROJECT_OUTPUT_DIR}/dist")

# Activate Unit Testing
if(NOT DEFINED ENV{MGL_APP_NO_TESTS})
  add_definitions(-DMGL_APP_TESTS)
endif()

# Allow config.h to be visible
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Include custom configuration if required
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/config.cmake")
  message(STATUS "Adding custom configuration: config.cmake")
  include("${CMAKE_CURRENT_SOURCE_DIR}/config.cmake")
endif()

while(MGL_APP_EXTERNAL_PROJECTS)
  list(POP_FRONT MGL_APP_EXTERNAL_PROJECTS PRJ_NAME PRJ_GIT_REPO PRJ_GIT_TAG)
  message(STATUS ${PRJ_NAME} ${PRJ_GIT_REPO} ${PRJ_GIT_TAG} )
  Add_External_Project(${PRJ_NAME} ${PRJ_GIT_REPO} ${PRJ_GIT_TAG})
endwhile()

# Application/Libraries Source
foreach(LIB ${MGL_APP_LIBRARIES})
  message(STATUS "Adding library: '${LIB}")
  AddLibrary(${LIB})
endforeach()

foreach(EXECUTABLE ${MGL_APP_EXECUTABLES})
  message(STATUS "Adding executable: '${EXECUTABLE}")
  AddExecutable(${EXECUTABLE})
endforeach()

# Unit Testing
# https://github.com/google/googletest.git

if(DEFINED ENV{MGL_APP_NO_BUILD_TESTS})
  set(MGL_APP_BUILD_TESTS OFF)
endif()

if(MGL_APP_BUILD_TESTS)
  # Prevent overriding the parent project's compiler/linker
  # settings on Windows
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  if(NOT GOOGLE_TEST_PRESENT)
    AddExternalProject(
        googletest
        "https://github.com/google/googletest.git" 
        "release-1.11.0"
    )    
  endif()

  # The gtest/gtest_main targets carry header search path
  # dependencies automatically when using CMake 2.8.11 or
  # later. Otherwise we have to add them here ourselves.
  if (CMAKE_VERSION VERSION_LESS 2.8.11)
      include_directories("${gtest_SOURCE_DIR}/include")
  endif()

  enable_testing()
  include(Dart)

  ListTests(UNITTESTS)

  foreach(UNITTEST ${UNITTESTS})
    AddUnitTest(${UNITTEST})
  endforeach(UNITTEST ${UNITTESTS})

endif() 

# Debug CMake Vars, uncomment to show CMake vars
if(MGL_APP_SHOW_CMAKE_VARS) 
  get_cmake_property(_variableNames VARIABLES)
  list (SORT _variableNames)
  foreach (_variableName ${_variableNames})
      message(STATUS "${_variableName}=${${_variableName}}")
  endforeach()
endif()