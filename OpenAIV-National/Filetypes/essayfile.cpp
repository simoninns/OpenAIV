/************************************************************************

    essayfile.cpp

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

#include "essayfile.h"

EssayFile::EssayFile(QString filename1, QString filename2)
{
    fileReady = false;
    open(filename1, filename2);
}

EssayFile::~EssayFile()
{
    close();
}

void EssayFile::open(QString filename1, QString filename2)
{
    // Open the names file
    qDebug() << "Opening essay file from" << filename1 << "and" << filename2;

    fileHandle1.setFileName(filename1);
    if (!fileHandle1.open(QIODevice::ReadOnly)) {
        // Failed to open file
        qDebug() << "Essay 1 open failed";
        fileReady = false;
    }

    fileHandle2.setFileName(filename2);
    if (!fileHandle2.open(QIODevice::ReadOnly)) {
        // Failed to open file
        qDebug() << "Essay 2 open failed";
        fileReady = false;
    }

    qDebug() << "Essay file 1 and 2 opened";
    fileReady = true;
}

void EssayFile::close()
{
    fileHandle1.close();
    fileHandle2.close();
    fileReady = false;
    qDebug() << "Essay file closed";
}

bool EssayFile::isFileReady()
{
    return fileReady;
}

// Read a names record and store in the Names datatype
Essay EssayFile::readEssay(qint32 itemAddress)
{
    if (!fileReady) return Essay();

    // Select the correct target DATA file based on the
    // item's address
    qint32 targetFile = 1; // DATA1
    if (itemAddress & 0x80000000) {
        targetFile = 2; // DATA2
        // Strip the most-significant bit from the item address
        itemAddress = itemAddress & 0x7FFFFFFF;
    }

    // Initialise a new record
    QByteArray datasetHeader;
    datasetHeader.resize(28);
    QVector<qint32> photoData;

    qint32 numberOfPages;
    QString title;
    QVector<QString> pageTitles;
    QVector<QString> pages;

    qint32 currentAddress = itemAddress;

    // Read the 28 byte dataset header
    datasetHeader = readFile(currentAddress, 28, targetFile);
    currentAddress += 28;

    // Note: it looks like there is some kind of flag to indicate if the text
    // page font should be proportional or fixed... not sure where it is though...
    // if ((st%os & #x80) ~= 0) then propspaced := FALSE // a tabular text page

    // Define a buffer for reading data from the input file
    QByteArray buffer;
    buffer.resize(1024);

    // Read the 200 byte photoData
    buffer = readFile(currentAddress, 200, targetFile);
    uchar *uBuffer0 = reinterpret_cast<uchar*>(buffer.data());

    // The photo data is 25 entries of 8 bytes each (200 bytes total)
    // I think this is a list of item addresses... but until the photo
    // overlay is implemented... hard to be sure.  For now I am making
    // it a vector of 50 32-bit values.
    for (qint32 i = 0; i < 50; i++) {
        qint32 p = i * 4; // 4 byte hops
        qint32 photoWord = uBuffer0[p] + (uBuffer0[p+1] << 8) + (uBuffer0[p+2] << 16) + (uBuffer0[p+3] << 24);
        photoData.append(photoWord);
    }
    currentAddress += 200;

    buffer = readFile(currentAddress, 2, targetFile);
    uchar *uBuffer1 = reinterpret_cast<uchar*>(buffer.data());
    numberOfPages = uBuffer1[0] + (uBuffer1[1] << 8);
    currentAddress += 2;

    // Read the 30 byte title
    buffer = readFile(currentAddress, 30, targetFile);
    buffer[31] = '\0'; // terminate the string
    title = QString::fromUtf8(buffer);
    currentAddress += 30;

    // Read the 30 byte page title (one per page)
    for (qint32 i = 0; i < numberOfPages; i++) {
        buffer = readFile(currentAddress, 30, targetFile);
        buffer[31] = '\0'; // terminate the string
        pageTitles.append(QString::fromUtf8(buffer));
        currentAddress += 30;
    }

    // Read the pages
    for (qint32 i = 0; i < numberOfPages; i++) {
        QByteArray buffer = readFile(currentAddress, 858, targetFile);
        buffer[859] = '\0';
        pages.append(QString::fromUtf8(buffer));
        currentAddress += 858;

        //qDebug() << "Page" << i << "=" << pages.last();
    }

    return Essay(photoData, numberOfPages, title, pageTitles, pages);
}

// Method to read byte data from the original file 1
QByteArray EssayFile::readFile(qint32 filePointer, qint32 dataSize, qint32 fileNumber)
{
    QByteArray response;
    response.resize(dataSize);

    if (fileNumber == 1) { // DATA1
        // Verify that request is in bounds
        if ((filePointer + dataSize) > fileHandle1.size()) {
            qFatal("Request for data beyond the size of the essay file");
            return QByteArray();
        }

        // Seek to the specified file location
        if (!fileHandle1.seek(filePointer)) {
            qFatal("Could not seek to required position in the essay file");
            return QByteArray();
        }

        // Read the requested data from the file
        if (!fileHandle1.read(response.data(), static_cast<qint64>(dataSize))) {
            qFatal("Could not read data from the essay file");
            return QByteArray();
        }
    } else if (fileNumber == 2) { // DATA2
        // Verify that request is in bounds
        if ((filePointer + dataSize) > fileHandle2.size()) {
            qFatal("Request for data beyond the size of the essay file");
            return QByteArray();
        }

        // Seek to the specified file location
        if (!fileHandle2.seek(filePointer)) {
            qFatal("Could not seek to required position in the essay file");
            return QByteArray();
        }

        // Read the requested data from the file
        if (!fileHandle2.read(response.data(), static_cast<qint64>(dataSize))) {
            qFatal("Could not read data from the essay file");
            return QByteArray();
        }
    } else {
        qFatal("Passed itemType is not an essay!");
    }

    return response;
}
