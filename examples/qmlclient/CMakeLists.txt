project(QmlClient)

find_package(Qt5 REQUIRED COMPONENTS Core Network DBus Xml Gui Qml Quick QuickControls2)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(brain_qml_SOURCES
    "main.cpp"
    "qml.qrc"
)

add_executable(brain-im ${brain_qml_SOURCES})

target_include_directories(brain-im PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src/brain-im

    ${TELEPATHY_QT_INCLUDE_DIR}
)

target_link_libraries(brain-im
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
    BrainIM
)

if (USE_TELEPATHY)
    add_definitions(-DUSE_TELEPATHY)
    target_link_libraries(brain-im
        ${TELEPATHY_QT_LIBRARIES}
    )
endif()

set(QML_IMPORT_PATH ${INSTALL_QML_IMPORT_DIR})

message(STATUS "Hint: Add 'QML2_IMPORT_PATH=${INSTALL_QML_IMPORT_DIR}' "
    "to 'brain-im' run enviroment variables to ensure that the QML import is correct.")
