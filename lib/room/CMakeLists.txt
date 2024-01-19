add_library(
    room
    ./Room.h
    ./Room.cpp
)

target_link_libraries(room matrix)
target_include_directories(room PUBLIC lib/matrix)

target_include_directories(
    room
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)