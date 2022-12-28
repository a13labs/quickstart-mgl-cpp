function(ListTests RESULT)
  # Get all available tests from MGL_TESTS_FOLDER (tests_* folders)
  set(MGL_TESTS_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/tests" )
  set(UNITTESTS "")
  message(STATUS "Searching for tests in '${MGL_TESTS_FOLDER}'")
  file(GLOB CHILDREN RELATIVE ${MGL_TESTS_FOLDER} ${MGL_TESTS_FOLDER}/test\_*)
  foreach(child ${CHILDREN})
    if(IS_DIRECTORY ${MGL_TESTS_FOLDER}/${child})
      list(APPEND UNITTESTS ${child})
    endif()
  endforeach()
  set(${RESULT} ${UNITTESTS} PARENT_SCOPE)
endfunction()

function(AddUnitTest UNITTEST)
  # Add a unit test the project

  set(MGL_CURRENT_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/tests/${UNITTEST}" )
  set(MGL_CURRENT_TARGET ${UNITTEST})

  message(STATUS "MGL_CURRENT_SOURCE: ${MGL_CURRENT_SOURCE}")
  message(STATUS "MGL_CURRENT_TARGET: ${MGL_CURRENT_TARGET}")

  if(NOT IS_DIRECTORY "${MGL_CURRENT_SOURCE}")
    message(FATAL_ERROR "Source folder does not exists '${MGL_CURRENT_SOURCE}'")
  endif()

  message(STATUS "Adding test '${UNITTEST}'")
  file(GLOB_RECURSE TEST_SRCS "${MGL_CURRENT_SOURCE}/*.cpp")
  file(GLOB_RECURSE TEST_HDRS "${MGL_CURRENT_SOURCE}/*.hpp")
  
  add_executable(
    ${UNITTEST} 
    ${TEST_SRCS}
    ${TEST_HDRS} 
  )
  
  # Unit Testing
  # https://github.com/google/googletest.git
  target_link_libraries(
    ${UNITTEST}
    PUBLIC 
    gtest_main
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

  add_test(
    NAME  "${UNITTEST}"
    COMMAND "${UNITTEST}"
  ) 

endfunction()
