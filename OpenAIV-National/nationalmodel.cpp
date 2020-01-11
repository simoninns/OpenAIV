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

NationalModel::NationalModel(const QDir &_nationalFileDirectory, QObject *parent)
    : QAbstractItemModel(parent), nationalFileDirectory(_nationalFileDirectory)
{
    // Reset statistics
    m_totalHierarchyRecords = 0;
    m_totalNamesRecords = 0;

    // Load the model
    rootItem = new NationalItem({tr("Description")}, NationalItem::ItemType::unknown);
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

    // Get the column type name
    QString typeName = QString::fromUtf8(item->data(index.column()).typeName());

    // If the column is a Hierarchy object, return the hierarchy record title string
    if (typeName == "Hierarchy")
        return item->data(index.column()).value<Hierarchy>().title();

    // If the column is a Names object, return the names record label string
    if (typeName == "Names")
        return item->data(index.column()).value<Names>().itemTypeAndItemName();

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

qint32 NationalModel::totalHierarchyRecords()
{
    return m_totalHierarchyRecords;
}

qint32 NationalModel::totalNamesRecords()
{
    return m_totalNamesRecords;
}

// Set up the data for the model
void NationalModel::setupModelData(NationalItem *parent)
{
    QFileInfo hierarchyFileInfo(nationalFileDirectory, "HIERARCHY");
    HierarchyFile hierarchyFile(hierarchyFileInfo.filePath());
    if (!hierarchyFile.isFileReady()) {
        qDebug() << "Could not open hierarchy file!";
        return;
    }

    QFileInfo namesFileInfo(nationalFileDirectory, "NAMES");
    NamesFile namesFile(namesFileInfo.filePath());
    if (!namesFile.isFileReady()) {
        qDebug() << "Could not open names file!";
        return;
    }

    // Process the hierarchy file starting from index 0
    recurseModelData(parent, 0, hierarchyFile, namesFile);
}

// Method to recursively read the National hierarchy records into the data-model
void NationalModel::recurseModelData(NationalItem *parent, qint32 fileIndex,
                                     HierarchyFile &hierarchyFile, NamesFile &namesFile)
{
    // Get a record from the file and store the result in the column data
    Hierarchy newHierarchyRecord = hierarchyFile.readRecord(fileIndex);
    QVector<QVariant> columnData;
    columnData.resize(1);
    columnData[0].setValue(newHierarchyRecord);

    // Append a child item based on the hierarchy record   
    //qDebug() << "Appending" << newHierarchyRecord;
    NationalItem* child = new NationalItem(columnData, NationalItem::ItemType::hieararchy, parent);
    parent->appendChild(child);
    m_totalHierarchyRecords++;

    // Get the hierarchy children recursively (bottomFlag = false = children available)
    if (!newHierarchyRecord.bottomFlag()) {
        for (qint32 i = 0; i < newHierarchyRecord.hdps().size(); i++) {
            recurseModelData(child, newHierarchyRecord.hdps()[i], hierarchyFile, namesFile);
        }
    }

    // Get the names records and append as children (bottomFlag = true = names available)
    if (newHierarchyRecord.bottomFlag()) {
        for (qint32 i = 0; i < newHierarchyRecord.hdps().size(); i++) {
            Names newNamesRecord = namesFile.readRecord(newHierarchyRecord.hdps()[i]);
            QVector<QVariant> namesColumnData;
            namesColumnData.resize(1);
            namesColumnData[0].setValue(newNamesRecord);
            //qDebug() << "Appending" << newNamesRecord;

            NationalItem::ItemType itemType;
            switch(newNamesRecord.itemType()) {
            case 1:
                itemType = NationalItem::ItemType::named_gmap;
                break;
            case 3:
                itemType = NationalItem::ItemType::named_amap;
                break;
            case 4:
                itemType = NationalItem::ItemType::named_data;
                break;
            case 6:
            case 7:
                itemType = NationalItem::ItemType::named_text;
                break;
            case 8:
                itemType = NationalItem::ItemType::named_pic;
                break;
            case 9:
                itemType = NationalItem::ItemType::named_walk;
                break;
            case 10:
                itemType = NationalItem::ItemType::named_film;
                break;
            default:
                itemType = NationalItem::ItemType::unknown;
            }

            NationalItem* namesChild = new NationalItem(namesColumnData, itemType, child);
            child->appendChild(namesChild);
            m_totalNamesRecords++;
        }
    }

    return;
}

