cmake_minimum_required(VERSION 3.10)
project(Game)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# 启用 ncurses 宽字符支持（UTF-8）
add_definitions(-DNCURSES_WIDECHAR=1)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

add_subdirectory(src)