/************************************************************************

    hierarchy.h

    OpenAIV - Domesday Retrieval application
    Copyright (C) 2020 Simon Inns

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

#ifndef HIERARCHY_H
#define HIERARCHY_H

#include <QObject>
#include <QVector>
#include <QDebug>

// See: https://github.com/simoninns/OpenAIV/wiki/National-Hierarchy-file

class Hierarchy
{
public:
    Hierarchy() = default;
    ~Hierarchy() = default;
    Hierarchy(const Hierarchy &) = default;
    Hierarchy &operator=(const Hierarchy &) = default;

    Hierarchy(const quint32 &index, const quint32 &father, const qint32 &text,
              const bool &bottomFlag, const qint32 &level, const QString &title,
              const QVector<quint32> &hdps, const QVector<quint32> &xrefs);

    quint32 index() const;
    bool bottomFlag() const;
    QString title() const;
    QVector<quint32> hdps() const;
    QVector<quint32> xrefs() const;

private:
    quint32 m_index;
    quint32 m_father;
    qint32 m_text;
    bool m_bottomFlag;
    qint32 m_level;
    QString m_title;
    QVector<quint32> m_hdps;
    QVector<quint32> m_xrefs;
};

// Custom streaming operator
QDebug operator<<(QDebug dbg, const Hierarchy &hierarchy);

// Custom meta-type declaration
Q_DECLARE_METATYPE(Hierarchy);

#endif // HIERARCHY_H
