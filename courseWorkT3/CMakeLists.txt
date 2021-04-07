cmake_minimum_required(VERSION 3.17)
project(courseWorkT)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_PREFIX_PATH "/usr/include/x86_64-linux-gnu/qt5")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets )
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets )
set(QT_USE_QTXMLPATTERNS TRUE)

add_executable(${PROJECT_NAME} main.cpp forms/MainForm.ui forms/MainForm.cpp forms/MainForm.hpp)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED)
target_link_libraries(${PROJECT_NAME} ${REQUIRED_LIBS_QUALIFIED})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

if (NOT CMAKE_PREFIX_PATH)
    message(WARNING "CMAKE_PREFIX_PATH is not defined, you may need to set it "
            "(-DCMAKE_PREFIX_PATH=\"path/to/Qt/lib/cmake\" or -DCMAKE_PREFIX_PATH=/usr/include/{host}/qt{version}/ on Ubuntu)")
endif ()