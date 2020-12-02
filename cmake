cmake_minimum_required(VERSION 3.17)
project(stationmeteo)

set(CMAKE_AUTOMOC ON )
set(CMAKE_CXX_STANDARD 14)

add_executable(stationmeteo main.cpp FenêtreGauge.cpp FenêtreGauge.h Presenter.cpp Presenter.h Jute.cpp Jute.h View.cpp View.h)
find_package(Qt5 COMPONENTS REQUIRED Core Gui Widgets SerialPort)
target_link_libraries(stationmeteo Qt5::Core Qt5::Gui Qt5::Widgets Qt5::SerialPort)
