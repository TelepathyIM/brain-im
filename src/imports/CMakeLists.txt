project(BrainIMQml)

find_package(Qt5 REQUIRED COMPONENTS Core Network DBus Xml Gui Qml Quick)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC TRUE)

if (NOT INSTALL_QML_IMPORT_DIR)
    set(INSTALL_QML_IMPORT_DIR ${CMAKE_INSTALL_LIBDIR}/qt5/qml)
endif()

set(brain_qml_SOURCES plugin.cpp)

if (USE_TELEPATHY)
    add_definitions(-DUSE_TELEPATHY)
endif()

add_library(BrainIMQml SHARED ${brain_qml_SOURCES})

target_include_directories(BrainIMQml PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../brain-im
)

target_link_libraries(BrainIMQml
    Qt5::Core
    Qt5::Gui
    Qt5::Network
    Qt5::Qml
    Qt5::Quick
    BrainIM
)

set(qml_SOURCES
    AccountEditor.qml
    AccountView.qml
    ConnectionManagerView.qml
    ProtocolView.qml
    ScrollablePage.qml
)

install(TARGETS BrainIMQml DESTINATION ${INSTALL_QML_IMPORT_DIR}/BrainIM)
install(FILES qmldir plugins.qmltypes DESTINATION ${INSTALL_QML_IMPORT_DIR}/BrainIM)
install(FILES ${qml_SOURCES} DESTINATION ${INSTALL_QML_IMPORT_DIR}/BrainIM)
