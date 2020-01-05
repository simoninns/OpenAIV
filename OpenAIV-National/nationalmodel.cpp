/************************************************************************

    nationalmodel.cpp

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

#include "nationalmodel.h"

NationalModel::NationalModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new NationalItem({tr("Description"), tr("Index")});
    setupModelData(rootItem);
}

NationalModel::~NationalModel()
{
    delete rootItem;
}

int NationalModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<NationalItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

QVariant NationalModel::data(const QModelIndex &index, qint32 role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    NationalItem *item = static_cast<NationalItem*>(index.internalPointer());

    // If the first column is a Hierarchy object, return the hierarchy record textLabel
    QString typeName = QString::fromUtf8(item->data(index.column()).typeName());
    if (QString::fromUtf8(item->data(index.column()).typeName()) == "Hierarchy")
        return item->data(index.column()).value<Hierarchy>().textLabel();

    return item->data(index.column());
}

Qt::ItemFlags NationalModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant NationalModel::headerData(qint32 section, Qt::Orientation orientation, qint32 role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex NationalModel::index(qint32 row, qint32 column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    NationalItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<NationalItem*>(parent.internalPointer());

    NationalItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}

QModelIndex NationalModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    NationalItem *childItem = static_cast<NationalItem*>(index.internalPointer());
    NationalItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int NationalModel::rowCount(const QModelIndex &parent) const
{
    NationalItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<NationalItem*>(parent.internalPointer());

    return parentItem->childCount();
}

// Set up the data for the model
void NationalModel::setupModelData(NationalItem *parent)
{
    HierarchyFile hierarchyFile("/home/sdi/tmp/Indiv_files/National/HIERARCHY");
    if (!hierarchyFile.isFileReady()) {
        qDebug() << "Could not open hierarchy file!";
        return;
    }

    // Process the hierarchy file starting from index 0
    recurseModelData(parent, 0, hierarchyFile);
}

// Method to recursively read the National hierarchy records into the data-model
void NationalModel::recurseModelData(NationalItem *parent, qint32 fileIndex, HierarchyFile &hierarchyFile)
{
    // Get a record from the file and store the result in the column data
    Hierarchy newHierarchyRecord = hierarchyFile.readRecord(fileIndex);
    QVector<QVariant> columnData;
    columnData.resize(1);
    columnData[0].setValue(newHierarchyRecord);

    // Append a child item based on the hierarchy record
    qDebug() << "Appending" << newHierarchyRecord;
    NationalItem* child = new NationalItem(columnData, parent);
    parent->appendChild(child);

    // Get the children recursively (bottomFlag = true - means no more children)
    if (!newHierarchyRecord.bottomFlag()) {
        for (qint32 i = 0; i < newHierarchyRecord.descPointers().size(); i++) {
            recurseModelData(child, newHierarchyRecord.descPointers()[i], hierarchyFile);
        }
    }

    return;
}

