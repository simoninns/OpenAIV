/************************************************************************

    walk.cpp

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

#include "walk.h"

Walk::Walk(const quint32 &baseview, const quint32 &baseplan, const quint32 &syslev,
           const QByteArray &linkTable, const QByteArray &controlTable,
           const QByteArray &planTable, const QByteArray &detailTable)
            : m_baseview(baseview), m_baseplan(baseplan), m_syslev(syslev),
              m_linkTable(linkTable), m_controlTable(controlTable),
              m_planTable(planTable), m_detailTable(detailTable)
{
}

// Custom streaming operator (for debug)
QDebug operator<<(QDebug dbg, const Walk &walk)
{
    const quint32 baseview = walk.baseview();

    dbg.nospace().noquote() << "Walk(Start frame #" << baseview << ")";

    return dbg.maybeSpace();
}

// Get functions ------------------------------------------------------------------------------------------------------
quint32 Walk::baseview() const
{
    return m_baseview;
}

quint32 Walk::baseplan() const
{
    return m_baseplan;
}

quint32 Walk::syslev() const
{
    return m_syslev;
}

QByteArray Walk::linkTable() const
{
    return m_linkTable;
}

QByteArray Walk::controlTable() const
{
    return m_controlTable;
}

QByteArray Walk::planTable() const
{
    return  m_planTable;
}

QByteArray Walk::detailTable() const
{
    return m_detailTable;
}
