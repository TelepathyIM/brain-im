project(BrainIM)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(base_sources
    Approver.cpp
    Approver.hpp
    ContactsModel.cpp
    ContactsModel.hpp
    Entity.cpp
    Entity.hpp
    Event.cpp
    Event.hpp
    PeersModel.cpp
    PeersModel.hpp
    MessagesModel.cpp
    MessagesModel.hpp
    Types.cpp
    Types.hpp
    global.h
)

if (USE_TELEPATHY)
    set(tp_sources
        TelepathyMessagesModel.cpp
        TelepathyMessagesModel.hpp

        AccountManager
        AccountManager.cpp
        AccountManager.hpp

        AccountsModel
        AccountsModel.cpp
        AccountsModel.hpp

        AccountParameterModel
        AccountParameterModel.cpp
        AccountParameterModel.hpp

        ConnectionManagersModel
        ConnectionManagersModel.cpp
        ConnectionManagersModel.hpp

        Core.cpp
        Core.hpp

        DBusServiceHelper
        DBusServiceHelper.cpp
        DBusServiceHelper.hpp

        ProtocolsModel
        ProtocolsModel.cpp
        ProtocolsModel.hpp
    )
    list(APPEND base_sources
        ${tp_sources}
    )
    add_definitions(-DUSE_TELEPATHY)
endif()

add_definitions(-DBRAIN_IM_LIBRARY)
add_library(BrainIM SHARED ${base_sources})

target_include_directories(BrainIM PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}

    ${TELEPATHY_QT_INCLUDE_DIR}
)

target_link_libraries(BrainIM
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    Qt5::Network
    Qt5::Qml
    Qt5::Quick
    ${TELEPATHY_QT_LIBRARIES}
    -lstdc++
)

set(BRAIN_IM_INCLUDE_DIR ${CMAKE_INSTALL_FULL_INCLUDEDIR}/BrainIM)
set(BRAIN_IM_LIB_DIR ${CMAKE_INSTALL_FULL_LIBDIR})

install(
    TARGETS BrainIM
    LIBRARY DESTINATION ${BRAIN_IM_LIB_DIR}
    ARCHIVE DESTINATION ${BRAIN_IM_LIB_DIR}
    COMPONENT Library
)
