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

}

// Method to set the filter's string
void HierarchySortFilter::setFilterString(QString filterString)
{
    m_filterString = filterString;
}

bool HierarchySortFilter::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(!m_filterString.isEmpty()) {
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
            QString key = sourceModel()->data(source_index, filterRole()).toString().toLower();

            if (key.contains(m_filterString.toLower())) {
                //qDebug() << "key =" << key << "filter =" << m_filterString << "result = true";
                return true;
            }
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

