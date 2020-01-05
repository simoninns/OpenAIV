/************************************************************************

    names.h

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

#ifndef NAMES_H
#define NAMES_H

#include <QObject>
#include <QVector>
#include <QDebug>

class Names
{
public:
    Names() = default;
    ~Names() = default;
    Names(const Names &) = default;
    Names &operator=(const Names &) = default;

    Names(const QString &label, const qint32 &type, const qint32 &destinationOffset);

    QString label() const;
    QString typeAndLabel() const;
    qint32 type() const;
    QString typeName() const;
    qint32 destinationOffset() const;

private:
    QString m_label;
    qint32 m_type;
    qint32 m_destinationOffset;
};

// Custom streaming operator
QDebug operator<<(QDebug dbg, const Names &names);

// Custom meta-type declaration
Q_DECLARE_METATYPE(Names);

#endif // NAMES_H
