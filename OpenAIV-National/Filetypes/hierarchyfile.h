/************************************************************************

    hierarchyfile.h

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

#ifndef HIERARCHYFILE_H
#define HIERARCHYFILE_H

#include <QObject>
#include <QVector>
#include <QDebug>
#include <QFile>

#include "Datatypes/hierarchy.h"

class HierarchyFile
{
public:
    HierarchyFile(QString filename);
    ~HierarchyFile();

    bool isFileReady();
    Hierarchy readRecord(quint32 fileIndex);

private:
    QFile fileHandle;
    const qint32 hierarchyRecordSize = 128;
    bool fileReady;

    void open(QString filename);
    void close();
    QByteArray readFile(quint32 filePointer, qint32 dataSize);
};

#endif // HIERARCHYFILE_H
