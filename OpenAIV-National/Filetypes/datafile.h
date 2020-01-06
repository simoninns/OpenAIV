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

#include "Datatypes/essay.h"

class DataFile
{
public:
    DataFile(QString filename1, QString filename2);
    ~DataFile();

    bool isFileReady();
    Essay readEssay(qint32 itemAddress);

private:
    QFile fileHandle1;
    QFile fileHandle2;
    const qint32 essayRecordSize = 36;
    bool fileReady;

    void open(QString filename1, QString filename2);
    void close();
    QByteArray readFile(qint32 filePointer, qint32 dataSize, qint32 fileNumber);
};

#endif // DATAFILE_H
