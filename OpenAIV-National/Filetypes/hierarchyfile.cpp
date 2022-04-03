/************************************************************************

    hierarchyfile.cpp

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

#include "hierarchyfile.h"

HierarchyFile::HierarchyFile(QString filename)
{
    fileReady = false;
    open(filename);
}

HierarchyFile::~HierarchyFile()
{
    close();
}

void HierarchyFile::open(QString filename)
{
    // Open the hierarchy file
    qDebug() << "Opening hierarchy file from" << filename;

    fileHandle.setFileName(filename);
    if (!fileHandle.open(QIODevice::ReadOnly)) {
        // Failed to open file
        qDebug() << "Hierarchy open failed";
        fileReady = false;
    }

    qDebug() << "Hierarchy file opened";
    fileReady = true;
}

void HierarchyFile::close()
{
    fileHandle.close();
    fileReady = false;
    qDebug() << "Hierarchy file closed";
}

bool HierarchyFile::isFileReady()
{
    return fileReady;
}

// Read a hierarchy record and store in the Hierarchy datatype
Hierarchy HierarchyFile::readRecord(quint32 fileIndex)
{
    if (!fileReady) return Hierarchy();

    // Initialise a new record
    quint32 index = fileIndex;
    quint32 father = 0;
    qint32 text = 0;
    QString title;
    bool bottomFlag = false;
    qint32 level = 0;
    QVector<quint32> hdps;
    QVector<quint32> xrefs;

    // Read in a hierarchy record
    QByteArray hierarchyRawData = readFile(fileIndex, hierarchyRecordSize);
    uchar *uHierarchyRawData = reinterpret_cast<uchar*>(hierarchyRawData.data()); // Needs uchar for raw data manipulation

    // Read father pointer
    father = uHierarchyRawData[0] + (uHierarchyRawData[1] << 8) +
            (uHierarchyRawData[2] << 16) + (uHierarchyRawData[3] << 24);

    // Read text pointer
    text = uHierarchyRawData[6] + (uHierarchyRawData[7] << 8) +
            (uHierarchyRawData[8] << 16) + (uHierarchyRawData[9] << 24);

    // Read the title string
    qint32 textLength = uHierarchyRawData[10]; // Get the length of the string
    char rawString[32];
    for (qint32 i = 0; i < textLength; i++)
        rawString[i] = uHierarchyRawData[11 + i];
    rawString[textLength] = '\0'; // terminate the string
    title = QString::fromUtf8(rawString);

    // Read bottom flag
    if (uHierarchyRawData[42] == 0) bottomFlag = false;
    else if (uHierarchyRawData[42] == 128) bottomFlag = true;
    else {
        qDebug() << "Expected bottom flag value of 0 or 128, got" << uHierarchyRawData[42] << "defaulting to false";
        bottomFlag = false;
    }

    // Read hierarchy level
    level = uHierarchyRawData[43];

    // Read the Hierarchy Descendant Pointers
    qint32 descPointer = -1;
    for (qint32 i = 44; i < 124; i += 4) {
        descPointer = uHierarchyRawData[i] + (uHierarchyRawData[i+1] << 8) + (uHierarchyRawData[i+2] << 16) + (uHierarchyRawData[i+3] << 24);
        if (descPointer == -1) break; // -1 is end of list (0xFFFFFFFF)
        else hdps.append(descPointer);
    }


    // Note: the following is probably broken - todo - fix it.

    // Read cross-references (if any)
    qint32 xrefPtr = uHierarchyRawData[124] + (uHierarchyRawData[125] << 8) + (uHierarchyRawData[126] << 16) + (uHierarchyRawData[127] << 24);

    // If there are any cross-references (i.e. xrefPtr isn't 0xFFFFFFFF), go get them from the far-end of the file
    // and populate the cross-reference vector.
    if (xrefPtr >= 0) {
        // How many cross-references are there? That's the 2 bytes in the file at xrefPtr
        QByteArray refRawData = readFile(xrefPtr, 2);
        uchar *uXrefRawData = reinterpret_cast<uchar*>(refRawData.data());
        if (refRawData.size() != 2) {
            qFatal("Unable to read cross-reference amount from file");
        }
        qint32 xrefCounter = uXrefRawData[0] + (uXrefRawData[1] << 8);

        // Now we can get that quantity of cross-reference addresses and put them in the vector
        for (int ct = 0; ct < xrefCounter; ct++) {
            // Get the address from the file at the ptr+2 (skip counter) + 4 * ct
            QByteArray xrefData = readFile((xrefPtr + 2) + (ct * 4), 4);
            uchar *uXrefData = reinterpret_cast<uchar*>(xrefData.data());
            if (xrefData.size() != 4) {
                qFatal("Unable to read cross-reference data from file");
            }

            // Append the cross-reference
            xrefs.append(uXrefData[0] + (uXrefData[1] << 8) + (uXrefData[2] << 16) + (uXrefData[3] << 24));
        }
    }
    else xrefs.clear(); // There are no cross-references

    return Hierarchy(index, father, text, bottomFlag, level, title, hdps, xrefs);
}


// Method to read byte data from the original file
QByteArray HierarchyFile::readFile(quint32 filePointer, qint32 dataSize)
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
