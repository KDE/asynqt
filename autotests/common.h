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

#ifndef TESTS_COMMON_H
#define TESTS_COMMON_H

#include <QElapsedTimer>
#include <QCoreApplication>

#include <type_traits>

#include "basic/all.h"
#include "wrappers/process.h"

#define RUN_TEST(Type)                                                         \
    if (QCoreApplication::arguments().size() < 2                               \
            || QCoreApplication::arguments().contains(#Type)) {                \
        Type test;                                                             \
        QTest::qExec(&test);                                                   \
    } else {                                                                   \
        qDebug() << "Test" << #Type << "is \033[1;33mdisabled\033[0m";         \
    }

// We can not use user-defined literals yet, but we want to make this
// prettier

#define _milliseconds
#define _seconds * 1000

template <typename Future>
inline bool waitForFuture(Future f, int milliseconds)
{
    QElapsedTimer timer;
    timer.start();

    if (milliseconds < 0) milliseconds = 0;

    milliseconds += 100 _milliseconds;

    while (timer.elapsed() < milliseconds && !f.isFinished()) {
        QCoreApplication::processEvents();
    }

    return f.isFinished();
}

template <typename Future>
inline bool waitForFuture(Future f, int minMilliseconds, int maxMilliseconds)
{
    QElapsedTimer timer;
    timer.start();

    while (timer.elapsed() < maxMilliseconds && !f.isFinished() && !f.isCanceled()) {
        QCoreApplication::processEvents();
    }

    if (timer.elapsed() < minMilliseconds) {
        qWarning() << "Future came earlier than it was supposed to";
        return false;
    }

    return f.isFinished();
}

#define VERIFY_FINISHED_AROUND(Future, Time)                                   \
    QVERIFY(waitForFuture(Future, Time - 200, Time + 200));                    \
    QVERIFY(!Future.isCanceled())

#define COMPARE_FINISHED_AROUND(Future, Value, Time)                           \
    QVERIFY(waitForFuture(Future, Time - 200, Time + 200));                    \
    QVERIFY(!Future.isCanceled());                                             \
    QCOMPARE(Future.result(), Value)

#define VERIFY_CANCELED_AROUND(Future, Time)                                   \
    waitForFuture(Future, Time - 200, Time + 200);                             \
    QVERIFY(Future.isCanceled())

#define VERIFY_EXCEPTION_AROUND(Future, Exception, Time)                       \
    waitForFuture(Future, Time - 200, Time + 200);                             \
    QVERIFY(Future.isCanceled());                                              \
    {                                                                          \
        bool exceptionCaught = false;                                          \
        try {                                                                  \
            Future.waitForFinished();                                          \
        } catch (const Exception &e) {                                         \
            exceptionCaught = true;                                            \
        }                                                                      \
        QVERIFY(exceptionCaught);                                              \
    }

#define VERIFY_FINISHED_BEFORE(Future, Time)                                   \
    QVERIFY(waitForFuture(Future, Time + 200));                                \
    QVERIFY(!Future.isCanceled())

#define COMPARE_FINISHED_BEFORE(Future, Value, Time)                           \
    QVERIFY(waitForFuture(Future, Time + 200));                                \
    QVERIFY(!Future.isCanceled());                                             \
    QCOMPARE(Future.result(), Value)

#define VERIFY_CANCELED_BEFORE(Future, Time)                                   \
    waitForFuture(Future, Time + 200);                                         \
    QVERIFY(Future.isCanceled())

#define VERIFY_EXCEPTION_BEFORE(Future, Exception, Time)                       \
    waitForFuture(Future, Time + 200);                                         \
    QVERIFY(Future.isCanceled());                                              \
    {                                                                          \
        bool exceptionCaught = false;                                          \
        try {                                                                  \
            Future.waitForFinished();                                          \
        } catch (const Exception &e) {                                         \
            exceptionCaught = true;                                            \
        }                                                                      \
        QVERIFY(exceptionCaught);                                              \
    }



#define VERIFY_TYPE(Variable, Type)                                            \
    QVERIFY((std::is_same<decltype(Variable), Type>::value))

#define TEST_CHUNK(Name) qDebug() << "Test: " << Name;

namespace {
    template <typename T>
    struct debug_type;
}

#define DEBUG_TYPE(Variable) { debug_type<decltype(Variable)> error; }

using namespace AsynQt;

namespace common {
    QByteArray helloKdeMessage();
    QFuture<QByteArray> execHelloKde();
    QFuture<QByteArray> execEcho(const QString &message);
    QFuture<QString> fail(const QString &message);
}

#endif // TESTS_COMMON_H

