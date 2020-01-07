/************************************************************************

    datafile.cpp

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

#include "datafile.h"

DataFile::DataFile(QString filename1, QString filename2)
{
    fileReady = false;
    open(filename1, filename2);
}

DataFile::~DataFile()
{
    close();
}

void DataFile::open(QString filename1, QString filename2)
{
    // Open the data file
    qDebug() << "Opening data file from" << filename1 << "and" << filename2;

    fileHandle1.setFileName(filename1);
    if (!fileHandle1.open(QIODevice::ReadOnly)) {
        // Failed to open file
        qDebug() << "Data 1 open failed";
        fileReady = false;
    }

    fileHandle2.setFileName(filename2);
    if (!fileHandle2.open(QIODevice::ReadOnly)) {
        // Failed to open file
        qDebug() << "Data 2 open failed";
        fileReady = false;
    }

    qDebug() << "Data file 1 and 2 opened";
    fileReady = true;
}

void DataFile::close()
{
    fileHandle1.close();
    fileHandle2.close();
    fileReady = false;
    qDebug() << "Data file closed";
}

bool DataFile::isFileReady()
{
    return fileReady;
}

// Read an essay record and store in the Essay datatype
Essay DataFile::readEssayRecord(qint32 itemAddress)
{
    if (!fileReady) return Essay();
    qint32 targetFile = selectTargetDataFile(itemAddress);

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

    // Read the number of pages
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
        pageTitles.append(QString::fromUtf8(buffer).trimmed()); // Remove trailing spaces
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

// Read a picture set record and store it in the PictureSet datatype
PictureSet DataFile::readPictureSetRecord(qint32 itemAddress)
{
    if (!fileReady) return PictureSet();
    qint32 targetFile = selectTargetDataFile(itemAddress);

    // Initialise a new record
    QByteArray datasetHeader;
    datasetHeader.resize(28);
    qint32 numberOfPictures;
    QVector<qint32> frameNumbers;
    QVector<QString> shortCaptions;
    QVector<QString> longCaptions;
    qint32 longCaptionLength = 152;

    qint32 currentAddress = itemAddress;

    // Read the 28 byte dataset header
    datasetHeader = readFile(currentAddress, 28, targetFile);
    currentAddress += 28;

    // Define a buffer for reading data from the input file
    QByteArray buffer;
    buffer.resize(1024);

    // Read the number of pictures in the picture set and the long caption length flag
    buffer = readFile(currentAddress, 2, targetFile);
    uchar *uBuffer0 = reinterpret_cast<uchar*>(buffer.data());
    qint32 lengthBitAndNumberOfPictures = uBuffer0[0] + (uBuffer0[1] << 8);
    numberOfPictures = lengthBitAndNumberOfPictures & 0x7FFF;
    bool longCaptionFlag = (lengthBitAndNumberOfPictures & 0x8000) ? true : false;
    if (longCaptionFlag) longCaptionLength = 312; else longCaptionLength = 156;
    //qDebug() << "numberOfPictures =" << numberOfPictures;
    //qDebug() << "longCaptionLength =" << longCaptionLength;
    currentAddress += 2;

    // Read the 2 byte frame numbers (one per picture)
    buffer = readFile(currentAddress, 2 * numberOfPictures, targetFile);
    uchar *uBuffer1 = reinterpret_cast<uchar*>(buffer.data());

    for (qint32 i = 0; i < numberOfPictures; i++) {
        qint32 p = i * 2; // 2 byte hops
        qint32 frameNumber = uBuffer1[p] + (uBuffer1[p+1] << 8);
        frameNumbers.append(frameNumber);
    }
    currentAddress += 2 * numberOfPictures;

    // Read the 30 byte short captions (one per picture) - maximum of 100
    for (qint32 i = 0; i < numberOfPictures; i++) {
        buffer = readFile(currentAddress, 30, targetFile);
        buffer[31] = '\0'; // terminate the string
        shortCaptions.append(QString::fromUtf8(buffer).trimmed()); // Remove trailing spaces
        currentAddress += 30;
        //qDebug() << "Short caption" << i << "-" << shortCaptions.last();
    }

    // Read the longCaptionLength byte long captions (one per picture)
    for (qint32 i = 0; i < numberOfPictures; i++) {
        buffer = readFile(currentAddress, longCaptionLength, targetFile);
        buffer[longCaptionLength+1] = '\0'; // terminate the string
        longCaptions.append(QString::fromUtf8(buffer).trimmed()); // Remove trailing spaces
        currentAddress += longCaptionLength;
        //qDebug() << "Long caption" << i << "-" << longCaptions.last();
    }

    return PictureSet(numberOfPictures, frameNumbers, shortCaptions, longCaptions);
}

// Read a data set record and store it in the DataSet datatype
DataSet DataFile::readDataSetRecord(qint32 itemAddress)
{
    if (!fileReady) return DataSet();
    qint32 targetFile = selectTargetDataFile(itemAddress);

    qint32 currentAddress = itemAddress;

    // Read the 56 byte dataset header
    //
    // m.nc.vars   = 0   // Number of dimensions/variables
    // m.nc.datoff = 26  // Byte offset to data area
    // m.nc.dsize  = 30  // Data size (1,2 or 4)
    // m.nc.add    = 31  // Flag (from the code 2 is new data set, 4 is old-style dataset?)
    // m.nc.norm   = 32  // Normalising factor for values
    // m.nc.s.f    = 36  // 'M' or 'D' - multiply or divide by norm. fact
    // m.nc.sfe    = 37  // 'E' or ' ': exponent or value: scaling factor
    // m.nc.dm     = 42  // 10 bytes: list of available display methods
    // m.nc.defdis = 52  // Default display method
    // m.nc.colset = 53  // 3 bytes: colour set for dataset: BBC colours
    // (m.nc.labels.b = 56 // Start of label region)

    QByteArray datasetHeader;
    datasetHeader.resize(56);
    datasetHeader = readFile(currentAddress, 56, targetFile);
    uchar *uDatasetHeader = reinterpret_cast<uchar*>(datasetHeader.data());
    currentAddress += 56;

    qint32 numberOfVariables = uDatasetHeader[0];

    // These two fields state where in the DATA the dataset is, and the length of each datum
    qint32 dataAreaByteOffset = uDatasetHeader[26] + (uDatasetHeader[27] << 8) + (uDatasetHeader[28] << 16) + (uDatasetHeader[29] << 24);
    qint32 dataSize = uDatasetHeader[30];

    // Seems to be some form of bit flag?
    qint32 addFlag = uDatasetHeader[31];

    // The normalizing factor and a flag to show if values should be multiplied or divided by it
    qint32 normalizingFactor = uDatasetHeader[32] + (uDatasetHeader[33] << 8) + (uDatasetHeader[34] << 16) + (uDatasetHeader[35] << 24);
    qint32 normMultiplyOrDivide = uDatasetHeader[36];

    // The scaling factor and flag?
    qint32 scalingFactorFlag = uDatasetHeader[37]; // 'E' or ' '?
    qint32 scalingFactor = uDatasetHeader[38] + (uDatasetHeader[39] << 8) + (uDatasetHeader[40] << 16) + (uDatasetHeader[41] << 24);

    // Flags indicating which types of chart are appropriate for the dataset
    QVector<bool> availableDisplayMethods; // 10 flags
    for (qint32 i = 42; i < 52; i++) {
        bool allowed = uDatasetHeader[i] == 0 ? true : false;
        availableDisplayMethods.append(allowed);
    }

    // The default type of chart to use for the dataset
    qint32 defaultDisplayMethod = uDatasetHeader[52];

    // The Colour set to use for the chart
    qint32 colourSet = uDatasetHeader[53] + (uDatasetHeader[54] << 8) + (uDatasetHeader[55] << 16);

    // Show dataset header debug
    qDebug() << "Dataset header:";
    qDebug() << "numberOfVariables =" << numberOfVariables;
    qDebug() << "dataAreaByteOffset =" << dataAreaByteOffset;
    qDebug() << "dataSize =" << dataSize << "bytes";
    qDebug() << "addFlag =" << addFlag;
    qDebug() << "normalizingFactor =" << normalizingFactor;

    qDebug() << "normMultiplyOrDivide =" << normMultiplyOrDivide;
    qDebug() << "scalingFactorFlag =" << scalingFactorFlag;
    qDebug() << "scalingFactor =" << scalingFactor;

    for (qint32 i = 0; i < availableDisplayMethods.size(); i++) {
        qDebug().nospace() << "availableDisplayMethods[" << i << "] = " << availableDisplayMethods[i];
    }

    qDebug() << "defaultDisplayMethod =" << defaultDisplayMethod;
    qDebug() << "colourSet =" << colourSet;

    return DataSet();
}

// Private methods ----------------------------------------------------------------------------------------------------

// Method to read byte data from the data file
QByteArray DataFile::readFile(qint32 filePointer, qint32 dataSize, qint32 fileNumber)
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

// Method to select the correct target DATA file based on the
// item's address
qint32 DataFile::selectTargetDataFile(qint32 itemAddress)
{
    qint32 targetFile = 1; // DATA1
    if (itemAddress & 0x80000000) {
        targetFile = 2; // DATA2
        // Strip the most-significant bit from the item address
        itemAddress = itemAddress & 0x7FFFFFFF;
    }

    if (targetFile == 1) qDebug() << "Using DATA1 with item address" << itemAddress;
    else qDebug() << "Using DATA2 with item address" << itemAddress;

    return targetFile;
}
