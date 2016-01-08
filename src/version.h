/*
 *   Copyright 2008 by Aaron Seigo <aseigo@kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU Library General Public License as
 *   published by the Free Software Foundation; either version 2, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU Library General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef ASYNQT_VERSION_H
#define ASYNQT_VERSION_H

/** @file version.h <AsynQt/Version> */

#include "asynqt_export.h"

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
