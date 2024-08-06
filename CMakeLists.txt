cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project (Snake_Game)

add_subdirectory(3rd_Party/cpp-terminal)

add_executable(Snake_Game
Snake_Game/Snake_Game.cpp
Snake_Game/Controller.cpp
Snake_Game/Field.cpp
Snake_Game/Snake.cpp
Snake_Game/Strategy.cpp
Snake_Game/View.cpp
)

target_link_libraries(Snake_Game cpp-terminal)

target_include_directories(Snake_Game PRIVATE 3rd_Party/cpp-terminal)

set_target_properties(Snake_Game PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)