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

#include "transform_test.h"

#include <wrappers/process.h>
#include <operations/transform.h>

#include <QFuture>
#include <QCoreApplication>
#include <QtTest>

#include "common.h"

namespace base {

namespace {
    int stringSize(const QString &input)
    {
        return input.size();
    }

    struct SizeMultiplier {
        SizeMultiplier(int coef)
            : m_coef(coef)
        {
        }

        int operator () (const QString &input)
        {
            return m_coef * input.size();
        }

        int operator () (const QByteArray &input)
        {
            return m_coef * input.size();
        }

    private:
        int m_coef;
    };
}

TransformTest::TransformTest()
{
}

void TransformTest::testTransformWithFunctions()
{
    auto future = common::execHelloKde();

    auto transformedFuture = AsynQt::transform(future, stringSize);

    COMPARE_FINISHED_BEFORE(transformedFuture, 11, 1 _seconds);
    VERIFY_TYPE(future, QFuture<QByteArray>);
    VERIFY_TYPE(transformedFuture, QFuture<int>);
}

void TransformTest::testTransformWithFunctionObjects()
{
    auto future = common::execHelloKde();

    auto transformedFuture = AsynQt::transform(future, SizeMultiplier(2));

    COMPARE_FINISHED_BEFORE(transformedFuture, 22, 1 _seconds);
    VERIFY_TYPE(future, QFuture<QByteArray>);
    VERIFY_TYPE(transformedFuture, QFuture<int>);
}

void TransformTest::testTransformWithLambdas()
{
    auto future = common::execHelloKde();

    auto transformedFuture = AsynQt::transform(future,
        [] (const QString &input) {
            qDebug() << "Result: " << input;
            return input.size();
        });

    COMPARE_FINISHED_BEFORE(transformedFuture, 11, 1 _seconds);
    VERIFY_TYPE(future, QFuture<QByteArray>);
    VERIFY_TYPE(transformedFuture, QFuture<int>);
}

void TransformTest::testTransformWithPipeSyntax()
{
    using namespace AsynQt::operators;

    TEST_CHUNK("Pipe with lambda") {
        auto future = common::execHelloKde();

        auto transformedFuture = future | transform(
            [] (const QString &input) {
                qDebug() << "Result: " << input;
                return input.size();
            });

        COMPARE_FINISHED_BEFORE(transformedFuture, 11, 1 _seconds);
        VERIFY_TYPE(future, QFuture<QByteArray>);
        VERIFY_TYPE(transformedFuture, QFuture<int>);
    }

    TEST_CHUNK("Pipe with functional object, no temporary future") {
        auto transformedFuture =
            common::execHelloKde() | transform(SizeMultiplier(2));

        COMPARE_FINISHED_BEFORE(transformedFuture, 22, 1 _seconds);
        VERIFY_TYPE(transformedFuture, QFuture<int>);
    }
}

void TransformTest::testTransformWithCanceledFutures()
{
    auto future = common::fail(QString());

    auto transformedFuture = AsynQt::transform(future,
        [] (const QString &input) {
            qDebug() << "Result: " << input;
            return input.size();
        });

    VERIFY_CANCELED_AROUND(transformedFuture, 100 _milliseconds);
    VERIFY_TYPE(future, QFuture<QString>);
    VERIFY_TYPE(transformedFuture, QFuture<int>);
}

void TransformTest::testTransformWithReadyFutures()
{
    auto future = makeReadyFuture(common::helloKdeMessage());

    auto transformedFuture = AsynQt::transform(future,
        [] (const QString &input) {
            qDebug() << "Result: " << input;
            return input.size();
        });

    COMPARE_FINISHED_AROUND(transformedFuture, 11, 100 _milliseconds);
    VERIFY_TYPE(future, QFuture<QByteArray>);
    VERIFY_TYPE(transformedFuture, QFuture<int>);
}

void TransformTest::testTransformVoidToValueFuture()
{
    const auto delay = 1 _seconds;
    auto future = makeDelayedFuture(delay);

    auto transformedFuture = AsynQt::transform(future,
        [] () {
            return 42;
        });

    COMPARE_FINISHED_AROUND(transformedFuture, 42, delay);
    VERIFY_TYPE(future, QFuture<void>);
    VERIFY_TYPE(transformedFuture, QFuture<int>);
}

void TransformTest::testTransformValueToVoidFuture()
{
    const auto delay = 1 _seconds;
    auto future = makeDelayedFuture(42, delay);

    auto transformedFuture = AsynQt::transform(future,
        [] (int) {
        });

    VERIFY_FINISHED_AROUND(transformedFuture, delay);
    VERIFY_TYPE(future, QFuture<int>);
    VERIFY_TYPE(transformedFuture, QFuture<void>);
}

void TransformTest::testTransformVoidToVoidFuture()
{
    bool done = false;
    const auto delay = 1 _seconds;
    auto future = makeDelayedFuture(delay);

    auto transformedFuture = AsynQt::transform(future,
        [&done] () {
            done = true;
        });

    VERIFY_FINISHED_AROUND(transformedFuture, delay);
    VERIFY_TYPE(future, QFuture<void>);
    VERIFY_TYPE(transformedFuture, QFuture<void>);
    QVERIFY(done);
}

void TransformTest::initTestCase()
{
}

void TransformTest::cleanupTestCase()
{
}

} // namespace base

