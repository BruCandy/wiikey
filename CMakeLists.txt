cmake_minimum_required(VERSION 3.28.3)
project(wiikey LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig   REQUIRED)
pkg_check_modules(GTK3       REQUIRED gtk+-3.0)
pkg_check_modules(PANGOCAIRO REQUIRED pangocairo)

add_library(wiikey_lib
    src/utf8.cpp
    src/modifier.cpp
    src/flick.cpp
    src/ui.cpp
    src/uinput_dev.cpp
)

target_include_directories(wiikey_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
           ${GTK3_INCLUDE_DIRS}
           ${PANGOCAIRO_INCLUDE_DIRS}
)

target_compile_options(wiikey_lib PRIVATE
    ${GTK3_CFLAGS_OTHER}
    ${PANGOCAIRO_CFLAGS_OTHER}
)

target_link_libraries(wiikey_lib PRIVATE
    ${GTK3_LIBRARIES}
    ${PANGOCAIRO_LIBRARIES}
)

add_executable(wiikey
    src/main.cpp
)

target_link_libraries(wiikey PRIVATE
    wiikey_lib
)
