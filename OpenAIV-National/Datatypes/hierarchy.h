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

class Hierarchy
{
public:
    Hierarchy() = default;
    ~Hierarchy() = default;
    Hierarchy(const Hierarchy &) = default;
    Hierarchy &operator=(const Hierarchy &) = default;

    Hierarchy(const qint32 &index, const qint32 &fatherRecord, const qint32 &essayAddress,
              const bool &bottomFlag, const qint32 &level, const QString &textLabel,
              const QVector<qint32> &descPointers, const QVector<qint32> &crossRefs);

    qint32 index() const;
    bool bottomFlag() const;
    QString textLabel() const;
    QVector<qint32> descPointers() const;
    QVector<qint32> crossRefs() const;

private:
    qint32 m_index;
    qint32 m_fatherRecord;
    qint32 m_essayAddress;
    bool m_bottomFlag;
    qint32 m_level;
    QString m_textLabel;
    QVector<qint32> m_descPointers;
    QVector<qint32> m_crossRefs;
};

// Custom streaming operator
QDebug operator<<(QDebug dbg, const Hierarchy &hierarchy);

// Custom meta-type declaration
Q_DECLARE_METATYPE(Hierarchy);

#endif // HIERARCHY_H
