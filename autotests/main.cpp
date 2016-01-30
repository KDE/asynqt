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

#include <QtTest/QtTest>
#include <QCoreApplication>
#include <QList>

#include "common.h"

#include "wrappers/basic_test.h"
#include "wrappers/qdbus_test.h"
#include "wrappers/qprocess_test.h"

#include "base/cast_test.h"
#include "base/continuewith_test.h"
#include "base/filter_test.h"
#include "base/flatten_test.h"
#include "base/transform_test.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    RUN_TEST(wrappers::ProcessExecutionTest);
    RUN_TEST(wrappers::DBusExecutionTest);
    RUN_TEST(wrappers::BasicFuturesTest);

    RUN_TEST(base::CastTest);
    RUN_TEST(base::TransformTest);
    RUN_TEST(base::FlattenTest);
    RUN_TEST(base::FilterTest);
    RUN_TEST(base::ContinueWithTest);

    return 1;
}

