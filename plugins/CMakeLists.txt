MACRO (MACRO_KT_PLUGIN _option _name _dir)
option(${_option} "Whether to build the ${_name} plugin or not" true)
if (${_option})
  message(STATUS "Building ${_name} plugin (-D${_option}=false to disable)")
  add_subdirectory(${_dir})
else (${_option})
  message(STATUS "Not building ${_name} plugin (-D${_option}=true to enable)")
endif (${_option})
ENDMACRO (MACRO_KT_PLUGIN)

set(KOMPORT_PLUGIN_INSTALL_DIR ${PLUGIN_INSTALL_DIR}/komport)

macro_kt_plugin(ENABLE_LOGVIEWER_PLUGIN listcommand listcommand)
macro_kt_plugin(ENABLE_LOGVIEWER_PLUGIN logviewer logviewer)
