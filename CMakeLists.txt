cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)

file(GLOB game_SRC
    "src/*.h"
    "src/*.cpp"
    "src/util/*.cpp"
)

add_executable(game ${game_SRC})
target_link_libraries(game SDL2 GLEW GL)
