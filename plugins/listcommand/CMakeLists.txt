set(commandplugin_SRC
    commandplugin.cpp
    guicommand.cpp
    commandsettings.cpp
    guicommand.ui
)

ki18n_wrap_ui(commandplugin_SRC commandsettings.ui guicommand.ui)
kconfig_add_kcfg_files(commandplugin_SRC commandpluginsettings.kcfgc)
add_library(listcommand MODULE ${commandplugin_SRC})

kcoreaddons_desktop_to_json(listcommand listcommand.desktop)

target_link_libraries(
    listcommand
    libkomport
    KF5::ConfigCore
    KF5::I18n
    KF5::XmlGui
)

install(TARGETS listcommand DESTINATION ${KOMPORT_PLUGIN_INSTALL_DIR} )
