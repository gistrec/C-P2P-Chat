cmake_minimum_required(VERSION 3.10)
project(C_P2P_Chat C)

set(CMAKE_C_STANDARD 11)

# Подключаем ncursesw
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses)

# На macOS системный curses не содержит ncursesw — пробуем homebrew
if(NOT CURSES_INCLUDE_DIRS OR NOT EXISTS "${CURSES_INCLUDE_DIRS}/ncursesw/curses.h")
    foreach(_brew_prefix /opt/homebrew/opt/ncurses /usr/local/opt/ncurses)
        if(EXISTS "${_brew_prefix}/include/ncursesw/curses.h")
            set(CURSES_INCLUDE_DIRS "${_brew_prefix}/include")
            find_library(CURSES_LIBRARIES ncursesw PATHS "${_brew_prefix}/lib" NO_DEFAULT_PATH)
            break()
        endif()
    endforeach()
endif()

if(NOT CURSES_LIBRARIES)
    message(FATAL_ERROR "ncursesw не найден. На macOS установите: brew install ncurses")
endif()

set(SOURCE_FILES
        src/main.c
        src/Chat.c
        src/Clients.c
        src/Network/Socket.c
        src/Network/Packet.c
        src/Utils/Utils.c
        src/Utils/Interface.c)

add_executable(C_P2P_Chat ${SOURCE_FILES})
target_include_directories(C_P2P_Chat PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(C_P2P_Chat ${CURSES_LIBRARIES})
target_compile_options(C_P2P_Chat PRIVATE -Wall -Wextra)

# ── Tests ─────────────────────────────────────────────────────────────
enable_testing()

add_executable(test_utf8_copy
        tests/test_utf8_copy.c
        src/Utils/Utils.c)
target_compile_options(test_utf8_copy PRIVATE -Wall -Wextra)

add_executable(test_packet
        tests/test_packet.c
        src/Network/Packet.c
        src/Clients.c
        src/Network/Socket.c
        src/Utils/Utils.c)
target_compile_options(test_packet PRIVATE -Wall -Wextra)

add_test(NAME utf8_copy COMMAND test_utf8_copy)
add_test(NAME packet    COMMAND test_packet)
