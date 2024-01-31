add_library(
    perlin
    ./PerlinNoise.h
    ./PerlinNoise.cpp
)

target_include_directories(
    perlin
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)