cmake_minimum_required(VERSION 3.3)

project(BrainIM
    VERSION 0.1.0
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Add an option for building tests
option(ENABLE_TESTS "Enable compilation of automated tests" FALSE)
# Add an option for dev build
option(DEVELOPER_BUILD "Enable extra debug codepaths, like asserts and extra output" FALSE)

option(ENABLE_QML "Enable declarative plugin" TRUE)
option(ENABLE_QML_CLIENT "Enable QML Client build" FALSE)

option(USE_TELEPATHY "Build with Telepathy support" TRUE)

if (NOT INSTALL_QML_IMPORT_DIR)
    set(INSTALL_QML_IMPORT_DIR ${CMAKE_INSTALL_LIBDIR}/qt5/qml)
endif()

find_package(Qt5 CONFIG REQUIRED COMPONENTS
    Core
    DBus
    Gui
    Network
    Qml
    Quick
    Xml
    Widgets
)

set(QT_VERSION_MAJOR "5")

if (USE_TELEPATHY)
    find_package(TelepathyQt5 0.9.8 COMPONENTS Core Service REQUIRED)
endif()

include(GNUInstallDirs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W -Wall -Wextra -std=c++11 -Werror=return-type")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

add_subdirectory(src/brain-im)

if (ENABLE_QML)
    add_subdirectory(src/imports)
endif()

if (ENABLE_QML_CLIENT)
    add_subdirectory(examples/qmlclient)
endif()

message(STATUS "BrainIM configuration:")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Qt: ${Qt5_VERSION} at ${_qt5Core_install_prefix}")
if (USE_TELEPATHY)
    message(STATUS "  TelepathyQt: ${TELEPATHY_QT5_VERSION}")
endif()
