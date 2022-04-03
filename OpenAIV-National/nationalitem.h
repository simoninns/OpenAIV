/************************************************************************

    nationalitem.h

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

#ifndef NATIONALITEM_H
#define NATIONALITEM_H

#include <QObject>
#include <QVariant>
#include <QVector>

class NationalItem
{
public:
    enum ItemType {
        hieararchy,
        named_gmap,
        named_amap,
        named_data,
        named_plan,
        named_text,
        named_pic,
        named_walk,
        named_film,
        unknown
    };

    explicit NationalItem(const QVector<QVariant> &data, const ItemType &itemType, NationalItem *parent = nullptr);
    ~NationalItem();

    void appendChild(NationalItem *child);

    NationalItem *child(qint32 row);
    qint32 childCount() const;
    qint32 columnCount() const;
    QVariant data(qint32 column) const;
    qint32 row() const;
    NationalItem *parentItem();
    ItemType itemType();
    QString itemTypeAsString();

private:
    QVector<NationalItem*> m_childItems;
    QVector<QVariant> m_itemData;
    NationalItem *m_parentItem;

    ItemType m_itemType;
};

#endif // NATIONALITEM_H
