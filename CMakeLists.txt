project(C_P2P_Chat C)

cmake_minimum_required(VERSION 3.9)
set(CMAKE_C_STANDARD 11)

# Подключаем ncursesw
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses)
include(FindCurses)

set(SOURCE_FILES
        src/main.c
        src/Chat.c
        src/Clients.c
        src/Network/Socket.c
        src/Network/Packet.c
        src/Utils/Utils.c
        src/Utils/Interface.c)

add_executable(C_P2P_Chat ${SOURCE_FILES})
target_link_libraries(C_P2P_Chat ${CURSES_LIBRARIES})
