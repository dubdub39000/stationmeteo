cmake_minimum_required(VERSION 3.17)
project(stationmeteoV4)

set(CMAKE_AUTOMOC ON )
set(CMAKE_CXX_STANDARD 14)

add_executable(stationmeteoV4 main.cpp FenêtreGauge.cpp FenêtreGauge.h Presenter.cpp Presenter.h Jute.cpp Jute.h View.cpp View.h Setting.cpp Setting.h Logview.cpp Logview.h)
find_package(Qt5 COMPONENTS Network REQUIRED Core Gui Widgets)
target_link_libraries(stationmeteoV4 Qt5::Core Qt5::Gui Qt5::Widgets  Qt5::Network)

