project(BrainIM)

cmake_minimum_required(VERSION 2.8)

set(BRAIN_IM_VERSION_MAJOR 0)
set(BRAIN_IM_VERSION_MINOR 1)
set(BRAIN_IM_VERSION_PATCH 0)

set(BRAIN_IM_VERSION "${BRAIN_IM_VERSION_MAJOR}.${BRAIN_IM_VERSION_MINOR}.${BRAIN_IM_VERSION_PATCH}")

# Add an option for building tests
option(ENABLE_TESTS "Enable compilation of automated tests" FALSE)
# Add an option for dev build
option(DEVELOPER_BUILD "Enable extra debug codepaths, like asserts and extra output" FALSE)
# Add an option for dev build
option(ENABLE_WIDGETS "Enable extra debug codepaths, like asserts and extra output" FALSE)
option(ENABLE_QML "Enable extra debug codepaths, like asserts and extra output" TRUE)

set(QT_VERSION_MAJOR "5")

find_package(Qt5 REQUIRED COMPONENTS Core Network DBus Xml Gui Qml Quick Widgets)
find_package(TelepathyQt5 0.9.6 REQUIRED)

#if(ENABLE_TESTAPP)
#    find_package(Qt5 REQUIRED COMPONENTS Core Network Gui Widgets)
#else()
#    find_package(Qt5 REQUIRED COMPONENTS Core Network)
#endif()

include(GNUInstallDirs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

add_subdirectory(base)

if(ENABLE_QML)
    add_subdirectory(qml)
endif()
