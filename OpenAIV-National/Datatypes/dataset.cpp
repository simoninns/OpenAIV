/************************************************************************

    dataset.cpp

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

#include "dataset.h"

DataSet::DataSet(const qint32 &numberOfPictures, const QVector<qint32> &frameNumbers,
                       QVector<QString> &shortCaptions, const QVector<QString> &longCaptions)
            : m_numberOfPictures(numberOfPictures) ,m_frameNumbers(frameNumbers),
              m_shortCaptions(shortCaptions), m_longCaptions(longCaptions)
{
}

// Custom streaming operator (for debug)
QDebug operator<<(QDebug dbg, const DataSet &dataSet)
{
    const qint32 numberOfPictures = dataSet.numberOfPictures();
    if (numberOfPictures < 1)
        dbg.nospace().noquote() << "DataSet()";
    else {
        dbg.nospace().noquote() << "DataSet(" << numberOfPictures << ")";
        QVector<qint32> frameNumbers = dataSet.frameNumbers();
        if (!frameNumbers.isEmpty()) {
            dbg.nospace().noquote() << " with frames ";
            for (qint32 i = 0; i < frameNumbers.size(); i++) {
                dbg.nospace().noquote() << frameNumbers[i];
                if (i != frameNumbers.size() - 1) dbg.nospace().noquote() << ", ";
            }
        }
    }

    return dbg.maybeSpace();
}

// Get functions ------------------------------------------------------------------------------------------------------
qint32 DataSet::numberOfPictures() const
{
    return m_numberOfPictures;
}

QVector<qint32> DataSet::frameNumbers() const
{
    return m_frameNumbers;
}

QVector<QString> DataSet::shortCaptions() const
{
    return m_shortCaptions;
}

QVector<QString> DataSet::longCaptions() const
{
    return m_longCaptions;
}
