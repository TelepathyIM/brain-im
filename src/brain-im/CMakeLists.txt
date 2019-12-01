set(base_sources
    Approver.cpp
    Approver.hpp
    Entity.cpp
    Entity.hpp
    Event.cpp
    Event.hpp
    Types.cpp
    Types.hpp
    global.h
)

if (USE_TELEPATHY)
    list(APPEND base_sources
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
endif()

add_library(BrainIM SHARED ${base_sources})
add_library(BrainIM::Core ALIAS BrainIM)

set_target_properties(BrainIM
    PROPERTIES
    DEFINE_SYMBOL "BRAIN_IM_LIBRARY"
)

if (USE_TELEPATHY)
    target_compile_definitions(BrainIM PRIVATE
        USE_TELEPATHY
    )
endif()

target_include_directories(BrainIM PUBLIC
    $<INSTALL_INTERFACE:${TELEGRAM_QT_INCLUDE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(BrainIM
    Qt5::Core
    Qt5::DBus
    Qt5::Gui
    Qt5::Network
    Qt5::Qml
    Qt5::Quick
    TelepathyQt5::Core
)

set(BRAIN_IM_INCLUDE_DIR ${CMAKE_INSTALL_FULL_INCLUDEDIR}/BrainIM)
set(BRAIN_IM_LIB_DIR ${CMAKE_INSTALL_FULL_LIBDIR})

install(
    TARGETS BrainIM
    LIBRARY DESTINATION ${BRAIN_IM_LIB_DIR}
    ARCHIVE DESTINATION ${BRAIN_IM_LIB_DIR}
    COMPONENT Library
)
