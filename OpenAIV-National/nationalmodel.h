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

class NationalItem;

class NationalModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit NationalModel(QObject *parent = nullptr);
    ~NationalModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    // Data structures
    struct HierarchyRecord {
        qint32 index;
        qint32 fatherRecord;
        qint32 essayAddress;
        bool bottomFlag;
        qint32 level;
        QString textLabel;
        QVector<qint32> descPointers;
        QVector<qint32> crossRefs;
    };

    void setupModelData(const QString filename, NationalItem *parent);
    void readModelData(NationalItem *parent, qint32 fileIndex, QFile &fileHandle);
    HierarchyRecord readRecordFromFile(QFile &fileHandle, qint32 fileIndex);
    QByteArray readFile(qint32 filePointer, qint32 dataSize, QFile &fileHandle);

    NationalItem *rootItem;
    const qint32 hierarchyRecordSize = 128;
};

#endif // NATIONALMODEL_H
