/*
 *   Copyright (C) 2016 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   This library is free software; you can redistribute it and/or
 *   modify it under the terms of the GNU Lesser General Public
 *   License as published by the Free Software Foundation; either
 *   version 2.1 of the License, or (at your option) version 3, or any
 *   later version accepted by the membership of KDE e.V. (or its
 *   successor approved by the membership of KDE e.V.), which shall
 *   act as a proxy defined in Section 6 of version 3 of the license.
 *
 *   This library is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with this library.
 *   If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ASYNQT_VERSION_H
#define ASYNQT_VERSION_H

/** @file version.h <AsynQt/Version> */

#include <asynqt/asynqt_export.h>
// #include "asynqt_export.h"

/**
 * String version of libAsynQt version, suitable for use in
 * file formats or network protocols
 */
#define ASYNQT_VERSION_STRING \
    "6.2.0"

/// @brief Major version of libAsynQt, at compile time
#define ASYNQT_VERSION_MAJOR \
    6
/// @brief Minor version of libAsynQt, at compile time
#define ASYNQT_VERSION_MINOR \
    2
/// @brief Release version of libAsynQt, at compile time
#define ASYNQT_VERSION_RELEASE \
    0

#define ASYNQT_MAKE_VERSION(a, b, c) \
    (((a) << 16) | ((b) << 8) | (c))

/**
 * Compile time macro for the version number of libAsynQt
 */
#define ASYNQT_VERSION \
    ASYNQT_MAKE_VERSION(ASYNQT_VERSION_MAJOR, ASYNQT_VERSION_MINOR, ASYNQT_VERSION_RELEASE)

/**
 * Compile-time macro for checking the AsynQt version. Not useful for
 * detecting the version of libAsynQt at runtime.
 */
#define ASYNQT_IS_VERSION(a, b, c) \
    (ASYNQT_VERSION >= ASYNQT_MAKE_VERSION(a, b, c))

/**
 * Namespace for everything in libAsynQt
 */
namespace AsynQt {

/**
 * The runtime version of libAsynQt
 */
ASYNQT_EXPORT unsigned int version();

/**
 * The runtime major version of libAsynQt
 */
ASYNQT_EXPORT unsigned int versionMajor();

/**
 * The runtime major version of libAsynQt
 */
ASYNQT_EXPORT unsigned int versionMinor();

/**
 * The runtime major version of libAsynQt
 */
ASYNQT_EXPORT unsigned int versionRelease();

/**
 * The runtime version string of libAsynQt
 */
ASYNQT_EXPORT const char *versionString();

} // AsynQt namespace

#endif // multiple inclusion guard
