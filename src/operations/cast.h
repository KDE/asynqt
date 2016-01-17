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

#ifndef ASYNQT_BASE_CAST_H
#define ASYNQT_BASE_CAST_H

#include <QFuture>
#include <QFutureWatcher>

#include <type_traits>
#include <memory>

#include "transform.h"

namespace AsynQt {

/**
 */
template <typename _Out, typename _In>
QFuture<_Out> qfuture_cast(const QFuture<_In> &future)
{
    return transform(future, [] (const _In &value) -> _Out {
            return static_cast<_Out>(value);
        });
}

} // namespace AsynQt

#endif // ASYNQT_BASE_CAST_H

