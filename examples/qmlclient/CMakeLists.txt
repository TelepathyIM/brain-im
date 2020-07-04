find_package(Qt5 REQUIRED COMPONENTS Core Network DBus Xml Gui Qml Quick QuickControls2)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(brain_qml_SOURCES
    "main.cpp"
    "qml.qrc"
)

add_executable(brain-im-client ${brain_qml_SOURCES})

target_link_libraries(brain-im-client
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::Qml
    Qt5::Quick
    Qt5::QuickControls2
    BrainIM::Core
)

if (USE_TELEPATHY)
    target_compile_definitions(brain-im-client PRIVATE
        USE_TELEPATHY
    )
    target_link_libraries(brain-im-client
        TelepathyQt5::Core
    )
endif()

set(QML_IMPORT_PATH ${INSTALL_QML_IMPORT_DIR})

message(STATUS "Hint: The qmlclient example requires complete installation. Ensure that you have"
    " built CMake target 'install'")
message(STATUS "Hint: Add QML2_IMPORT_PATH=${CMAKE_INSTALL_FULL_LIBDIR}/qt5/qml "
    "to 'brain-im' run enviroment variables to ensure that the QML import is correct.")
