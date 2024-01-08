project(application_example)

# Add the C source files for the application
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# generate shader headers
generate_shader_headers()

# Create the executable target "window"
add_executable(application_example ${APP_SOURCES})

set (DEFAULT_INCLUDE_DIRS 
    ${MGL_CORE_INCLUDE_DIR}
    ${MGL_OPENGL_INCLUDE_DIR}
    ${MGL_WINDOW_INCLUDE_DIR}
    ${MGL_GRAPHICS_INCLUDE_DIR}
    ${MGL_APPLICATION_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    ${IMGUI_INCLUDE_DIR}
    ${VENDORS_HEADERS_ONLY_INC_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_BINARY_DIR}/inc
)

# Add include directories
include_directories(application_example PRIVATE ${DEFAULT_INCLUDE_DIRS})

# Link gamelib to the application
target_link_libraries(application_example PRIVATE mgl::application-static)
