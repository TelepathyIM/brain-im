cmake_minimum_required(VERSION 3.3)

project(BrainControls
    VERSION 0.1.0
)

include(GNUInstallDirs)

find_package(Qt5 REQUIRED COMPONENTS Core Qml Quick)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC TRUE)

if (NOT INSTALL_QML_IMPORT_DIR)
    set(INSTALL_QML_IMPORT_DIR ${CMAKE_INSTALL_FULL_LIBDIR}/qt5/qml)
endif()

set(brain_controls_SOURCES
    plugin.cpp
)

add_library(BrainControlsPlugin SHARED ${brain_controls_SOURCES})

target_link_libraries(BrainControlsPlugin PUBLIC
    Qt5::Qml
)

set(qml_SOURCES
    ColumnLayout.qml
    ListView.qml
    RowLayout.qml
    Spacer.qml
)
target_sources(BrainControlsPlugin PRIVATE ${qml_SOURCES})

install(TARGETS BrainControlsPlugin DESTINATION ${INSTALL_QML_IMPORT_DIR}/BrainControls)
install(FILES qmldir plugins.qmltypes DESTINATION ${INSTALL_QML_IMPORT_DIR}/BrainControls)
install(FILES ${qml_SOURCES} DESTINATION ${INSTALL_QML_IMPORT_DIR}/BrainControls)
