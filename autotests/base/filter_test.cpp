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

#include "filter_test.h"

#include <wrappers/process.h>
#include <operations/filter.h>

#include <QFuture>
#include <QCoreApplication>
#include <QtTest>

#include "common.h"

namespace base {

namespace {
    bool falsePredicate(const QString &message)
    {
        return false;
    }

    struct BoolPredicate {
        BoolPredicate(bool result)
            : m_result(result)
        {
        }

        int operator () (const QString &)
        {
            return m_result;
        }

        int operator () (const QByteArray &)
        {
            return m_result;
        }

    private:
        bool m_result;
    };
}

FilterTest::FilterTest()
{
}

void FilterTest::testFilterWithFunctions()
{
    TEST_CHUNK("Filtered out") {
        auto future = common::execHelloKde();

        auto filteredFuture
            = AsynQt::filter(future, falsePredicate);

        VERIFY_FINISHED_BEFORE(filteredFuture, 1 _seconds);
        QVERIFY(filteredFuture.resultCount() == 0);
        VERIFY_TYPE(future, QFuture<QByteArray>);
        VERIFY_TYPE(filteredFuture, QFuture<QByteArray>);
    }
}

void FilterTest::testFilterWithFunctionObjects()
{
    TEST_CHUNK("Filtered out") {
        auto future = common::execHelloKde();

        auto filteredFuture
            = AsynQt::filter(future, BoolPredicate(false));

        VERIFY_FINISHED_BEFORE(filteredFuture, 1 _seconds);
        QVERIFY(filteredFuture.resultCount() == 0);
        VERIFY_TYPE(future, QFuture<QByteArray>);
        VERIFY_TYPE(filteredFuture, QFuture<QByteArray>);
    }

    TEST_CHUNK("Not filtered out") {
        auto future = common::execHelloKde();

        auto filteredFuture
            = AsynQt::filter(future, BoolPredicate(true));

        COMPARE_FINISHED_BEFORE(filteredFuture, common::helloKdeMessage(),
                                1 _seconds);
        VERIFY_TYPE(future, QFuture<QByteArray>);
        VERIFY_TYPE(filteredFuture, QFuture<QByteArray>);
    }
}

void FilterTest::testFilterWithLambdas()
{
    TEST_CHUNK("Filtered out") {
        auto future = common::execHelloKde();

        auto filteredFuture
            = AsynQt::filter(future, [](const QString &) { return false; });

        VERIFY_FINISHED_BEFORE(filteredFuture, 1 _seconds);
        QVERIFY(filteredFuture.resultCount() == 0);
        VERIFY_TYPE(future, QFuture<QByteArray>);
        VERIFY_TYPE(filteredFuture, QFuture<QByteArray>);
    }

    TEST_CHUNK("Not filtered out") {
        auto future = common::execHelloKde();

        auto filteredFuture
            = AsynQt::filter(future, [](const QString &) { return true; });

        COMPARE_FINISHED_BEFORE(filteredFuture, common::helloKdeMessage(),
                                1 _seconds);
        VERIFY_TYPE(future, QFuture<QByteArray>);
        VERIFY_TYPE(filteredFuture, QFuture<QByteArray>);
    }
}

void FilterTest::testFilterWithPipeSyntax()
{
    using namespace operators;

    TEST_CHUNK("Filtered out") {
        auto filteredFuture = common::execHelloKde()
                              | filter([](const QString &) { return false; });

        VERIFY_FINISHED_BEFORE(filteredFuture, 1 _seconds);
        QVERIFY(filteredFuture.resultCount() == 0);
        VERIFY_TYPE(filteredFuture, QFuture<QByteArray>);
    }

    TEST_CHUNK("Not filtered out") {
        auto filteredFuture = common::execHelloKde()
                              | filter([](const QString &) { return true; });

        COMPARE_FINISHED_BEFORE(filteredFuture, common::helloKdeMessage(),
                                1 _seconds);
        VERIFY_TYPE(filteredFuture, QFuture<QByteArray>);
    }
}

void FilterTest::testFilterWithCanceledFutures()
{
    auto future = common::fail(QString());

    auto filteredFuture = AsynQt::filter(future, falsePredicate);

    VERIFY_CANCELED_AROUND(filteredFuture, 100 _milliseconds);
    VERIFY_TYPE(future, QFuture<QString>);
    VERIFY_TYPE(filteredFuture, QFuture<QString>);
}

void FilterTest::testFilterWithReadyFutures()
{
    auto future = makeReadyFuture(common::helloKdeMessage());

    auto filteredFuture = AsynQt::filter(future, falsePredicate);

    VERIFY_FINISHED_AROUND(filteredFuture, 100 _milliseconds);
    QVERIFY(filteredFuture.resultCount() == 0);
    VERIFY_TYPE(future, QFuture<QByteArray>);
    VERIFY_TYPE(filteredFuture, QFuture<QByteArray>);
}

void FilterTest::initTestCase()
{
}

void FilterTest::cleanupTestCase()
{
}

} // namespace base

