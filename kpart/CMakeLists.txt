set(CMAKE_AUTOMOC ON)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    )

set(libkomport_SRC
    device/serialport.cpp
    device/tcpclient.cpp
    device/tcpserver.cpp
    device/udp.cpp
    gui/centralwidget.cpp
    gui/klistwidget.cpp
    interfaces/activity.cpp
    interfaces/guiinterface.cpp
    interfaces/plugin.cpp
    interfaces/prefpageinterface.cpp
    interfaces/serialportinterface.cpp
    interfaces/socketinterface.cpp
    plugin/pluginmanager.cpp
    plugin/pluginsetting.cpp
    util/error.cpp
    util/waitjob.cpp
    komport.kcfg
)

kconfig_add_kcfg_files(libkomport_SRC settings.kcfgc)
add_library(libkomport ${libkomport_SRC})
generate_export_header(libkomport BASE_NAME libkomport)
set_property(TARGET libkomport PROPERTY CXX_STANDARD 14)
set_target_properties(libkomport PROPERTIES OUTPUT_NAME komport)

target_link_libraries(libkomport PUBLIC
    Qt5::Widgets
    Qt5::Svg
    Qt5::SerialPort
    Qt5::Network
    KF5::ConfigCore
    KF5::CoreAddons
    KF5::I18n
    KF5::KCMUtils
    KF5::Parts
    KF5::Solid
    KF5::XmlGui
)

target_include_directories(libkomport PUBLIC "$<BUILD_INTERFACE:${KOMPORT_SOURCE_DIR}/libkomport;${KOMPORT_BINARY_DIR}/libkomport;${KOMPORT_BINARY_DIR}>")
set_target_properties(libkomport PROPERTIES VERSION 0.0.1 SOVERSION 1 )
install(TARGETS libkomport ${INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)
