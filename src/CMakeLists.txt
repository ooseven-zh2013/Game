add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ../include)

# 优先链接 ncursesw（支持 UTF-8），如果不存在则回退到普通 ncurses
find_library(NCURSESW_LIBRARY ncursesw)
if(NCURSESW_LIBRARY)
    target_link_libraries(${PROJECT_NAME} ${NCURSESW_LIBRARY})
    message(STATUS "Using ncursesw for UTF-8 support")
else()
    target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
    message(WARNING "ncursesw not found, UTF-8 may not work properly")
endif()