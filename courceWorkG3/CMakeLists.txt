cmake_minimum_required(VERSION 3.17)
set(cross_qt ON)
set(CMAKE_TOOLCHAIN_FILE "~/CLionProjects/Toolchain-cross-mingw32-linux.cmake")
project(courceWorkG)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)

add_executable(${PROJECT_NAME} main.cpp)
cross_addDll(
        ${REQUIRED_LIBS}
)
add_library(${PROJECT_NAME}.impl
        forms/MainForm.cpp
        forms/MainForm.hpp
        forms/MainForm.ui
        forms/ImageScaled.cpp
        forms/ImageScaled.hpp)
target_include_directories(${PROJECT_NAME}.impl PUBLIC ${PROJECT_SOURCE_DIR}/forms)
target_link_libraries(${PROJECT_NAME}.impl PUBLIC ${REQUIRED_LIBS_QUALIFIED})

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}.impl)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})