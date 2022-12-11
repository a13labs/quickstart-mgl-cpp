find_package(SDL2 REQUIRED)

if(CMAKE_COMPILER_IS_GNUCXX)
    find_package(Threads REQUIRED)
endif()

if (UNIX)
   set("OpenGL_GL_PREFERENCE" "GLVND")
endif(UNIX)
find_package(OpenGL REQUIRED)

