cmake_minimum_required(VERSION 3.16)

project(JsonSerializer LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Test Gui)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Test Gui)

add_subdirectory(lib)
add_subdirectory(tests)

include(GNUInstallDirs)
install(TARGETS JsonSerializer
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
