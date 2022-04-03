/************************************************************************

    namesfile.cpp

    OpenAIV-National - Domesday Retrieval application
    Copyright (C) 2020-2022 Simon Inns

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

#include "namesfile.h"

NamesFile::NamesFile(QString filename)
{
    fileReady = false;
    open(filename);
}

NamesFile::~NamesFile()
{
    close();
}

void NamesFile::open(QString filename)
{
    // Open the names file
    qDebug() << "Opening names file from" << filename;

    fileHandle.setFileName(filename);
    if (!fileHandle.open(QIODevice::ReadOnly)) {
        // Failed to open file
        qDebug() << "Names open failed";
        fileReady = false;
    }

    qDebug() << "Names file opened";
    fileReady = true;
}

void NamesFile::close()
{
    fileHandle.close();
    fileReady = false;
    qDebug() << "Names file closed";
}

bool NamesFile::isFileReady()
{
    return fileReady;
}

// Read a names record and store in the Names datatype
Names NamesFile::readRecord(quint32 recordIndex)
{
    if (!fileReady) return Names();

    // Initialise a new record
    quint32 index = recordIndex * namesRecordSize;
    QString itemName;
    qint32 itemType;
    quint32 itemAddress;

    // Read in a hierarchy record
    QByteArray namesRawData = readFile(index, namesRecordSize);
    uchar *uNamesRawData = reinterpret_cast<uchar*>(namesRawData.data()); // Needs uchar for raw data manipulation

    // Read the item name
    qint32 textLength = uNamesRawData[0]; // How long is the label?
    char rawString[32];
    for (qint32 i = 0; i < textLength; i++) {
        rawString[i] = uNamesRawData[i + 1];
    }

    rawString[textLength] = '\0'; // terminate the string
    itemName = QString::fromUtf8(rawString);

    // Read the item type
    itemType = uNamesRawData[31];

    // Read the item address
    itemAddress = uNamesRawData[32] + (uNamesRawData[33] << 8) + (uNamesRawData[34] << 16) + (uNamesRawData[35] << 24);

    return Names(itemName, itemType, itemAddress);
}


// Method to read byte data from the original file
QByteArray NamesFile::readFile(quint32 filePointer, qint32 dataSize)
{
    QByteArray response;
    response.resize(dataSize);

    // Verify that request is in bounds
    if ((filePointer + dataSize) > fileHandle.size()) {
        qFatal("Request for data beyond the size of the names file");
        return QByteArray();
    }

    // Seek to the specified file location
    if (!fileHandle.seek(filePointer)) {
        qFatal("Could not seek to required position in the names file");
        return QByteArray();
    }

    // Read the requested data from the file
    if (!fileHandle.read(response.data(), static_cast<qint64>(dataSize))) {
        qFatal("Could not read data from the names file");
        return QByteArray();
    }

    return response;
}
