/************************************************************************

    walk.h

    OpenAIV-National - Domesday Retrieval application
    Copyright (C) 2020-2022 Simon Inns

    This file is part of OpenAIV.

    This application is free software: you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

************************************************************************/

#ifndef WALK_H
#define WALK_H

#include <QObject>
#include <QVector>
#include <QDebug>

// See: https://github.com/simoninns/OpenAIV/wiki/National-Data-file-walk

class Walk
{
public:
    Walk() = default;
    ~Walk() = default;
    Walk(const Walk &) = default;
    Walk &operator=(const Walk &) = default;

    Walk(const quint32 &baseview, const quint32 &baseplan,
          const quint32 &syslev, const QByteArray &linkTable,
         const QByteArray &controlTable, const QByteArray &planTable,
         const QByteArray &detailTable);

    quint32 baseview() const;
    quint32 baseplan() const;
    quint32 syslev() const;
    QByteArray linkTable() const;
    QByteArray controlTable() const;
    QByteArray planTable() const;
    QByteArray detailTable() const;

private:
    quint32 m_baseview;
    quint32 m_baseplan;
    quint32 m_syslev;
    QByteArray m_linkTable;
    QByteArray m_controlTable;
    QByteArray m_planTable;
    QByteArray m_detailTable;
};

// Custom streaming operator
QDebug operator<<(QDebug dbg, const Walk &walk);

// Custom meta-type declaration
Q_DECLARE_METATYPE(Walk);

#endif // WALK_H
