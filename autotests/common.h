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

#ifndef TESTS_COMMON_H
#define TESTS_COMMON_H

#include <QElapsedTimer>
#include <QCoreApplication>

#define RUN_TEST(Type)                                                         \
    if (QCoreApplication::arguments().isEmpty()                                \
            || QCoreApplication::arguments().contains(#Type)) {                \
        Type test;                                                             \
        QTest::qExec(&test);                                                   \
    } else {                                                                   \
        qDebug() << "Test" << #Type << "is \033[1;33mdisabled\033[0m";         \
    }

// We can not use user-defined literals yet, but we want to make this
// prettier

#define _seconds

template <typename Future>
inline bool waitForFuture(Future f, int seconds)
{
    QElapsedTimer timer;
    timer.start();

    while (timer.elapsed() < seconds * 1000 && !f.isFinished()) {
        QCoreApplication::processEvents();
    }

    return f.isFinished();
}


#endif // TESTS_COMMON_H


