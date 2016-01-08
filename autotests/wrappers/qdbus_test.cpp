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

    qDebug() << "DBus returned " << result << " for the id";

    QCOMPARE(result.length(),
             QString("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx").length());
}

void DBusExecutionTest::testDBusExecutionWithArgument()
{
    auto future = AsynQt::DBus::asyncCall<QString>(m_dbus, "GetNameOwner",
                                                   "org.freedesktop.DBus");

    QVERIFY(waitForFuture(future, 1 _seconds));

    auto result = future.result();

    qDebug() << "Owner of org.freedesktop.DBus is " << result;

    QCOMPARE(future.result(), QStringLiteral("org.freedesktop.DBus"));
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

