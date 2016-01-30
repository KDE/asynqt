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

#include "cast_test.h"

#include <wrappers/process.h>
#include <operations/cast.h>

#include <QFuture>
#include <QCoreApplication>
#include <QtTest>

#include "common.h"

namespace base {

CastTest::CastTest()
{
}

void CastTest::testCastBytesToString()
{
    auto future = AsynQt::Process::getOutput("echo", { "Hello KDE" });

    auto castFuture = AsynQt::qfuture_cast<QString>(future);

    COMPARE_FINISHED_BEFORE(castFuture, QString("Hello KDE\n"), 1 _seconds);
    VERIFY_TYPE(future, QFuture<QByteArray>);
    VERIFY_TYPE(castFuture, QFuture<QString>);
}

void CastTest::testCastBytesToStringWithPipeSyntax()
{
    using namespace operators;

    TEST_CHUNK("With temporary future") {
        auto future = AsynQt::Process::getOutput("echo", { "Hello KDE" });

        auto castFuture = future | cast<QString>();

        COMPARE_FINISHED_BEFORE(castFuture, QString("Hello KDE\n"), 1 _seconds);
        VERIFY_TYPE(future, QFuture<QByteArray>);
        VERIFY_TYPE(castFuture, QFuture<QString>);
    }

    TEST_CHUNK("Without temporary future") {
        auto castFuture = AsynQt::Process::getOutput("echo", { "Hello KDE" })
                          | cast<QString>();

        COMPARE_FINISHED_BEFORE(castFuture, QString("Hello KDE\n"), 1 _seconds);
        VERIFY_TYPE(castFuture, QFuture<QString>);
    }
}



void CastTest::initTestCase()
{
}

void CastTest::cleanupTestCase()
{
}

} // namespace base

