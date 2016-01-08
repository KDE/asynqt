prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=${BIN_INSTALL_DIR}
libdir=${LIB_INSTALL_DIR}
includedir=${INCLUDE_INSTALL_DIR}

Name: libAsynQt
Description: libAsynQt provides useful features for QFuture
URL: http://www.kde.org
Requires:
Version: ${ASYNQT_LIB_VERSION_STRING}
Libs: -L${LIB_INSTALL_DIR} -lKF5AsynQt
Cflags: -I${INCLUDE_INSTALL_DIR}
