/*
 *   Copyright (C) 2016 Ivan Cukic <ivan.cukic(at)kde.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QtTest/QtTest>
#include <QCoreApplication>
#include <QList>

#include "common.h"

#include "wrappers/qprocess_test.h"
#include "wrappers/qdbus_test.h"
#include "wrappers/basic_test.h"

#include "base/transform_test.h"
#include "base/flatten_test.h"
#include "base/continuewith_test.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    RUN_TEST(wrappers::ProcessExecutionTest);
    RUN_TEST(wrappers::DBusExecutionTest);
    RUN_TEST(wrappers::BasicFuturesTest);

    RUN_TEST(base::TransformTest);
    RUN_TEST(base::FlattenTest);
    RUN_TEST(base::ContinueWithTest);

    return 1;
}

