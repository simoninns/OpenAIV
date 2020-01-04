/************************************************************************

    nationalitem.cpp

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

#include "nationalitem.h"

NationalItem::NationalItem(const QVector<QVariant> &data, NationalItem *parent)
    : m_itemData(data), m_parentItem(parent)
{
}

NationalItem::~NationalItem()
{
    qDeleteAll(m_childItems);
}

void NationalItem::appendChild(NationalItem *item)
{
    m_childItems.append(item);
}

NationalItem *NationalItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

qint32 NationalItem::childCount() const
{
    return m_childItems.count();
}

qint32 NationalItem::columnCount() const
{
    return m_itemData.count();
}

QVariant NationalItem::data(qint32 column) const
{
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

NationalItem *NationalItem::parentItem()
{
    return m_parentItem;
}

qint32 NationalItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<NationalItem*>(this));

    return 0;
}
