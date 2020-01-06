/************************************************************************

    data.h

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

    DataSet(const qint32 &numberOfPictures, const QVector<qint32> &frameNumbers,
          QVector<QString> &shortCaptions, const QVector<QString> &longCaptions);

    qint32 numberOfPictures() const;
    QVector<qint32> frameNumbers() const;
    QVector<QString> shortCaptions() const;
    QVector<QString> longCaptions() const;

private:
    qint32 m_numberOfPictures;
    QVector<qint32> m_frameNumbers;
    QVector<QString> m_shortCaptions;
    QVector<QString> m_longCaptions;
};

// Custom streaming operator
QDebug operator<<(QDebug dbg, const DataSet &dataSet);

// Custom meta-type declaration
Q_DECLARE_METATYPE(DataSet);

#endif // DATASET_H
