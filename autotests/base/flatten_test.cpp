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
#include <operations/flatten.h>

#include <QFuture>
#include <QCoreApplication>
#include <QtTest>

#include "common.h"

namespace base {

namespace {
    QFuture<QByteArray> passOutputToEcho(QProcess *process)
    {
        return common::execEcho(QString::fromLatin1(process->readAllStandardOutput()));
    };

    class PassOutputToEcho {
    public:
        QFuture<QByteArray> operator () (QProcess *process)
        {
            return passOutputToEcho(process);
        }
    };
}

FlattenTest::FlattenTest()
{
}

void FlattenTest::testFlatten()
{
    auto future = AsynQt::Process::exec(
        "echo", { "Hello KDE!" },
        [] (QProcess *process) {
            return common::execEcho(QString::fromLatin1(process->readAllStandardOutput()));
        });

    auto flattenFuture = AsynQt::flatten(future);

    COMPARE_FINISHED_BEFORE(flattenFuture, QByteArray("Hello KDE!\n"), 1 _seconds);

    VERIFY_TYPE(future, QFuture<QFuture<QByteArray>>);
    VERIFY_TYPE(flattenFuture, QFuture<QByteArray>);
}

void FlattenTest::testFlattenWithPipeSyntax()
{
    using namespace AsynQt::operators;

    TEST_CHUNK("Pipe with lambda") {
        auto future = AsynQt::Process::exec(
            "echo", { "Hello KDE!" },
            [] (QProcess *process) {
                return common::execEcho(QString::fromLatin1(process->readAllStandardOutput()));
            });

        auto flattenFuture = future | flatten();

        COMPARE_FINISHED_BEFORE(flattenFuture, QByteArray("Hello KDE!\n"), 1 _seconds);

        VERIFY_TYPE(future, QFuture<QFuture<QByteArray>>);
        VERIFY_TYPE(flattenFuture, QFuture<QByteArray>);
    }

    TEST_CHUNK("Pipe with function, no temporary future") {
        auto flattenFuture
            = AsynQt::Process::exec("echo", { "Hello KDE!" }, passOutputToEcho)
              | flatten();

        COMPARE_FINISHED_BEFORE(flattenFuture, QByteArray("Hello KDE!\n"), 1 _seconds);

        VERIFY_TYPE(flattenFuture, QFuture<QByteArray>);
    }

    TEST_CHUNK("Pipe with functional object, no temporary future") {
        auto flattenFuture
            = AsynQt::Process::exec("echo", { "Hello KDE!" }, PassOutputToEcho())
              | flatten();

        COMPARE_FINISHED_BEFORE(flattenFuture, QByteArray("Hello KDE!\n"), 1 _seconds);

        VERIFY_TYPE(flattenFuture, QFuture<QByteArray>);
    }
}

void FlattenTest::testFlattenWithFailures()
{
    TEST_CHUNK("Failure in the inner future")
    {
        auto future = AsynQt::Process::exec(
            "echo", { "Hello KDE!" },
            [] (QProcess *process) {
                return common::fail(QString::fromLatin1(process->readAllStandardOutput()));
            });

        auto flattenFuture = AsynQt::flatten(future);

        VERIFY_CANCELED_AROUND(flattenFuture, 1 _seconds);

        VERIFY_TYPE(future, QFuture<QFuture<QString>>);
        VERIFY_TYPE(flattenFuture, QFuture<QString>);
    }

    TEST_CHUNK("Failure in the outer future")
    {
        auto future = AsynQt::makeCanceledFuture<QFuture<QString>>();

        auto flattenFuture = AsynQt::flatten(future);

        VERIFY_CANCELED_AROUND(flattenFuture, 1 _seconds);

        VERIFY_TYPE(future, QFuture<QFuture<QString>>);
        VERIFY_TYPE(flattenFuture, QFuture<QString>);
    }
}

void FlattenTest::testFlattenVoid()
{
    const auto delay = 1 _seconds;

    auto future = AsynQt::makeDelayedFuture(
                    AsynQt::makeDelayedFuture(1 _seconds),
                    1 _seconds);

    auto flattenFuture = AsynQt::flatten(future);

    // These futures are running in-parallel, they will both
    // finish in 1 second
    VERIFY_FINISHED_AROUND(flattenFuture, delay);

    VERIFY_TYPE(future, QFuture<QFuture<void>>);
    VERIFY_TYPE(flattenFuture, QFuture<void>);
}

void FlattenTest::testFlattenVoidWithFailures()
{
    TEST_CHUNK("Failure in the inner future")
    {
        const auto delay = 1 _seconds;

        auto future = AsynQt::makeDelayedFuture(
                        AsynQt::makeCanceledFuture(),
                        delay);

        auto flattenFuture = AsynQt::flatten(future);

        // These futures are running in-parallel, they will both
        // finish in 1 second
        VERIFY_CANCELED_AROUND(flattenFuture, delay);

        VERIFY_TYPE(future, QFuture<QFuture<void>>);
        VERIFY_TYPE(flattenFuture, QFuture<void>);
    }
}

void FlattenTest::initTestCase()
{
}

void FlattenTest::cleanupTestCase()
{
}

} // namespace base

