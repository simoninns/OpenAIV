/************************************************************************

    nationalnames.h

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

#ifndef NATIONALNAMES_H
#define NATIONALNAMES_H

#include <QCoreApplication>
#include <QDebug>

class NationalNames
{
public:
    NationalNames();
    ~NationalNames();

    bool open(QString filename);
    void close();

private:
    // Constants
    const qint32 namesRecordSize = 36;

    // Data structures
    enum NameType {
        pics = 8, text7 = 7, text6 = 6, data = 4, maps1 = 1, maps3 = 3, walk = 9, film = 10
    };

    struct NameRecord {
        QString nameLabel; // allow space for terminator; string could be 30 chars so we need one extra
        NameType nameType;
        qint32 destinationOffset;
    };
    QVector<NameRecord> nameRecords;

    // Class globals
    bool isDataOpen;
};

#endif // NATIONALNAMES_H
