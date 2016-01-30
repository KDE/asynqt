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

#include "qdbus_test.h"

#include <wrappers/dbus.h>

#include <QFuture>
#include <QCoreApplication>
#include <QDBusInterface>
#include <QDBusConnection>

#include <QtTest>

#include "common.h"

namespace wrappers {

DBusExecutionTest::DBusExecutionTest()
{
}

void DBusExecutionTest::testDBusExecution()
{
    auto future = AsynQt::DBus::asyncCall<QString>(m_dbus, "GetId");

    QVERIFY(waitForFuture(future, 1 _seconds));

    auto result = future.result();

    QCOMPARE(result.size(),
             QString("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx").size());

    VERIFY_TYPE(future, QFuture<QString>);
}

void DBusExecutionTest::testDBusExecutionWithArgument()
{
    auto future = AsynQt::DBus::asyncCall<QString>(
        m_dbus, "GetNameOwner", "org.freedesktop.DBus");

    COMPARE_FINISHED_BEFORE(future, QStringLiteral("org.freedesktop.DBus"), 1 _seconds);
    VERIFY_TYPE(future, QFuture<QString>);
}

void DBusExecutionTest::testDBusExecutionError()
{
    auto future = AsynQt::DBus::asyncCall<QString>(
        m_dbus, "ThisMethodDoesNotExist_Really_Really");

    VERIFY_CANCELED_BEFORE(future, 1 _seconds);
    VERIFY_TYPE(future, QFuture<QString>);
}


void DBusExecutionTest::initTestCase()
{
    m_dbus = new QDBusInterface("org.freedesktop.DBus", "/");
}

void DBusExecutionTest::cleanupTestCase()
{
    delete m_dbus;
}

} // namespace wrappers

