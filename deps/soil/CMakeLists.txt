include_directories(inc)
file(GLOB SOURCES "src/*.c")
add_library (soil STATIC ${SOURCES})
