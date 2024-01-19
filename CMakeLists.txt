cmake_minimum_required(VERSION 3.0.0)
project(enemy_AI VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(enemy_AI main.cpp)
add_subdirectory(lib/room)
add_subdirectory(lib/matrix)
add_subdirectory(lib/perlin)

target_link_libraries(enemy_AI room)
target_link_libraries(enemy_AI matrix)
target_link_libraries(enemy_AI perlin)
target_include_directories(enemy_AI PUBLIC lib/room lib/matrix lib/perlin)

# -Wall
target_compile_options(enemy_AI PUBLIC -Wall)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
