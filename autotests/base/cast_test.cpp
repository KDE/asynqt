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



void CastTest::initTestCase()
{
}

void CastTest::cleanupTestCase()
{
}

} // namespace base

