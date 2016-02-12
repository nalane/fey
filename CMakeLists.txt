cmake_minimum_required(VERSION 3.2.2)
project(opal)

file(GLOB SOURCES "src/*.c")
add_executable(opal ${SOURCES})

set(LINKED_LIBRARIES glfw GLEW GL)
if(UNIX)
  set(LINKED_LIBRARIES ${LINKED_LIBRARIES} m)
endif()
target_link_libraries(opal glfw ${LINKED_LIBRARIES})

set(GCC_COMPILE_FLAGS "-O3 -g")
add_definitions(${GCC_COMPILE_FLAGS})

file(COPY opalData DESTINATION .)
install(TARGETS opal DESTINATION games)
install(DIRECTORY opalData DESTINATION games DIRECTORY_PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE)