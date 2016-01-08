# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:

cmake_minimum_required (VERSION 2.8.11)

project (AsynQt)

set (REQUIRED_QT_VERSION 5.3.0)

# We don't build in-source
if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
   message (
      FATAL_ERROR
      "asynqt requires an out of source build. Please create a separate build directory and run 'cmake path_to_plasma [options]' there."
   )
endif ()

# Extra CMake stuff
include (FeatureSummary)
find_package (ECM 5.17.0  NO_MODULE)
set_package_properties (ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary (WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include (KDEInstallDirs)
include (KDECMakeSettings)
include (KDECompilerSettings)
include (GenerateExportHeader)
include (ECMGenerateHeaders)

# Qt
set (CMAKE_AUTOMOC ON)
find_package (Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED COMPONENTS Core)

# KDE Frameworks
set(KF5_VERSION "5.18.0") # handled by release scripts
set(KF5_DEP_VERSION "5.17.0") # handled by release scripts

find_package (KF5I18n ${KF5_DEP_VERSION} CONFIG REQUIRED)
find_package (KF5KIO ${KF5_DEP_VERSION} CONFIG REQUIRED)
find_package (KF5Config ${KF5_DEP_VERSION} CONFIG REQUIRED)

# Basic includes
include (CPack)

include (ECMPackageConfigHelpers)
include (ECMSetupVersion)

add_definitions (-DTRANSLATION_DOMAIN=\"asynqt5\")
if (IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/po")
    ki18n_install (po)
endif ()

# libAsynQt

ecm_setup_version (
   ${KF5_VERSION}
   VARIABLE_PREFIX ASYNQT
   VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/asynqt_version.h"
   PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5AsynQtConfigVersion.cmake"
   SOVERSION 5
   )

set (CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5AsynQt")

install (
   EXPORT KF5AsynQtLibraryTargets
   DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
   FILE KF5AsynQtLibraryTargets.cmake
   NAMESPACE KF5::
   )

ecm_configure_package_config_file (
   "${CMAKE_CURRENT_SOURCE_DIR}/KF5AsynQtConfig.cmake.in"
   "${CMAKE_CURRENT_BINARY_DIR}/KF5AsynQtConfig.cmake"
   INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
   PATH_VARS  KF5_INCLUDE_INSTALL_DIR CMAKE_INSTALL_PREFIX
   )

install (
   FILES "${CMAKE_CURRENT_BINARY_DIR}/KF5AsynQtConfig.cmake"
         "${CMAKE_CURRENT_BINARY_DIR}/KF5AsynQtConfigVersion.cmake"
   DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
   COMPONENT Devel
   )

install (
   FILES ${CMAKE_CURRENT_BINARY_DIR}/asynqt_version.h
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5} COMPONENT Devel
   )

add_subdirectory (src)
add_subdirectory (autotests)
add_subdirectory (tests)

# Write out the features
feature_summary (WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

