function(AddExecutable TGT_NAME)

  set(
    oneValueArgs 
    CPP_STD 
  )

  cmake_parse_arguments(CALL_ARGS "" "${oneValueArgs}" "" ${ARGN})

  if (${CALL_ARGS_CPP_STD})
      set(PRG_CPP_STD ${CALL_ARGS_CPP_STD})
  else()
      set(CPP_STD ${MGL_LANG_STD})    
  endif()

  set(MGL_CURRENT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/${TGT_NAME}" )
  set(MGL_CURRENT_TARGET ${TGT_NAME})

  message(STATUS "MGL_CURRENT_SOURCE: ${MGL_CURRENT_SOURCE}")
  message(STATUS "MGL_CURRENT_TARGET: ${MGL_CURRENT_TARGET}")

  if(NOT IS_DIRECTORY "${MGL_CURRENT_SOURCE}")
    message(FATAL_ERROR "Source folder does not exists '${MGL_CURRENT_SOURCE}'")
  endif()

  message(STATUS "Adding executable: '${MGL_CURRENT_TARGET}' from '${MGL_CURRENT_SOURCE}'")
  # Add all source files recursively
  FILE(GLOB_RECURSE PRG_SRCS "${MGL_CURRENT_SOURCE}/*.cpp")
  FILE(GLOB_RECURSE PRG_HDRS "${MGL_CURRENT_SOURCE}/*.hpp")

  add_executable(${MGL_CURRENT_TARGET} ${PRG_SRCS} ${PRG_HDRS})

  target_compile_features(
    ${MGL_CURRENT_TARGET} 
    PUBLIC ${MGL_CPP_STD}
  )

  target_include_directories( 
    ${MGL_CURRENT_TARGET}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

  # Include custom configuration if required
  if(EXISTS "${MGL_CURRENT_SOURCE}/config.cmake")
    message(STATUS "Adding custom configuration: ${MGL_CURRENT_SOURCE}/config.cmake")
    include("${MGL_CURRENT_SOURCE}/config.cmake")
  endif()

  install(
    TARGETS ${MGL_CURRENT_TARGET}
    RUNTIME 
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    CONFIGURATIONS All
  )

endfunction()

function(AddLibrary TGT_NAME)

  set(
    oneValueArgs 
    CPP_STD 
  )

  cmake_parse_arguments(CALL_ARGS "" "${oneValueArgs}" "" ${ARGN})

  if (${CALL_ARGS_CPP_STD})
      set(PRG_CPP_STD ${CALL_ARGS_CPP_STD})
  else()
      set(CPP_STD ${MGL_LANG_STD})    
  endif()

  set(MGL_CURRENT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/${TGT_NAME}" )
  set(MGL_CURRENT_TARGET ${TGT_NAME})

  message(STATUS "MGL_CURRENT_SOURCE: ${MGL_CURRENT_SOURCE}")
  message(STATUS "MGL_CURRENT_TARGET: ${MGL_CURRENT_TARGET}")

  if(NOT IS_DIRECTORY "${MGL_CURRENT_SOURCE}")
    message(FATAL_ERROR "Source folder does not exists '${MGL_CURRENT_SOURCE}'")
  endif()

  message(STATUS "Adding library: '${MGL_CURRENT_TARGET}' from '${MGL_CURRENT_SOURCE}'")
  
  # Add all source files recursively
  FILE(GLOB_RECURSE PRG_SRCS "${MGL_CURRENT_SOURCE}/*.cpp")
  FILE(GLOB_RECURSE PRG_HDRS "${MGL_CURRENT_SOURCE}/*.hpp")

  add_library(${MGL_CURRENT_TARGET} ${PRG_SRCS} ${PRG_HDRS})
  add_library("${MGL_CURRENT_TARGET}::${MGL_CURRENT_TARGET}" ALIAS ${MGL_CURRENT_TARGET})
  target_compile_features(
    ${MGL_CURRENT_TARGET} 
    PUBLIC ${MGL_CPP_STD}
  )

  target_include_directories( 
    ${MGL_CURRENT_TARGET}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

  # Include custom configuration if required
  if(EXISTS "${MGL_CURRENT_SOURCE}/config.cmake")
    message(STATUS "Adding custom configuration: ${MGL_CURRENT_SOURCE}/config.cmake")
    include("${MGL_CURRENT_SOURCE}/config.cmake")
  endif()

  install(
    TARGETS ${MGL_CURRENT_TARGET}
    RUNTIME 
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    CONFIGURATIONS All
  )

endfunction()


function(AddExample TGT_NAME)

  set(
    oneValueArgs 
    CPP_STD 
  )

  cmake_parse_arguments(CALL_ARGS "" "${oneValueArgs}" "" ${ARGN})

  if (${CALL_ARGS_CPP_STD})
      set(PRG_CPP_STD ${CALL_ARGS_CPP_STD})
  else()
      set(CPP_STD ${MGL_LANG_STD})    
  endif()

  set(MGL_CURRENT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/examples/${TGT_NAME}" )
  set(MGL_CURRENT_TARGET ${TGT_NAME})

  message(STATUS "MGL_CURRENT_SOURCE: ${MGL_CURRENT_SOURCE}")
  message(STATUS "MGL_CURRENT_TARGET: ${MGL_CURRENT_TARGET}")

  if(NOT IS_DIRECTORY "${MGL_CURRENT_SOURCE}")
    message(FATAL_ERROR "Source folder does not exists '${MGL_CURRENT_SOURCE}'")
  endif()

  message(STATUS "Adding executable: '${MGL_CURRENT_TARGET}' from '${MGL_CURRENT_SOURCE}'")
  # Add all source files recursively
  FILE(GLOB_RECURSE PRG_SRCS "${MGL_CURRENT_SOURCE}/*.cpp")
  FILE(GLOB_RECURSE PRG_HDRS "${MGL_CURRENT_SOURCE}/*.hpp")

  add_executable(${MGL_CURRENT_TARGET} ${PRG_SRCS} ${PRG_HDRS})

  target_compile_features(
    ${MGL_CURRENT_TARGET} 
    PUBLIC ${MGL_CPP_STD}
  )

  target_include_directories( 
    ${MGL_CURRENT_TARGET}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

  # Include custom configuration if required
  if(EXISTS "${MGL_CURRENT_SOURCE}/config.cmake")
    message(STATUS "Adding custom configuration: ${MGL_CURRENT_SOURCE}/config.cmake")
    include("${MGL_CURRENT_SOURCE}/config.cmake")
  endif()

  install(
    TARGETS ${MGL_CURRENT_TARGET}
    RUNTIME 
    DESTINATION ${CMAKE_INSTALL_PREFIX}
    CONFIGURATIONS All
  )

endfunction()
