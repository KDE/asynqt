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

    COMPARE_AFTER(future, QStringLiteral("org.freedesktop.DBus"), 1 _seconds);
    VERIFY_TYPE(future, QFuture<QString>);
}

void DBusExecutionTest::testDBusExecutionError()
{
    auto future = AsynQt::DBus::asyncCall<QString>(
        m_dbus, "ThisMethodDoesNotExist_Really_Really");

    VERIFY_CANCELED_AFTER(future, 1 _seconds);
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

