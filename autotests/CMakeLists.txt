# vim:set softtabstop=3 shiftwidth=3 tabstop=3 expandtab:
project (AsynQtTest)

find_package (Qt5 REQUIRED NO_MODULE COMPONENTS Test Core DBus)

include_directories (
   ${CMAKE_SOURCE_DIR}/src/
   ${CMAKE_BINARY_DIR}/src/lib/core/
   ${CMAKE_BINARY_DIR}/src/
   )

string (REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
add_definitions (-fexceptions)
add_definitions (-DENABLE_EVIL_QFUTURE_HACKS_THAT_SHOULD_BE_IN_QT)

set (
   AsynQtTest_SRCS

   main.cpp

   wrappers/basic_test.cpp
   wrappers/qdbus_test.cpp
   wrappers/qprocess_test.cpp

   base/cast_test.cpp
   base/continuewith_test.cpp
   base/filter_test.cpp
   base/flatten_test.cpp
   base/transform_test.cpp

   common.cpp
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
