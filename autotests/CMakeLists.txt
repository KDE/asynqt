# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:
project (AsynQtTest)

find_package (Qt5 REQUIRED NO_MODULE COMPONENTS Test Core DBus)

include_directories (
   ${CMAKE_SOURCE_DIR}/src/
   ${CMAKE_BINARY_DIR}/src/lib/core/
   ${CMAKE_BINARY_DIR}/src/
   )

set (
   AsynQtTest_SRCS

   main.cpp

   wrappers/qprocess_test.cpp
   wrappers/qdbus_test.cpp

   base/transform_test.cpp
   base/flatten_test.cpp
   base/continuewith_test.cpp
   # Ready.cpp
   )

if (NOT WIN32)

   add_executable (
      AsynQtTest
      ${AsynQtTest_SRCS}
      )

   target_link_libraries (
      AsynQtTest
      Qt5::Core
      Qt5::Test
      Qt5::DBus
      KF5::AsynQt
      )

endif ()
