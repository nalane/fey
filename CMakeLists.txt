cmake_minimum_required(VERSION 3.7)
project(fey)

include_directories(deps/glad)
include_directories(deps/glfw/include)
include_directories(deps/glm)
include_directories(deps/soil/inc)
include_directories(deps/stb)
add_subdirectory(deps)

find_package(Vulkan REQUIRED)

include_directories(${Vulkan_INCLUDE_DIR})
include_directories(inc)
include_directories(inc/vertexLayouts)
include_directories(inc/uniformLayouts)
include_directories(inc/controllers)
include_directories(inc/resources)
include_directories(inc/resources/baseResources)
include_directories(inc/resources/opengl)
include_directories(inc/resources/vulkan)
include_directories(inc/fixtures)
include_directories(inc/physics)
include_directories(inc/physics/colliders)
include_directories(inc/graphicsEngines)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(fey ${SOURCES})

set(LINKED_LIBRARIES glfw glad soil ${GLFW_LIBRARIES} ${Vulkan_LIBRARY})
set(DATA_DESTINATION feyData)
set(COPY_DATA feyData)

if(WIN32)
  set(LINKED_LIBRARIES ${LINKED_LIBRARIES} opengl32)
  set(DATA_DESTINATION Debug/feyData)
else()
  set(GCC_COMPILE_FLAGS "-g -std=c++11")
  add_definitions(${GCC_COMPILE_FLAGS})
  if(APPLE)
    find_library(OPENGL_LOCATION OpenGL)
    set(LINKED_LIBRARIES ${LINKED_LIBRARIES} ${OPENGL_LOCATION})
  elseif(UNIX)
    set(LINKED_LIBRARIES ${LINKED_LIBRARIES} GL)
  endif()
endif()
target_link_libraries(fey ${LINKED_LIBRARIES})

add_custom_command(TARGET fey POST_BUILD COMMAND perl ${CMAKE_CURRENT_SOURCE_DIR}/scripts/copyData.pl ${CMAKE_CURRENT_SOURCE_DIR}/${COPY_DATA} ${DATA_DESTINATION})
install(TARGETS fey DESTINATION games)
install(DIRECTORY feyData DESTINATION games DIRECTORY_PERMISSIONS WORLD_READ WORLD_WRITE WORLD_EXECUTE)
