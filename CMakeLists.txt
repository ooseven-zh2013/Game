cmake_minimum_required(VERSION 3.10)
project(Game)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB SOURCES
    "src/*.cpp"
    "src/common/*.cpp"
    "src/common/display/*.cpp"
    "src/common/game/*.cpp"
    "src/common/game/basic/*.cpp"
    "src/snake/*.cpp"
    "src/snake/role/*.cpp"
)

add_subdirectory(src)