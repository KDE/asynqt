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

#include "continuewith_test.h"

#include <wrappers/process.h>
#include <base/continuewith.h>

#include <QFuture>
#include <QCoreApplication>
#include <QtTest>

#include "common.h"

using namespace AsynQt;

namespace base {

ContinueWithTest::ContinueWithTest()
{
}

namespace {
    QByteArray helloKdeMessage = "Hello KDE!\n";

    QFuture<QByteArray> execHelloKde()
    {
        return Process::getOutput("echo", { helloKdeMessage.trimmed() });
    }

    QFuture<QByteArray> execEcho(const QString &message)
    {
        return Process::getOutput("echo", { message.trimmed() });
    }
}

void ContinueWithTest::testContinueWith()
{
    {
        auto future = execHelloKde();
        auto connectedFuture = continueWith(future, execEcho);

        QCOMPAREAFTER(connectedFuture, helloKdeMessage, 1 _seconds);
        QVERIFYTYPE(future, QFuture<QByteArray>);
        QVERIFYTYPE(connectedFuture, QFuture<QByteArray>);
    }

    {
        auto connectedFuture =
            continueWith(execHelloKde(), execEcho);

        QCOMPAREAFTER(connectedFuture, helloKdeMessage, 1 _seconds);
        QVERIFYTYPE(connectedFuture, QFuture<QByteArray>);
    }
}

void ContinueWithTest::testContinueWithOperator()
{
    using namespace AsynQt::operators;

    {
        auto future = execHelloKde();
        auto connectedFuture = future | execEcho;

        QCOMPAREAFTER(connectedFuture, helloKdeMessage, 1 _seconds);
        QVERIFYTYPE(future, QFuture<QByteArray>);
        QVERIFYTYPE(connectedFuture, QFuture<QByteArray>);
    }

    {
        auto connectedFuture = execHelloKde() | execEcho;

        QCOMPAREAFTER(connectedFuture, helloKdeMessage, 1 _seconds);
        QVERIFYTYPE(connectedFuture, QFuture<QByteArray>);
    }
}

void ContinueWithTest::initTestCase()
{
}

void ContinueWithTest::cleanupTestCase()
{
}

} // namespace base

