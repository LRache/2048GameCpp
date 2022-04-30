cmake_minimum_required(VERSION 3.19)
project(2048Game)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

ADD_DEFINITIONS(-D_CLion)

find_package(Qt5Widgets REQUIRED)

add_executable(2048Game main.cpp mainwindow.h mainwindow.cpp GameArea.cpp GameArea.h GameAreaWinWidget.cpp GameAreaWinWidget.h)
target_link_libraries(2048Game Qt5::Widgets)
