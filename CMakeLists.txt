cmake_minimum_required(VERSION 3.26.4)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-O2")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


#************************************

project(aedit)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Multimedia
        REQUIRED)

add_subdirectory(lib/portaudio bin EXCLUDE_FROM_ALL)

include_directories(src/core)
include_directories(src/app)

add_executable(aedit src/main.cpp
        src/app/mainwindow.cpp
        src/app/mainwindow.h
        src/app/MainWindow.ui
        src/core/CoreInfo.h
        src/core/CoreAudio.cpp
        src/core/CoreAudio.h
        src/core/pa_callback.c
)


target_link_libraries(aedit
        portaudio_static
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Multimedia
)


