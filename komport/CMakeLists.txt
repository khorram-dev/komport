INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}/../kpart
                    ${CMAKE_CURRENT_SOURCE_DIR}/../kpart
                    )

# application icon, in all sizes needed to have e.g. nice Windows icons, too
set(ICON_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/resource/png/sc-apps-komport.svg
  ${CMAKE_CURRENT_SOURCE_DIR}/resource/png/16-apps-komport.png
  ${CMAKE_CURRENT_SOURCE_DIR}/resource/png/22-apps-komport.png
  ${CMAKE_CURRENT_SOURCE_DIR}/resource/png/32-apps-komport.png
  ${CMAKE_CURRENT_SOURCE_DIR}/resource/png/44-apps-komport.png
  ${CMAKE_CURRENT_SOURCE_DIR}/resource/png/48-apps-komport.png
  ${CMAKE_CURRENT_SOURCE_DIR}/resource/png/64-apps-komport.png
  ${CMAKE_CURRENT_SOURCE_DIR}/resource/png/128-apps-komport.png
  ${CMAKE_CURRENT_SOURCE_DIR}/resource/png/150-apps-komport.png
  ${CMAKE_CURRENT_SOURCE_DIR}/resource/png/256-apps-komport.png
  ${CMAKE_CURRENT_SOURCE_DIR}/resource/png/310-apps-komport.png
  ${CMAKE_CURRENT_SOURCE_DIR}/resource/png/512-apps-komport.png
)

# Add icon files to the application's source files to have CMake bundle them in the executable.
ecm_add_app_icon(ICONS_SOURCES ICONS ${ICON_FILES})

set(komport_SRC
    main.cpp
    mainwindow.cpp
    gui.cpp
    statusbar.cpp
    trayicon.cpp
    pref/colorschemechooser.cpp
    pref/generalpref.cpp
    pref/prefdialog.cpp
    view/editor.cpp
    view/newdevice.cpp
    view/kserialport.cpp
    view/ksocket.cpp
    resource/resource.qrc
    data/komport.qrc
    ${RCC_SOURCES}
    )

ki18n_wrap_ui(komport_SRC
    pref/generalpref.ui
    view/newdevice.ui
    )

#FIND_PACKAGE(Qt5LinguistTools)
#qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} english.ts)
#QT5_ADD_TRANSLATION( QM_FILES )

add_executable(komport_app ${komport_SRC} ${QM_FILES})
set_property(TARGET komport_app PROPERTY CXX_STANDARD 14)
set_target_properties(komport_app PROPERTIES OUTPUT_NAME komport)

target_link_libraries(komport_app
    libkomport
    Qt5::SerialPort
    KF5::Crash
    KF5::ConfigCore
    KF5::ConfigGui
    KF5::DBusAddons
    KF5::I18n
    KF5::IconThemes
    KF5::KIOCore
    KF5::KIOFileWidgets
    KF5::TextEditor
    KF5::Notifications
    KF5::Parts
    KF5::Solid
    KF5::WidgetsAddons
    KF5::WindowSystem
    KF5::XmlGui
    KF5::Crash
)


# optional KUserFeedback integration
#find_package(UserFeedback)
#if (TARGET KUserFeedbackWidgets)
#    target_link_libraries(komport_app PUBLIC KUserFeedbackWidgets)
#    target_compile_definitions(komport_app PUBLIC -DWITH_KUSERFEEDBACK)
#endif()

if(NOT WIN32)
    target_link_libraries(komport_app
        KF5::NotifyConfig
        )
endif()

install(TARGETS komport_app ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES data/komportappui.rc DESTINATION ${KXMLGUI_INSTALL_DIR}/komport )
install(FILES data/serialportui.rc DESTINATION ${KXMLGUI_INSTALL_DIR}/komport )
install(FILES data/socketui.rc DESTINATION ${KXMLGUI_INSTALL_DIR}/komport )
install(FILES data/komport.notifyrc DESTINATION ${KNOTIFYRC_INSTALL_DIR} )
install(FILES data/org.kde.komport.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR} )
install(PROGRAMS data/org.kde.komport.desktop  DESTINATION  ${XDG_APPS_INSTALL_DIR} )

ecm_install_icons(
  ICONS ${ICON_FILES}
  DESTINATION ${ICON_INSTALL_DIR}
  THEME hicolor
)
