# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

# =======================================================
# Now that we finished with the boilerplate, start
# with the library definition

set (
   AsynQt_LIB_SRCS

   version.cpp

   wrappers/common.cpp

   wrappers/process.cpp

   )

add_library (
   KF5AsynQt SHARED
   ${AsynQt_LIB_SRCS}
   )
add_library (KF5::AsynQt ALIAS KF5AsynQt)

# include_directories (
#    )

set_target_properties (
   KF5AsynQt
   PROPERTIES
   VERSION ${ASYNQT_VERSION_STRING}
   SOVERSION ${ASYNQT_SOVERSION}
   EXPORT_NAME AsynQt
   )

target_link_libraries (
   KF5AsynQt
   PUBLIC
      Qt5::Core
   PRIVATE
      KF5::ConfigCore
      KF5::I18n
      KF5::KIOCore
   )

target_include_directories (
   KF5AsynQt
   INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/AsynQt>"
   )

# install
generate_export_header (KF5AsynQt BASE_NAME AsynQt)

ecm_generate_headers (
   AsynQt_CamelCase_HEADERS
   HEADER_NAMES
   Version
   wrappers/Common
   wrappers/DBus
   wrappers/Process
   PREFIX AsynQt
   REQUIRED_HEADERS AsynQt_HEADERS
   )

install (
   FILES ${AsynQt_CamelCase_HEADERS}
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/AsynQt/AsynQt
   COMPONENT Devel
   )

install (
   FILES ${AsynQt_HEADERS} ${CMAKE_CURRENT_BINARY_DIR}/asynqt_export.h
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/AsynQt/asynqt
   COMPONENT Devel
   )

install (
   TARGETS KF5AsynQt
   EXPORT KF5AsynQtLibraryTargets
   ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
   )

if (NOT WIN32)
   configure_file (
      ${CMAKE_CURRENT_SOURCE_DIR}/libAsynQt.pc.cmake
      ${CMAKE_CURRENT_BINARY_DIR}/libAsynQt.pc
      )
   install (
      FILES ${CMAKE_CURRENT_BINARY_DIR}/libAsynQt.pc
      DESTINATION ${KDE_INSTALL_LIBDIR}/pkgconfig
      )
endif ()

include (ECMGeneratePriFile)
ecm_generate_pri_file (
   BASE_NAME AsynQt
   LIB_NAME KF5AsynQt
   DEPS "KDBusAddons"
   FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/AsynQt
   )
install (
   FILES ${PRI_FILENAME}
   DESTINATION ${ECM_MKSPECS_INSTALL_DIR}
   )

