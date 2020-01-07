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

// See: https://github.com/simoninns/OpenAIV/wiki/National-Names-file

class Names
{
public:
    Names() = default;
    ~Names() = default;
    Names(const Names &) = default;
    Names &operator=(const Names &) = default;

    Names(const QString &itemName, const qint32 &itemType, const quint32 &itemAddress);

    QString itemName() const;
    QString itemTypeAndItemName() const;
    qint32 itemType() const;
    QString itemTypeDescription() const;
    quint32 itemAddress() const;

private:
    QString m_itemName;
    qint32 m_itemType;
    quint32 m_itemAddress;
};

// Custom streaming operator
QDebug operator<<(QDebug dbg, const Names &names);

// Custom meta-type declaration
Q_DECLARE_METATYPE(Names);

#endif // NAMES_H
