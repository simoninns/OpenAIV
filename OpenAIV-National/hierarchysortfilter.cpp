/************************************************************************

    hierarchysortfilter.cpp

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

#include "hierarchysortfilter.h"

HierarchySortFilter::HierarchySortFilter(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    m_filterString = "";
    m_gmap = true;
    m_amap = true;
    m_data = true;
    m_text = true;
    m_pic = true;
    m_walk = true;
    m_film = true;
}

// Method to set the filter's string
void HierarchySortFilter::setFilter(QString filterString,
                                    bool gmap, bool amap, bool data,
                                    bool text, bool pic, bool walk, bool film)
{
    m_filterString = filterString;
    m_gmap = gmap;
    m_amap = amap;
    m_data = data;
    m_text = text;
    m_pic = pic;
    m_walk = walk;
    m_film = film;
}

bool HierarchySortFilter::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(!m_filterString.isEmpty() || !(m_gmap && m_amap && m_data & m_text &&
            m_pic && m_walk && m_film)) {
        // Get source-model index for current row
        QModelIndex source_index = sourceModel()->index(source_row, this->filterKeyColumn(), source_parent);
        if(source_index.isValid()) {
            // If any of children matches the filter, then current index matches the filter as well
            qint32 nb = sourceModel()->rowCount(source_index) ;
            for(qint32 i = 0; i < nb; i++) {
                if(filterAcceptsRow(i, source_index)) {
                    return true;
                }
            }
            // Check the current index
            NationalItem* currentItem = reinterpret_cast<NationalItem *>(source_index.internalPointer());
            NationalItem::ItemType itemType = currentItem->itemType();
            QString key = sourceModel()->data(source_index, filterRole()).toString().toLower();

            if (key.contains(m_filterString.toLower()) && itemType == NationalItem::ItemType::named_gmap && m_gmap) return true;
            else if (key.contains(m_filterString.toLower()) && itemType == NationalItem::ItemType::named_amap && m_amap) return true;
            else if (key.contains(m_filterString.toLower()) && itemType == NationalItem::ItemType::named_data && m_data) return true;
            else if (key.contains(m_filterString.toLower()) && itemType == NationalItem::ItemType::named_text && m_text) return true;
            else if (key.contains(m_filterString.toLower()) && itemType == NationalItem::ItemType::named_pic && m_pic) return true;
            else if (key.contains(m_filterString.toLower()) && itemType == NationalItem::ItemType::named_walk && m_walk) return true;
            else if (key.contains(m_filterString.toLower()) && itemType == NationalItem::ItemType::named_film && m_film) return true;
            else return false;
        }
    }

    // Call original parent method and return result
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}

void HierarchySortFilter::forceUpdate()
{
    qDebug() << "Forcing filter update";
    invalidateFilter();
}

