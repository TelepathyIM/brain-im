project(libBrainIM)
set(BRAIN_IM_BASE_ABI "0")

#find_package(Qt5 REQUIRED COMPONENTS Core)

message(STATUS "Found includes: " ${QT5_INCLUDES})

set(base_sources
    AccountsModel
    AccountsModel.cpp
    AccountsModel.hpp
    ConnectionManagersModel
    ConnectionManagersModel.cpp
    ConnectionManagersModel.hpp
    ProtocolsModel
    ProtocolsModel.cpp
    ProtocolsModel.hpp
    AccountParameterModel
    AccountParameterModel.cpp
    AccountParameterModel.hpp
    DBusServiceHelper
    DBusServiceHelper.cpp
    DBusServiceHelper.hpp
    Types.hpp
    Entity.cpp
    Entity.hpp
    Event.cpp
    Event.hpp
)

add_library(BrainIM SHARED ${base_sources})

set_target_properties(BrainIM PROPERTIES AUTOMOC TRUE)

target_include_directories(BrainIM PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}

    ${QT5_INCLUDES}
    ${TELEPATHY_QT5_INCLUDE_DIR}
)

target_link_libraries(BrainIM
    Qt5::Core
    Qt5::DBus
    -lstdc++
)
