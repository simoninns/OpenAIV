/************************************************************************

    nationalmodel.h

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

#ifndef NATIONALMODEL_H
#define NATIONALMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QFile>
#include <QDebug>

#include "nationalitem.h"

#include "Datatypes/hierarchy.h"
#include "Filetypes/hierarchyfile.h"
#include "Datatypes/names.h"
#include "Filetypes/namesfile.h"

class NationalItem;

class NationalModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit NationalModel(QObject *parent = nullptr);
    ~NationalModel();

    QVariant data(const QModelIndex &index, qint32 role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(qint32 section, Qt::Orientation orientation,
                        qint32 role = Qt::DisplayRole) const override;
    QModelIndex index(qint32 row, qint32 column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    qint32 totalHierarchyRecords();
    qint32 totalNamesRecords();

private:
    void setupModelData(NationalItem *parent);
    void recurseModelData(NationalItem *parent, qint32 fileIndex, HierarchyFile &hierarchyFile, NamesFile &namesFile);

    NationalItem *rootItem;
    qint32 m_totalHierarchyRecords;
    qint32 m_totalNamesRecords;
};

#endif // NATIONALMODEL_H
