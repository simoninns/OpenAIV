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

DataSet::DataSet(const QVector<QString> &chartLabels, const QVector<QString> &variableLabels,
                 const QVector<QVector<QString>> &dimensionLabels, const QVector<qreal> &dataPoints,
                 const quint32 &footNoteItemAddress, const bool &additiveDataFlag,
                 const QVector<bool> &availableDisplayMethods, const qint32 &defaultDisplayMethod,
                 const quint32 &colourSet)
            : m_chartLabels(chartLabels) ,m_variableLabels(variableLabels),
              m_dimensionLabels(dimensionLabels), m_dataPoints(dataPoints),
              m_footNoteItemAddress(footNoteItemAddress), m_additiveDataFlag(additiveDataFlag),
              m_availableDisplayMethods(availableDisplayMethods), m_defaultDisplayMethod(defaultDisplayMethod),
              m_colourSet(colourSet)
{
}

// Custom streaming operator (for debug)
QDebug operator<<(QDebug dbg, const DataSet &dataSet)
{
    const qint32 numberOfDataPoints = dataSet.dataPoints().size();
    const QString chartTitle = dataSet.chartLabels()[0];
    if (numberOfDataPoints < 1)
        dbg.nospace().noquote() << "DataSet()";
    else {
        dbg.nospace().noquote() << "DataSet(" << numberOfDataPoints << ") - " << chartTitle;
    }

    return dbg.maybeSpace();
}

// Get functions ------------------------------------------------------------------------------------------------------
QVector<QString> DataSet::chartLabels() const
{
    return  m_chartLabels;
}

QVector<QString> DataSet::variableLabels() const
{
    return m_variableLabels;
}

QVector<QVector<QString>> DataSet::dimensionLabels() const
{
    return m_dimensionLabels;
}

QVector<qreal> DataSet::dataPoints() const
{
    return m_dataPoints;
}

quint32 DataSet::footNoteItemAddress() const
{
    return m_footNoteItemAddress;
}

bool DataSet::additiveDataFlag() const
{
    return m_additiveDataFlag;
}

QVector<bool> DataSet::availableDisplayMethods() const
{
    return m_availableDisplayMethods;
}

qint32 DataSet::defaultDisplayMethod() const
{
    return m_defaultDisplayMethod;
}

quint32 DataSet::colourSet() const
{
    return m_colourSet;
}
