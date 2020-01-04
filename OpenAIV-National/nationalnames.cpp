/************************************************************************

    nationalnames.cpp

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

#include "nationalnames.h"

NationalNames::NationalNames()
{
    // Default to no data available
    isDataOpen = false;
}

NationalNames::~NationalNames()
{
    if (isDataOpen) close();
}

bool NationalNames::open(QString filename)
{
    if (isDataOpen) {
        qDebug() << "National names data is already open!";
        return false;
    }

    qDebug() << "Opening names file from" << filename;

    return false;
}

void NationalNames::close()
{
    if (!isDataOpen) return;
}
