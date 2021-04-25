cmake_minimum_required(VERSION 3.17)
project(courceWorkG)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)

find_package(Qt${QT_VERSION} REQUIRED COMPONENTS ${REQUIRED_LIBS} REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
        forms/MainForm.cpp
        forms/MainForm.hpp
        forms/MainForm.ui
        forms/ImageScaled.cpp
        forms/ImageScaled.hpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/forms)
target_link_libraries(${PROJECT_NAME} PUBLIC ${REQUIRED_LIBS_QUALIFIED})