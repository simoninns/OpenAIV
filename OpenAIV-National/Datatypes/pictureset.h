/************************************************************************

    pictureset.h

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

#ifndef PICTURESET_H
#define PICTURESET_H

#include <QObject>
#include <QVector>
#include <QDebug>

// See: https://github.com/simoninns/OpenAIV/wiki/National-Data-file#picture-set-records

class PictureSet
{
public:
    PictureSet() = default;
    ~PictureSet() = default;
    PictureSet(const PictureSet &) = default;
    PictureSet &operator=(const PictureSet &) = default;

    PictureSet(const qint32 &numberOfPictures, const QVector<qint32> &frameNumbers,
          const QVector<QString> &shortCaptions, const QVector<QString> &longCaptions);

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
QDebug operator<<(QDebug dbg, const PictureSet &pictureSet);

// Custom meta-type declaration
Q_DECLARE_METATYPE(PictureSet);

#endif // PICTURESET_H
