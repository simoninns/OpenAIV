/************************************************************************

    data.h

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

#ifndef DATASET_H
#define DATASET_H

#include <QObject>
#include <QVector>
#include <QDebug>

// See: https://github.com/simoninns/OpenAIV/wiki/National-Data-file#data-set-records

class DataSet
{
public:
    DataSet() = default;
    ~DataSet() = default;
    DataSet(const DataSet &) = default;
    DataSet &operator=(const DataSet &) = default;

    DataSet(const QVector<QString> &chartLabels, const QVector<QString> &variableLabels,
            const QVector<QVector<QString>> &dimensionLabels, const QVector<qreal> &dataPoints,
            const quint32 &footNoteItemAddress, const bool &additiveDataFlag,
            const QVector<bool> &availableDisplayMethods, const qint32 &defaultDisplayMethod,
            const quint32 &colourSet);

    QVector<QString> chartLabels() const;
    QVector<QString> variableLabels() const;
    QVector<QVector<QString>> dimensionLabels() const;
    QVector<qreal> dataPoints() const;
    quint32 footNoteItemAddress() const;
    bool additiveDataFlag() const;
    QVector<bool> availableDisplayMethods() const;
    qint32 defaultDisplayMethod() const;
    quint32 colourSet() const;

private:
    QVector<QString> m_chartLabels;
    QVector<QString> m_variableLabels;
    QVector<QVector<QString>> m_dimensionLabels;
    QVector<qreal> m_dataPoints;
    quint32 m_footNoteItemAddress;
    bool m_additiveDataFlag;
    QVector<bool> m_availableDisplayMethods;
    qint32 m_defaultDisplayMethod;
    quint32 m_colourSet;
};

// Custom streaming operator
QDebug operator<<(QDebug dbg, const DataSet &dataSet);

// Custom meta-type declaration
Q_DECLARE_METATYPE(DataSet);

#endif // DATASET_H
