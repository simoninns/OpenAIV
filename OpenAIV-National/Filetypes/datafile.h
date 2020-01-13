/************************************************************************

    datafile.h

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

#ifndef DATAFILE_H
#define DATAFILE_H

#include <QObject>
#include <QVector>
#include <QDebug>
#include <QFile>
#include <QtMath>

#include "Datatypes/essay.h"
#include "Datatypes/pictureset.h"
#include "Datatypes/dataset.h"
#include "Datatypes/walk.h"

class DataFile
{
public:
    DataFile(QString filename1, QString filename2);
    ~DataFile();

    bool isFileReady();
    Essay readEssayRecord(quint32 itemAddress);
    PictureSet readPictureSetRecord(quint32 itemAddress);
    DataSet readDataSetRecord(quint32 itemAddress);
    Walk readWalkRecord(quint32 itemAddress);

private:
    QFile fileHandle1;
    QFile fileHandle2;
    const qint32 essayRecordSize = 36;
    bool fileReady;

    void open(QString filename1, QString filename2);
    void close();
    quint32 scaleValue(quint32 dataValue, quint32 normalizingFactor, bool multiplyValue, bool scalingIsExponent);
    QByteArray readFile(quint32 filePointer, qint32 dataSize, qint32 fileNumber);
    qint32 selectTargetDataFile(quint32 &itemAddress);
};

#endif // DATAFILE_H
