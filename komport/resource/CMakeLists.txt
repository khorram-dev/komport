# collect icons
set(KOMPORT_ICONS_PNG
    icons/16-apps-komport.png
    icons/22-apps-komport.png
    icons/32-apps-komport.png
    icons/48-apps-komport.png
    icons/64-apps-komport.png
    icons/128-apps-komport.png
)

list (APPEND RESOURCES resource.qrc)
qt5_add_resources(RCC_SOURCES ${RESOURCES})

ecm_add_app_icon(komport_SRC ICONS ${KOMPORT_ICONS_PNG})
qt5_add_resources(komport_ICON resource.qrc)

#set(komport_Icon resource.qrc)

ecm_install_icons(ICONS
    16-apps-komport.png
    22-apps-komport.png
    32-apps-komport.png
    48-apps-komport.png
    64-apps-komport.png
    128-apps-komport.png
    DESTINATION ${ICON_INSTALL_DIR}
    THEME hicolor
)
