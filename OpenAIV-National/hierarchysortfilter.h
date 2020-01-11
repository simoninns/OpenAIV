/************************************************************************

    hierarchysortfilter.h

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

#ifndef HIERARCHYSORTFILTER_H
#define HIERARCHYSORTFILTER_H

#include <QObject>
#include <QSortFilterProxyModel>
#include <QDebug>

class HierarchySortFilter : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit HierarchySortFilter(QObject *parent = nullptr);

    void setFilterString(QString filterString);
    void forceUpdate();
    qint32 totalHierarchyRecords();
    qint32 totalNamesRecords();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;

private:
    QString m_filterString;
};

#endif // HIERARCHYSORTFILTER_H
