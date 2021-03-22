cmake_minimum_required(VERSION 3.3)
project(KOMPORT)

set (QT_MIN_VERSION "5.7.0")
set (KF5_MIN_VERSION "5.15")
find_package(ECM REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules/")
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)
include(GenerateExportHeader)
include(ECMInstallIcons)
include(ECMAddAppIcon)
include(ECMMarkAsTest)
include(ECMMarkNonGuiExecutable)
include(ECMOptionalAddSubdirectory)
#include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

add_definitions(-DTRANSLATION_DOMAIN=\"komport\")

find_package(Qt5SerialPort REQUIRED)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED
    Core
    Widgets
    Svg
)

find_package(Qt5Core ${QT_MIN_VERSION} CONFIG REQUIRED
    Serialport
    Network
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED
  COMPONENTS
    Config
    ConfigWidgets
    CoreAddons
    Crash
    DBusAddons
    IconThemes
    KIO
    Notifications
    KCMUtils
    TextEditor
    Parts
    Solid
    WidgetsAddons
    WindowSystem
    XmlGui
REQUIRED COMPONENTS
    I18n
OPTIONAL_COMPONENTS
    DocTools
)

if(NOT WIN32)
    find_package(KF5 ${KF5_MIN_VERSION} REQUIRED
        NotifyConfig
        )
endif()

# Qt 5.13 deprecated QComboBox::currentIndexChanged(QString) and Qt 5.14 undid that...
#if (NOT Qt5Widgets_VERSION VERSION_LESS 5.14.0)
#add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050d00)
#endif()

# according to http://www.cmake.org/pipermail/cmake/2008-June/022381.html
kde_enable_exceptions()

add_definitions(
  -DQT_USE_FAST_OPERATOR_PLUS
)

#FIND_PACKAGE(Qt5LinguistTools)

ecm_optional_add_subdirectory(kpart)
ecm_optional_add_subdirectory(komport)
if (KF5DocTools_FOUND)
	kdoctools_install(po)
    ecm_optional_add_subdirectory(doc)
endif()
ecm_optional_add_subdirectory(plugins)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

# this needs at least ECM 5.64...
if(${ECM_VERSION} VERSION_GREATER "5.63.0")
    include(KDEClangFormat)

    # add clang-format target for all our real source files
    file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h)
    kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
endif()
ki18n_install(po)
if (KF5DocTools_FOUND)
 kdoctools_install(po)
endif()
