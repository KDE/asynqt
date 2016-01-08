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

#include "qprocess_test.h"

#include <wrappers/process.h>

#include <QFuture>
#include <QCoreApplication>
#include <QtTest>

#include "common.h"

namespace wrappers {

ProcessExecutionTest::ProcessExecutionTest()
{
}

void ProcessExecutionTest::testProcessExecution()
{
    auto future = AsynQt::Process::exec("sleep", { "2" });

    QVERIFY(waitForFuture(future, 3 _seconds));

    auto process = future.result();

    QCOMPARE(process->program(), QString("sleep"));
}

void ProcessExecutionTest::testProcessExecutionWithMap()
{
    bool mapFunctionCalled = false;

    auto future = AsynQt::Process::exec(
        "sleep", { "2" },
        [&mapFunctionCalled] (QProcess *process) {
            mapFunctionCalled = true;

            return process->exitCode();
        });

    QVERIFY(waitForFuture(future, 3 _seconds));

    QVERIFY(mapFunctionCalled);

    QCOMPARE(future.result(), 0);
}

void ProcessExecutionTest::testProcessOutput()
{
    auto future = AsynQt::Process::getOutput("echo", { "Hello KDE" });

    QVERIFY(waitForFuture(future, 1 _seconds));

    QCOMPARE(future.result(), QString("Hello KDE\n"));

}

void ProcessExecutionTest::initTestCase()
{
    m_process = new QProcess();
}

void ProcessExecutionTest::cleanupTestCase()
{
    delete m_process;
}

} // namespace wrappers
