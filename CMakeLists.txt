cmake_minimum_required(VERSION 3.19)
project(2048Game)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_PREFIX_PATH "/opt/homebrew/Cellar/qt@5/5.15.10_1/lib/cmake")
set(CMAKE_OSX_ARCHITECTURES "x86_64")

ADD_DEFINITIONS(-D_CLion)

find_package(Qt5Widgets REQUIRED)

add_executable(2048Game main.cpp mainwindow.h mainwindow.cpp GameArea.cpp GameArea.h GameAreaWinWidget.cpp GameAreaWinWidget.h GameAreaEndWidget.cpp GameAreaEndWidget.h)
target_link_libraries(2048Game Qt5::Widgets)
