/************************************************************************

    datafile.cpp

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

bool DataFile::isFileReady()
{
    return fileReady;
}

// Read an essay record and store in the Essay datatype
Essay DataFile::readEssayRecord(quint32 itemAddress)
{
    if (!fileReady) return Essay();
    qint32 targetFile = selectTargetDataFile(itemAddress);

    // Initialise a new record
    QVector<qint32> photoData;

    qint32 numberOfPages;
    QString title;
    QVector<QString> pageTitles;
    QVector<QString> pages;

    quint32 currentAddress = itemAddress;

    // Read the 28 byte dataset header
    QByteArray datasetHeader = readFile(currentAddress, 28, targetFile);
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
PictureSet DataFile::readPictureSetRecord(quint32 itemAddress)
{
    if (!fileReady) return PictureSet();
    qint32 targetFile = selectTargetDataFile(itemAddress);

    // Initialise a new record
    qint32 numberOfPictures;
    QVector<qint32> frameNumbers;
    QVector<QString> shortCaptions;
    QVector<QString> longCaptions;
    qint32 longCaptionLength = 152;

    quint32 currentAddress = itemAddress;

    // Read the 28 byte dataset header
    QByteArray datasetHeader = readFile(currentAddress, 28, targetFile);
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
DataSet DataFile::readDataSetRecord(quint32 itemAddress)
{
    if (!fileReady) return DataSet();

    qint32 targetFile = selectTargetDataFile(itemAddress);
    quint32 currentAddress = itemAddress;

    // Read the 28 byte dataset header
    QByteArray datasetHeader = readFile(currentAddress, 28, targetFile);
    currentAddress += 28;

    // The footnotes reference (itemAddress) is in the dataset header bytes 2-5;
    uchar *uDatasetHeader = reinterpret_cast<uchar*>(datasetHeader.data());
    quint32 footNoteItemAddress = uDatasetHeader[2] + (uDatasetHeader[3] << 8) +
            (uDatasetHeader[4] << 16) + (uDatasetHeader[5] << 24);

    // Read the 56 byte data chart header -----------------------------------------------------------------------------
    QByteArray dataChartHeader = readFile(currentAddress, 56, targetFile);
    uchar *uDataChartHeader = reinterpret_cast<uchar*>(dataChartHeader.data());
    currentAddress += 56;

    // Number of dimensions per variable in the dataset
    // Note: size of numberOfDimensionsPerVariable vector is the number of variables available
    QVector<qint32> numberOfDimensionsPerVariable;
    qDebug() << "uChartHeader[0] =" << uDataChartHeader[0];
    qDebug() << "uChartHeader[1] =" << uDataChartHeader[1];
    for (qint32 i = 2; i < 25; i++) {
        qint32 dimensions = uDataChartHeader[i];
        if (dimensions != 0) numberOfDimensionsPerVariable.append(dimensions);
        else break; // stop at the first 0 byte value (end of variables)
    }

    // The byte offset to the data area
    qint32 dataAreaByteOffset = uDataChartHeader[26] + (uDataChartHeader[27] << 8) +
            (uDataChartHeader[28] << 16) + (uDataChartHeader[29] << 24);

    // Number of bytes per datum in the data area
    qint32 dataSize = uDataChartHeader[30];

    if (dataSize != 1 && dataSize != 2 && dataSize != 4) {
        qWarning() << "dataSize of chart dataset is" << dataSize << "- it should be 1, 2 or 4 - defaulting to 4";
        dataSize = 4;
    }

    // Flag indicating if the data is additive/non-additive
    bool additiveDataFlag = uDataChartHeader[31] == 0 ? false : true;

    // The normalizing factor
    qint32 normalizingFactor = uDataChartHeader[32] + (uDataChartHeader[33] << 8) +
            (uDataChartHeader[34] << 16) + (uDataChartHeader[35] << 24);

    // Should the data be normalized by multiplication or division by the normalizing factor?
    // ASCII 77 ('M') Multiply or ASCII 13 ('D') Divide
    qint32 normMultiplyOrDivide = uDataChartHeader[36];

    bool multiplyValue = false;
    if (normMultiplyOrDivide == 77) { // Should be 77 or 13
        multiplyValue = true;
    } else {
        multiplyValue = false;
    }

    // Scaling factor: ASCII 69 ('E') indicates exponent and ASCII 32 (' ') indicates value
    qint32 scalingFactor = uDataChartHeader[37];

    bool scalingIsExponent = false;
    if (scalingFactor == 69) { // Should be 69 or 32
        scalingIsExponent = true;
    } else {
        scalingIsExponent = false;
    }

    // Get the unused word
    quint32 unusedWord = uDataChartHeader[38] + (uDataChartHeader[39] << 8) +
            (uDataChartHeader[40] << 16) + (uDataChartHeader[41] << 24);

    // 10 Flags indicating which types of chart are appropriate for the dataset
    QVector<bool> availableDisplayMethods; // 10 flags
    for (qint32 i = 42; i <= 51; i++) {
        bool allowed = uDataChartHeader[i] == 0 ? false : true;
        availableDisplayMethods.append(allowed);
    }

    // The default type of chart to use for the dataset
    qint32 defaultDisplayMethod = uDataChartHeader[52];

    // The Colour set to use for the chart (not sure how to interpret this - notes say "BBC Colours")
    qint32 colourSet = uDataChartHeader[53] + (uDataChartHeader[54] << 8) + (uDataChartHeader[55] << 16);

    // Show dataset header debug
    qDebug() << "Dataset header:";
    qDebug() << "Footnotes address = " << footNoteItemAddress;
    qDebug() << "numberOfVariables =" << numberOfDimensionsPerVariable.size();

    for (qint32 i = 0; i < numberOfDimensionsPerVariable.size(); i++) {
        qDebug().nospace() << "Variable #" << i << " has a dimension of " << numberOfDimensionsPerVariable[i];
    }

    qDebug() << "dataAreaByteOffset =" << dataAreaByteOffset;
    qDebug() << "dataSize =" << dataSize << "bytes";

    if (additiveDataFlag) qDebug() << "Chart data is additive";
    else qDebug() << "Chart data is non-additive";

    if (multiplyValue) qDebug() << "Values should be multiplied by scaling factor";
    else qDebug() << "Values should be divided by scaling factor";

    if (scalingIsExponent) qDebug() << "Scaling factor is an exponent";
    else qDebug() << "Scaling factor is a value";

    qDebug() << "unusedWord =" << unusedWord;

    for (qint32 i = 0; i < availableDisplayMethods.size(); i++) { // Chart types are 1 to 10
        qDebug().nospace() << "availableDisplayMethods[" << i+1 << "] = " << availableDisplayMethods[i];
    }

    qDebug() << "defaultDisplayMethod =" << defaultDisplayMethod;
    qDebug() << "colourSet =" << colourSet;

    // ----------------------------------------------------------------------------------------------------------------
    // Now we are at byte position 84 from the start of the record

    // Define a buffer for reading data from the input file
    QByteArray buffer;
    buffer.resize(1024);

    // Get the 3 chart labels (Title, primary unit, not sure)
    QVector<QString> chartLabels;
    for (qint32 i = 0; i < 3; i++) {
        buffer = readFile(currentAddress, 42, targetFile);
        buffer[buffer[0]+1] = '\0'; // terminate the string
        chartLabels.append(QString::fromUtf8(buffer.mid(1)).trimmed()); // Remove trailing spaces and ignore first byte
        currentAddress += 42;
        qDebug() << "Chart label" << i << "is" << chartLabels.last();
    }

    // Get the variable abbreviations and labels (42 byte maximum BCPL strings)
    // Each variable has a header string followed by the number of dimensions in numberOfDimensionsPerVariable[x]
    QVector<QString> variableLabels;
    variableLabels.resize(numberOfDimensionsPerVariable.size());

    QVector<QVector<QString>> dimensionLabels;
    QVector<QVector<QString>> dimensionLabelAbbreviations;

    dimensionLabels.resize(variableLabels.size());
    dimensionLabelAbbreviations.resize(variableLabels.size());

    for (qint32 v = 0; v < numberOfDimensionsPerVariable.size(); v++) {
        // Get the variable header
        buffer = readFile(currentAddress, 42, targetFile);
        buffer[buffer[0]+1] = '\0'; // terminate the string
        variableLabels[v] = QString::fromUtf8(buffer.mid(1)).trimmed(); // Remove trailing spaces and ignore first byte
        currentAddress += 42;
        qDebug() << "Variable" << v << "header is" << variableLabels.last();

        // Get the dimension names
        for (qint32 i = 0; i < numberOfDimensionsPerVariable[v]; i++) {
            buffer = readFile(currentAddress, 42, targetFile);
            buffer[buffer[0]+1] = '\0'; // terminate the string
            dimensionLabels[v].append(QString::fromUtf8(buffer.mid(1)).trimmed()); // Remove trailing spaces and ignore first byte
            dimensionLabelAbbreviations[v].append(QString::fromUtf8(buffer.mid(41, 1))); // Get the abbreviation at position 41 of the label
            currentAddress += 42;
            qDebug().nospace() << "  Label #" << i << ": " << dimensionLabelAbbreviations[v].last() << " = " << dimensionLabels[v].last();
        }
    }


    // Read the chart data set's data --------------------------------------------------------------------------------

    // Calculate the number of data points in the dataset
    qint32 numberOfDataPoints = 1;
    for (qint32 i = 0; i < numberOfDimensionsPerVariable.size(); i++) {
        numberOfDataPoints *= dimensionLabels[i].size();
    }
    qDebug() << "Number of datapoints in the dataset =" << numberOfDataPoints;

    QVector<qreal> dataPoints(numberOfDataPoints);

    for (qint32 i = 0; i < numberOfDataPoints; i++) {
        buffer = readFile(currentAddress, dataSize, targetFile);
        uchar *uChartData = reinterpret_cast<uchar*>(buffer.data());

        // Retrieve the next value based on the expected dataSize (in bytes)
        quint32 dataValue = 0;
        if (dataSize == 1) dataValue = uChartData[0]; // 8 bit value
        else if (dataSize == 2) dataValue = uChartData[0] + (uChartData[1] << 8); // 16 bit value
        else if (dataSize == 4) dataValue = uChartData[0] + (uChartData[1] << 8) +
                (uChartData[2] << 16) + (uChartData[3] << 24); // 32 bit value
        currentAddress += dataSize;

        // Scale the retrieved value and store
        dataPoints[i] = scaleValue(dataValue, normalizingFactor, multiplyValue, scalingIsExponent);
        //qDebug().nospace() << "dataPoint[" << i << "] = " << dataPoints[i];
    }

    return DataSet(chartLabels, variableLabels, dimensionLabels, dataPoints,
                   footNoteItemAddress, additiveDataFlag, availableDisplayMethods,
                   defaultDisplayMethod, colourSet);
}

// Read a walk record and store it in the Walk datatype
Walk DataFile::readWalkRecord(quint32 itemAddress)
{
    if (!fileReady) return Walk();
    qint32 targetFile = selectTargetDataFile(itemAddress);
    quint32 currentAddress = itemAddress;

    // Define a buffer for reading data from the input file
    QByteArray buffer;
    buffer.resize(100);

    // Read header (30 words - 60 bytes)
    buffer = readFile(currentAddress, 60, targetFile);
    currentAddress += 60;
    uchar *uBuffer0 = reinterpret_cast<uchar*>(buffer.data());

    // Get the table data (start from 28 due to the dataset header)
    quint16 ltable = uBuffer0[28] + (uBuffer0[29] << 8) + (uBuffer0[30] << 16) + (uBuffer0[31] << 24);
    quint16 ctable = uBuffer0[32] + (uBuffer0[33] << 8) + (uBuffer0[34] << 16) + (uBuffer0[35] << 24);
    quint16 ptable = uBuffer0[36] + (uBuffer0[37] << 8) + (uBuffer0[38] << 16) + (uBuffer0[39] << 24);
    quint16 dtable = uBuffer0[40] + (uBuffer0[41] << 8) + (uBuffer0[42] << 16) + (uBuffer0[43] << 24);

    // 44-47 unused
    // 48-49 unused

    quint16 dtablesize = uBuffer0[50] + (uBuffer0[51] << 8) + (uBuffer0[52] << 16) + (uBuffer0[53] << 24);

    quint16 baseview = uBuffer0[54] + (uBuffer0[55] << 8) - 1;
    quint16 baseplan = uBuffer0[56] + (uBuffer0[57] << 8);
    quint16 syslev = uBuffer0[58] + (uBuffer0[59] << 8);

    // Calculate remaining length
    // dtable is the last item + the length of the dtable - the 60 bytes we already read
    quint32 dataRemaining = dtable + dtablesize - 60;

    // Show the table data debug
    qDebug() << "Table data:";
    qDebug() << "  dataRemaining =" << dataRemaining << "bytes" << "- total size" << dataRemaining + 60 << "bytes";

    qDebug() << "  ltable =" << ltable << "bytes (link table)";
    qDebug() << "  ctable =" << ctable << "bytes (control table)";
    qDebug() << "  ptable =" << ptable << "bytes (plan table)";
    qDebug() << "  dtable =" << dtable << "bytes (detail table)";

    qDebug() << "  dtablesize =" << dtablesize << " bytes (detail table size)";

    qDebug() << "  baseview =" << baseview << "- initial frame is" << baseview;
    qDebug() << "  baseplan =" << baseplan << "- initial plan is" << baseview + baseplan;

    qDebug() << "  syslev =" << syslev;
    if (syslev == 1) qDebug() << "syslev = Gallery type structure";
    else qDebug() << "syslev = Normal structure";

    // Read the link table
    qint32 linkTableLength = ctable - ltable;
    qDebug() << "Reading" << linkTableLength << "bytes of the link table";
    QByteArray linkTable = readFile(currentAddress, linkTableLength, targetFile);
    currentAddress += linkTableLength;

    // Read the control table
    qint32 controlTableLength = ptable - ltable;
    qDebug() << "Reading" << controlTableLength << "bytes of the control table";
    QByteArray controlTable = readFile(currentAddress, controlTableLength, targetFile);
    currentAddress += controlTableLength;

    // Read the plan table
    qint32 planTableLength = dtable - ptable;
    qDebug() << "Reading" << planTableLength << "bytes of the plan table";
    QByteArray planTable = readFile(currentAddress, planTableLength, targetFile);
    currentAddress += planTableLength;

    // Read the detail table
    qDebug() << "Reading" << dtablesize << "bytes of the detail table";
    QByteArray detailTable = readFile(currentAddress, dtablesize, targetFile);
    currentAddress += dtablesize;

    return Walk(baseview, baseplan, syslev, linkTable,
                controlTable, planTable, detailTable);
}

// Private methods ----------------------------------------------------------------------------------------------------

// Method to scale a chart value according to the scaling flags
// and convert the result to a qreal FP value
quint32 DataFile::scaleValue(quint32 dataValue, quint32 normalizingFactor, bool multiplyValue, bool scalingIsExponent)
{
    qreal result = 0;

    if (multiplyValue && scalingIsExponent) result = static_cast<qreal>(dataValue) * qPow(10, static_cast<qreal>(normalizingFactor)); // Multiply / exponent
    else if (!multiplyValue && scalingIsExponent) result = static_cast<qreal>(dataValue) / qPow(10, static_cast<qreal>(normalizingFactor)); // Divide / exponent
    else if (multiplyValue && !scalingIsExponent) result = static_cast<qreal>(dataValue) * static_cast<qreal>(normalizingFactor); // Multiply / value
    else if (!multiplyValue && !scalingIsExponent) result = static_cast<qreal>(dataValue) / static_cast<qreal>(normalizingFactor); // Divide / value
    else result = 0;

    return result;
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

// Method to read byte data from the data file
QByteArray DataFile::readFile(quint32 filePointer, qint32 dataSize, qint32 fileNumber)
{
    QByteArray response;
    response.resize(dataSize);

    if (dataSize <= 0) return response;

    if (fileNumber == 1) { // DATA1
        // Verify that request is in bounds
        if ((filePointer + dataSize) > fileHandle1.size()) {
            qFatal("Request for data beyond the size of the DATA1 file");
            return QByteArray();
        }

        // Seek to the specified file location
        if (!fileHandle1.seek(filePointer)) {
            qFatal("Could not seek to required position in the DATA1 file");
            return QByteArray();
        }

        // Read the requested data from the file
        if (!fileHandle1.read(response.data(), static_cast<qint64>(dataSize))) {
            qFatal("Could not read data from the DATA1 file");
            return QByteArray();
        }
    } else if (fileNumber == 2) { // DATA2
        // Verify that request is in bounds
        if ((filePointer + dataSize) > fileHandle2.size()) {
            qFatal("Request for data beyond the size of the DATA2 file");
            return QByteArray();
        }

        // Seek to the specified file location
        if (!fileHandle2.seek(filePointer)) {
            qFatal("Could not seek to required position in the DATA2 file");
            return QByteArray();
        }

        // Read the requested data from the file
        if (!fileHandle2.read(response.data(), static_cast<qint64>(dataSize))) {
            qFatal("Could not read data from the DATA2 file");
            return QByteArray();
        }
    } else {
        qFatal("fileNumber must be either 1 or 2 - this is a bug");
    }

    return response;
}

// Method to select the correct target DATA file based on the
// item's address
qint32 DataFile::selectTargetDataFile(quint32& itemAddress)
{
    qint32 targetFile = 1; // DATA1
    if (itemAddress & 0x80000000) {
        targetFile = 2; // DATA2
        // Strip the most-significant bit from the item address
        itemAddress = itemAddress & 0x7FFFFFFF;
    }

    if (targetFile == 1) qDebug() << "Using file DATA1 with item address" << itemAddress;
    else qDebug() << "Using file DATA2 with item address" << itemAddress;

    return targetFile;
}
