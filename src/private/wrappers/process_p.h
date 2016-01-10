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

//
// W A R N I N G
// -------------
//
// This file is not part of the AsynQt API. It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

namespace AsynQt {
namespace detail {

template <typename _Result, typename _Function>
class ProcessFutureInterface : public QObject,
                               public QFutureInterface<_Result> {

public:
    ProcessFutureInterface(QProcess *process, _Function map)
        : m_process(process)
        , m_map(map)
    {
    }

    void error()
    {
        this->reportCanceled();
    }

    QFuture<_Result> start()
    {
        QObject::connect(
                m_process,
                // Pretty new Qt connect syntax :)
                (void (QProcess::*)(QProcess::ProcessError)) &QProcess::error,
                this, [this] (QProcess::ProcessError) {
                    this->error();
                });

        QObject::connect(
                m_process,
                // Pretty new Qt connect syntax :)
                (void (QProcess::*)(int, QProcess::ExitStatus)) &QProcess::finished,
                this, [this] (int, QProcess::ExitStatus) {
                    this->finished();
                });

        this->reportStarted();

        m_process->start();

        return this->future();
    }

    void finished();


private:
    QProcess *m_process;
    _Function m_map;

};

template <typename _Result, typename _Function>
void ProcessFutureInterface<_Result, _Function>::finished()
{
    if (m_process->exitCode() == 0
            && m_process->exitStatus() == QProcess::NormalExit) {

        this->reportResult(m_map(m_process));
        this->reportFinished();

    } else {
        this->reportCanceled();
    }
}

} // namespace detail
} // namespace AsynQt

