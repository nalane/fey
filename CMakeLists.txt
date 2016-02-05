cmake_minimum_required(VERSION 3.2.2)
project(opal)

file(GLOB SOURCES "src/*.c")
add_executable(opal ${SOURCES})
target_link_libraries(opal glfw GLEW)

set(GCC_COMPILE_FLAGS "-O3")
add_definitions(${GCC_COMPILE_FLAGS})

file(COPY config DESTINATION opalData)
install(TARGETS opal DESTINATION bin)
install(DIRECTORY config/ DESTINATION bin/opalData/config)