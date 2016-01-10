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

#include "flatten_test.h"

#include <wrappers/process.h>
#include <base/flatten.h>

#include <QFuture>
#include <QCoreApplication>
#include <QtTest>

#include "common.h"

namespace base {

FlattenTest::FlattenTest()
{
}

QFuture<QString> execEcho(const QString &message)
{
    return AsynQt::Process::getOutput("echo", { message.trimmed() });
}

void FlattenTest::testFlatten()
{
    auto future = AsynQt::Process::exec(
        "echo", { "Hello KDE!" },
        [] (QProcess *process) {
            return execEcho(QString::fromLatin1(process->readAllStandardOutput()));
        });

    auto flattenFuture = AsynQt::flatten(future);

    QCOMPAREAFTER(flattenFuture, QString("Hello KDE!\n"), 1 _seconds);

    QVERIFYTYPE(future, QFuture<QFuture<QString>>);
    QVERIFYTYPE(flattenFuture, QFuture<QString>);
}

void FlattenTest::initTestCase()
{
}

void FlattenTest::cleanupTestCase()
{
}

} // namespace base

