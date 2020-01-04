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
#include "nationalitem.h"

#include <QStringList>

NationalModel::NationalModel(QObject *parent)
    : QAbstractItemModel(parent)
{
    QString filename = "/home/sdi/tmp/Indiv_files/National/HIERARCHY";
    rootItem = new NationalItem({tr("Description"), tr("Index")});
    setupModelData(filename, rootItem);
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

QVariant NationalModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    NationalItem *item = static_cast<NationalItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags NationalModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant NationalModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

QModelIndex NationalModel::index(int row, int column, const QModelIndex &parent) const
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

void NationalModel::setupModelData(const QString filename, NationalItem *parent)
{
    // Open the hierarchy file
    qDebug() << "Opening hierarchy file from" << filename;
    QFile fileHandle;
    fileHandle.setFileName(filename);
    if (!fileHandle.open(QIODevice::ReadOnly)) {
        // Failed to open file
        qDebug() << "File open failed";
        return;
    }

    // Process the hierarchy file starting from index 0
    readModelData(parent, 0, fileHandle);

    // Close the hierarchy file
    fileHandle.close();
}

// Method to recursively read the model data records
void NationalModel::readModelData(NationalItem *parent, qint32 fileIndex, QFile &fileHandle)
{
    // Get a record from the file and store the result in the column data
    HierarchyRecord newRecord = readRecordFromFile(fileHandle, fileIndex);
    QVector<QVariant> columnData;
    columnData.append(newRecord.textLabel);
    columnData.append(newRecord.index);

    // Append a child item based on the hierarchy record
    qDebug() << "Appending" << fileIndex << "- desc:" << newRecord.textLabel;
    NationalItem* child = new NationalItem(columnData, parent);
    parent->appendChild(child);

    // Get the children recursively (bottomFlag = true - means no more children)
    if (!newRecord.bottomFlag) {
        for (qint32 i = 0; i < newRecord.descPointers.size(); i++) {
            readModelData(child, newRecord.descPointers[i], fileHandle);
        }
    }

    return;
}

NationalModel::HierarchyRecord NationalModel::readRecordFromFile(QFile &fileHandle, qint32 fileIndex)
{
    // Initialise a new record
    HierarchyRecord newRecord;
    newRecord.index = fileIndex;
    newRecord.fatherRecord = 0;
    newRecord.essayAddress = 0;
    newRecord.bottomFlag = false;
    newRecord.level = 0;
    newRecord.textLabel.clear();
    newRecord.descPointers.clear();
    newRecord.crossRefs.clear();

    // Read in a hierarchy record
    QByteArray hierarchyRawData = readFile(fileIndex, hierarchyRecordSize, fileHandle);
    uchar *uHierarchyRawData = reinterpret_cast<uchar*>(hierarchyRawData.data()); // Needs uchar for raw data manipulation

    // Interpret hierarchy record
    newRecord.fatherRecord = uHierarchyRawData[0] + (uHierarchyRawData[1] << 8) +
            (uHierarchyRawData[2] << 16) + (uHierarchyRawData[3] << 24);

    // Read essay pointer (if any, -1 if none)
    newRecord.essayAddress = uHierarchyRawData[6] + (uHierarchyRawData[7] << 8) +
            (uHierarchyRawData[8] << 16) + (uHierarchyRawData[9] << 24);

    // Read bottom flag
    if (uHierarchyRawData[42] == 0) newRecord.bottomFlag = false;
    else if (uHierarchyRawData[42] == 128) newRecord.bottomFlag = true;
    else {
        qDebug() << "Expected bottom flag value of 0 or 128, got" << uHierarchyRawData[42] << "defaulting to false";
        newRecord.bottomFlag = false;
    }

    // Read hierarchy level
    newRecord.level = uHierarchyRawData[43];

    // Read the text
    qint32 textLength = uHierarchyRawData[10]; // how long is the label? the rest is just space padding. We can ignore it.
    QByteArray rawString;
    rawString.resize(textLength + 1); // +1 for terminate character
    for (qint32 i = 11; i < 11 + textLength; i++) // read from offset 11 for specified number of characters
        rawString[i - 11] = uHierarchyRawData[i]; // write into the structure starting at 0, so subtract the 11 from the counter
    rawString[textLength] = '\0'; // terminate the string
    newRecord.textLabel = QString::fromUtf8(rawString);

    // Read the descendant pointers
    qint32 descPointer = -1;
    for (qint32 i = 44; i < 124; i += 4) {
        descPointer = uHierarchyRawData[i] + (uHierarchyRawData[i+1] << 8) + (uHierarchyRawData[i+2] << 16) + (uHierarchyRawData[i+3] << 24);
        if (descPointer == -1) break; // -1 is end of list
        else newRecord.descPointers.append(descPointer);
    }

    // Read cross-references (if any)
    qint32 xrefPtr = uHierarchyRawData[124] + (uHierarchyRawData[125] << 8) + (uHierarchyRawData[126] << 16) + (uHierarchyRawData[127] << 24);

    // If there are any cross-references (i.e. xrefPtr isn't zero), go get them from the far-end of the file
    // and populate the cross-reference vector.
    if (xrefPtr != 0) {
        // How many cross-references are there? That's the 2 bytes in the file at xrefPtr
        QByteArray refRawData = readFile(xrefPtr, 2, fileHandle);
        uchar *uXrefRawData = reinterpret_cast<uchar*>(refRawData.data());
        if (refRawData.size() != 2) {
            qFatal("Unable to read cross-reference amount from file");
        }
        qint32 xrefCounter = uXrefRawData[0];

        // Now we can get that quantity of cross-reference addresses and put them in the vector
        for (int ct = 0; ct < xrefCounter; ct++) {
            // Get the address from the file at the ptr+2 (skip counter) + 4 * ct
            QByteArray xrefData = readFile((xrefPtr + 2) + (ct * 4), 4, fileHandle);
            if (xrefData.size() != 4) {
                qFatal("Unable to read cross-reference data from file");
            }

            // Append the cross-reference
            newRecord.crossRefs.append(xrefData[0] + (xrefData[1] << 8) + (xrefData[2] << 16) + (xrefData[3] << 24));
        }
    }
    else newRecord.crossRefs.clear(); // There are no cross-references

    return newRecord;
}


// Method to read the raw file
QByteArray NationalModel::readFile(qint32 filePointer, qint32 dataSize, QFile &fileHandle)
{
    QByteArray response;
    response.resize(dataSize);

    // Verify that request is in bounds
    if ((filePointer + dataSize) > fileHandle.size()) {
        qFatal("Request for data beyond the size of the hierarchy file");
        return QByteArray();
    }

    // Seek to the specified file location
    if (!fileHandle.seek(filePointer)) {
        qFatal("Could not seek to required position in the hierarchy file");
        return QByteArray();
    }

    // Read the requested data from the file
    if (!fileHandle.read(response.data(), static_cast<qint64>(dataSize))) {
        qFatal("Could not read data from the hierarchy file");
        return QByteArray();
    }

    return response;
}
