# generate_shader_headers.cmake
# This script will be used to convert GLSL files to C arrays using xxd
function(generate_shader_header INPUT_SHADER SHADER_TYPE OUTPUT_HEADER)
    get_filename_component(FILENAME ${INPUT_SHADER} NAME_WE)
    if (SHADER_TYPE STREQUAL "vert")
        set(VARIABLE_NAME "vertex_shader_source")
    elseif (SHADER_TYPE STREQUAL "frag")
        set(VARIABLE_NAME "fragment_shader_source")
    else()
        message(FATAL_ERROR "Invalid shader type: ${SHADER_TYPE}")
    endif()
    execute_process(
        COMMAND xxd -i -n "${VARIABLE_NAME}" "${INPUT_SHADER}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_FILE ${OUTPUT_HEADER}
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
endfunction()
