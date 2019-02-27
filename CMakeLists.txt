project(brain_im)

cmake_minimum_required(VERSION 3.3)

set(BRAIN_IM_VERSION_MAJOR 0)
set(BRAIN_IM_VERSION_MINOR 1)
set(BRAIN_IM_VERSION_PATCH 0)

set(BRAIN_IM_VERSION "${BRAIN_IM_VERSION_MAJOR}.${BRAIN_IM_VERSION_MINOR}.${BRAIN_IM_VERSION_PATCH}")

# Add an option for building tests
option(ENABLE_TESTS "Enable compilation of automated tests" FALSE)
# Add an option for dev build
option(DEVELOPER_BUILD "Enable extra debug codepaths, like asserts and extra output" FALSE)

option(ENABLE_QML "Enable declarative plugin" TRUE)
option(ENABLE_QML_CLIENT "Enable QML Client build" FALSE)

option(USE_TELEPATHY "Build with Telepathy support" TRUE)

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
    find_package(TelepathyQt5 0.9.7 REQUIRED)
    if (TelepathyQt5_FOUND)
        set(TELEPATHY_QT_VERSION ${TELEPATHY_QT5_VERSION})
        set(TELEPATHY_QT_FOUND ${TelepathyQt5_FOUND})
        set(TELEPATHY_QT_INSTALL_DIR ${TELEPATHY_QT5_INSTALL_DIR})
        set(TELEPATHY_QT_INCLUDE_DIR ${TELEPATHY_QT5_INCLUDE_DIR})
        set(TELEPATHY_QT_LIBRARIES ${TELEPATHY_QT5_LIBRARIES})
    endif()
endif()

include(GNUInstallDirs)

if (NOT INSTALL_QML_IMPORT_DIR)
    set(INSTALL_QML_IMPORT_DIR ${CMAKE_INSTALL_FULL_LIBDIR}/qt5/qml)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
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
    message(STATUS "  TelepathyQt: ${TELEPATHY_QT_VERSION} at ${TELEPATHY_QT_INSTALL_DIR}")
endif()
