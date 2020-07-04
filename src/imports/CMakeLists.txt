find_package(Qt5 REQUIRED COMPONENTS Core Network DBus Xml Gui Qml Quick)

add_definitions(-DQT_NO_CAST_FROM_ASCII)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC TRUE)

if (NOT INSTALL_QML_IMPORT_DIR)
    set(INSTALL_QML_IMPORT_DIR ${CMAKE_INSTALL_LIBDIR}/qt5/qml)
endif()

set(brain_qml_SOURCES
    DeclarativeAccountsModel.cpp
    DeclarativeAccountsModel.hpp
    plugin.cpp
)

add_library(BrainIMQml SHARED ${brain_qml_SOURCES})
add_library(BrainIM::Qml ALIAS BrainIMQml)

if (USE_TELEPATHY)
    target_compile_definitions(BrainIMQml PRIVATE
        USE_TELEPATHY
    )
endif()

target_link_libraries(BrainIMQml PUBLIC
    Qt5::Qml
    Qt5::Quick
    BrainIM::Core
)

set(qml_SOURCES
    AccountEditor.qml
    AccountView.qml
    ConnectionManagerView.qml
    ProtocolView.qml
    ScrollablePage.qml
)
target_sources(BrainIMQml PRIVATE ${qml_SOURCES})

install(TARGETS BrainIMQml DESTINATION ${INSTALL_QML_IMPORT_DIR}/BrainIM)
install(FILES qmldir plugins.qmltypes DESTINATION ${INSTALL_QML_IMPORT_DIR}/BrainIM)
install(FILES ${qml_SOURCES} DESTINATION ${INSTALL_QML_IMPORT_DIR}/BrainIM)
