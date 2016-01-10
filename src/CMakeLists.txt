# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

# =======================================================
# Now that we finished with the boilerplate, start
# with the library definition

set (
   AsynQt_LIB_SRCS

   version.cpp

   wrappers/process.cpp
   version.cpp
   basic/readyfuture.cpp
   basic/delayedfuture.cpp

   )

add_library (
   KF5AsynQt SHARED
   ${AsynQt_LIB_SRCS}
   )
add_library (KF5::AsynQt ALIAS KF5AsynQt)

message(
   ">>>>>>>>>>> ${CMAKE_CURRENT_BINARY_DIR}/asynqt"
   )

include_directories (
   ${CMAKE_CURRENT_BINARY_DIR}
   )

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
file (
   COPY
       ${CMAKE_CURRENT_BINARY_DIR}/asynqt_export.h
   DESTINATION
       ${CMAKE_CURRENT_BINARY_DIR}/asynqt/
   )

install (
   FILES ${CMAKE_CURRENT_BINARY_DIR}/asynqt/asynqt_export.h
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/AsynQt/asynqt
   COMPONENT Devel
   )

# Generate top level headers

ecm_generate_headers (
   AsynQt_CamelCase_HEADERS
   HEADER_NAMES
   Version
   PREFIX AsynQt
   )

install (
   FILES ${AsynQt_CamelCase_HEADERS}
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/AsynQt/AsynQt
   COMPONENT Devel
   )

# Generate basic headers

ecm_generate_headers (
   AsynQt_Basic_CamelCase_HEADERS
   HEADER_NAMES
      CanceledFuture
      DelayedFuture
      ReadyFuture
   PREFIX AsynQt/Basic
   RELATIVE basic
   )

install (
   FILES ${AsynQt_Basic_CamelCase_HEADERS}
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/AsynQt/AsynQt/Basic
   COMPONENT Devel
   )

# Generate operations headers

ecm_generate_headers (
   AsynQt_Operations_CamelCase_HEADERS
   HEADER_NAMES
      ContinueWith
      Flatten
      Transform
   PREFIX AsynQt/Operations
   RELATIVE operations
   )

install (
   FILES ${AsynQt_Operations_CamelCase_HEADERS}
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/AsynQt/AsynQt/Operations
   COMPONENT Devel
   )

# Generate wrappers headers

ecm_generate_headers (
   AsynQt_Wrappers_CamelCase_HEADERS
   HEADER_NAMES
      DBus
      Process
      PREFIX AsynQt/Wrappers
   RELATIVE wrappers
   )

install (
   FILES ${AsynQt_Wrappers_CamelCase_HEADERS}
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/AsynQt/AsynQt/Wrappers
   COMPONENT Devel
   )


# Install ugly headers

set (AsynQt_Ugly_HEADERS
   private/wrappers/process_p.h
   private/wrappers/dbus_p.h
   private/basic/delayedfuture_p.h
   private/basic/canceledfuture_p.h
   private/basic/readyfuture_p.h
   private/operations/flatten_p.h
   private/operations/transform_p.h
   wrappers/dbus.h
   wrappers/process.h
   version.h
   basic/delayedfuture.h
   basic/readyfuture.h
   basic/canceledfuture.h
   basic/all.h
   operations/transform.h
   operations/flatten.h
   operations/continuewith.h
   )

foreach (header ${AsynQt_Ugly_HEADERS})
    get_filename_component (dir ${header} DIRECTORY)
    install (FILES ${header} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/AsynQt/asynqt/${dir})
endforeach()

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

