/************************************************************************

    nationalitem.h

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

#ifndef NATIONALITEM_H
#define NATIONALITEM_H

#include <QObject>
#include <QVariant>
#include <QVector>

class NationalItem
{
public:
    explicit NationalItem(const QVector<QVariant> &data, NationalItem *parent = nullptr);
    ~NationalItem();

    void appendChild(NationalItem *child);

    NationalItem *child(qint32 row);
    qint32 childCount() const;
    qint32 columnCount() const;
    QVariant data(qint32 column) const;
    qint32 row() const;
    NationalItem *parentItem();

private:
    QVector<NationalItem*> m_childItems;
    QVector<QVariant> m_itemData;
    NationalItem *m_parentItem;
};

#endif // NATIONALITEM_H
