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

    COMPARE_AFTER(transformedFuture, 11, 1 _seconds);
    VERIFY_TYPE(future, QFuture<QByteArray>);
    VERIFY_TYPE(transformedFuture, QFuture<int>);
}

void TransformTest::testTransformWithFunctionObjects()
{
    auto future = common::execHelloKde();

    auto transformedFuture = AsynQt::transform(future, SizeMultiplier(2));

    COMPARE_AFTER(transformedFuture, 22, 1 _seconds);
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

    COMPARE_AFTER(transformedFuture, 11, 1 _seconds);
    VERIFY_TYPE(future, QFuture<QByteArray>);
    VERIFY_TYPE(transformedFuture, QFuture<int>);
}

void TransformTest::testTransformWithCanceledFutures()
{
    auto future = common::fail(QString());

    auto transformedFuture = AsynQt::transform(future,
        [] (const QString &input) {
            qDebug() << "Result: " << input;
            return input.size();
        });

    VERIFY_CANCELED_AFTER(transformedFuture, 100 _milliseconds);
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

    COMPARE_AFTER(transformedFuture, 11, 100 _milliseconds);
    VERIFY_TYPE(future, QFuture<QByteArray>);
    VERIFY_TYPE(transformedFuture, QFuture<int>);
}

void TransformTest::testTransformVoidFuture()
{
    QFuture<void> future = makeCanceledFuture();

    // This should report a compilation error
    // auto transformedFuture = AsynQt::transform(future,
    //     [] () {
    //     });
}

void TransformTest::initTestCase()
{
}

void TransformTest::cleanupTestCase()
{
}

} // namespace base

