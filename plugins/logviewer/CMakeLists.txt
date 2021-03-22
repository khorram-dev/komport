set(KF5_MIN_VERSION "5.46.0")
set(QT_REQUIRED_VERSION "5.11.0")
find_package(ECM ${KF5_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})
set(CMAKE_AUTOMOC ON)

set(logviewerplugin_SRC
    filetree.cpp
    logviewerplugin.cpp
    logviewer.cpp
)

add_library(logviewer MODULE ${logviewerplugin_SRC})
kcoreaddons_desktop_to_json(logviewer logviewer.desktop)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    TextEditor
    GuiAddons
)

target_link_libraries(logviewer
    libkomport
    KF5::I18n
    KF5::TextEditor
    KF5::XmlGui
    KF5::GuiAddons
)

install(TARGETS logviewer DESTINATION ${KOMPORT_PLUGIN_INSTALL_DIR})
