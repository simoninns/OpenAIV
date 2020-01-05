/************************************************************************

    namesfile.h

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

#ifndef NAMESFILE_H
#define NAMESFILE_H

#include <QObject>
#include <QVector>
#include <QDebug>
#include <QFile>

#include "Datatypes/names.h"

class NamesFile
{
public:
    NamesFile(QString filename);
    ~NamesFile();

    bool isFileReady();
    Names readRecord(qint32 recordIndex);

private:
    QFile fileHandle;
    const qint32 namesRecordSize = 36;
    bool fileReady;

    void open(QString filename);
    void close();
    QByteArray readFile(qint32 filePointer, qint32 dataSize);
};

#endif // NAMESFILE_H
