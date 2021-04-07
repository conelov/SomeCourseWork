cmake_minimum_required(VERSION 3.17)
#set(cross_qt ON)
#set(CMAKE_TOOLCHAIN_FILE "~/CLionProjects/Toolchain-cross-mingw32-linux.cmake")

project(courseWorkD)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_executable(${PROJECT_NAME} main.cpp forms/mainForm.cpp forms/mainForm.hpp forms/mainForm.ui cellImage.cpp cellImage.hpp)

#cross_addDll( {REQUIRED_LIBS}s

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/forms")
target_link_libraries(${PROJECT_NAME} PUBLIC ${REQUIRED_LIBS_QUALIFIED})