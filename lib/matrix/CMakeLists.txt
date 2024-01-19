add_library(
    matrix
    ./Matrix.h
    ./Matrix.cpp
)

target_link_libraries(matrix perlin)
target_include_directories(matrix PUBLIC lib/perlin)

target_include_directories(
    matrix
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)