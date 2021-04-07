cmake_minimum_required(VERSION 3.5)
set(CLP_DIRECTORY "$ENV{HOME}/CLionProjects")
#include(${CLP_DIRECTORY}/Toolchain-cross-mingw32-linux.cmake)
project(evolutionTrees LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
#set(QT_HOST_PATH /usr/lib/qt5/bin)
#set(Qt6HostInfo_DIR /usr/lib/qt5/bin)

#add_subdirectory(${CLP_DIRECTORY}/Qt/qtbase build)
#add_subdirectory(${CLP_DIRECTORY}/qtbase-everywhere-src-6.0.0 build)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
add_subdirectory(./libTree)

add_executable(evolutionTrees
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        advancedSettingForm.ui
        AdvancedSettingForm.h
        AdvancedSettingForm.cpp
        src/ArenaToImage.cpp
        include/ArenaToImage.hpp
        src/ScaledImageLabel.cpp
        include/ScaledImageLabel.hpp
        src/ScaledMainImageLabel.cpp
        include/ScaledMainImageLabel.hpp
        AdvancedSettingForm.cpp
        AdvancedSettingForm.h)

target_include_directories(evolutionTrees PRIVATE
        ${CMAKE_BINARY_DIR}
        PUBLIC
        ./include)
target_link_libraries(evolutionTrees PRIVATE Qt5::Widgets Tree)
