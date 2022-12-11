cmake_minimum_required(VERSION @PRJ_CMAKE_VERSION@)

project(@PRJ_NAME@ NONE)

include(ExternalProject)

ExternalProject_Add(
  @PRJ_NAME@
  GIT_REPOSITORY    "@GIT_REPOSITORY@"
  GIT_TAG           "@GIT_TAG@"
  SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/@PRJ_NAME@-src"
  BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/@PRJ_NAME@-build"
  CONFIGURE_COMMAND "@PRJ_CONFIGURE_COMMAND@"
  BUILD_COMMAND     "@PRJ_BUILD_COMMAND@"
  INSTALL_COMMAND   "@PRJ_INSTALL_COMMAND@"
  TEST_COMMAND      "@PRJ_TEST_COMMAND@"
)