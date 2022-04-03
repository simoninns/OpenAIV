/************************************************************************

    essay.cpp

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

#include "essay.h"

Essay::Essay(const QVector<qint32> &photoData, const qint32 &numberOfPages, const QString &title,
             const QVector<QString> &pageTitles, const QVector<QString> &pages)
            : m_photoData(photoData) ,m_numberOfPages(numberOfPages), m_title(title),
              m_pageTitles(pageTitles), m_pages(pages)
{
}

// Custom streaming operator (for debug)
QDebug operator<<(QDebug dbg, const Essay &essay)
{
    const qint32 numberOfPages = essay.numberOfPages();
    if (numberOfPages < 1)
        dbg.nospace().noquote() << "Essay()";
    else
        dbg.nospace().noquote() << "Essay(" << numberOfPages << " pages)";

    return dbg.maybeSpace();
}

// Get functions ------------------------------------------------------------------------------------------------------
QVector<qint32> Essay::photoData() const
{
    return m_photoData;
}

qint32 Essay::numberOfPages() const
{
    return m_numberOfPages;
}

QString Essay::title() const
{
    return m_title;
}

QVector<QString> Essay::pageTitles() const
{
    return m_pageTitles;
}

QVector<QString> Essay::pages() const
{
    return m_pages;
}

