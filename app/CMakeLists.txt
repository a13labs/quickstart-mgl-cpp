project(application_example)

# Add the C source files for the application
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# Use GLOB to find all shader files in the specified directory
file(GLOB_RECURSE VS_SHADER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/shaders/vertex/*.vs")
file(GLOB_RECURSE FS_SHADER_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/shaders/fragment/*.fs")

# Create an empty list to store generated headers
set(SHADER_HEADERS "")

# Create a directory to store generated headers
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/inc/shaders/vertex")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/inc/shaders/fragment")

# Generate header files for each shader using the custom script
foreach(SHADER_SOURCE ${VS_SHADER_SOURCES})
    get_filename_component(SHADER_NAME ${SHADER_SOURCE} NAME_WE)
    set(SHADER_HEADER "${CMAKE_CURRENT_BINARY_DIR}/inc/shaders/vertex/${SHADER_NAME}.hpp")
    generate_shader_header(${SHADER_SOURCE} vert ${SHADER_HEADER})
    list(APPEND MGL_SHADER_HEADERS ${SHADER_HEADER})
endforeach()

# Generate header files for each shader using the custom script
foreach(SHADER_SOURCE ${FS_SHADER_SOURCES})
    get_filename_component(SHADER_NAME ${SHADER_SOURCE} NAME_WE)
    set(SHADER_HEADER "${CMAKE_CURRENT_BINARY_DIR}/inc/shaders/fragment/${SHADER_NAME}.hpp")
    generate_shader_header(${SHADER_SOURCE} frag ${SHADER_HEADER})
    list(APPEND MGL_SHADER_HEADERS ${SHADER_HEADER})
endforeach()

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
