project(application_example)

# Add the C source files for the application
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# generate shader headers
generate_shader_headers()

# Create the executable target "window"
add_executable(${PROJECT_NAME} ${APP_SOURCES})

# Add include directories
include_directories(
    ${PROJECT_NAME} 
  PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_BINARY_DIR}/inc
)

# Link gamelib to the application
target_link_libraries(
    ${PROJECT_NAME} 
  PRIVATE 
    mgl::static
)
