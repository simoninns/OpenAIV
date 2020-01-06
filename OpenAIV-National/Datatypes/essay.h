/************************************************************************

    essay.h

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

#ifndef ESSAY_H
#define ESSAY_H

#include <QObject>
#include <QVector>
#include <QDebug>

// See: https://github.com/simoninns/OpenAIV/wiki/National-Essay-file

class Essay
{
public:
    Essay() = default;
    ~Essay() = default;
    Essay(const Essay &) = default;
    Essay &operator=(const Essay &) = default;

    Essay(const QVector<qint32> &photoData, const qint32 &numberOfPages, QString &title,
          QVector<QString> &pageTitles, const QVector<QString> &pages);

    QVector<qint32> photoData() const;
    qint32 numberOfPages() const;
    QString title() const;
    QVector<QString> pageTitles() const;
    QVector<QString> pages() const;

private:
    QVector<qint32> m_photoData;
    qint32 m_numberOfPages;
    QString m_title;
    QVector<QString> m_pageTitles;
    QVector<QString> m_pages;
};

// Custom streaming operator
QDebug operator<<(QDebug dbg, const Essay &essay);

// Custom meta-type declaration
Q_DECLARE_METATYPE(Essay);

#endif // ESSAY_H
