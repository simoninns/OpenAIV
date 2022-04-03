/************************************************************************

    hierarchy.cpp

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

#include "hierarchy.h"

Hierarchy::Hierarchy(const quint32 &index, const quint32 &father, const qint32 &text,
          const bool &bottomFlag, const qint32 &level, const QString &title,
          const QVector<quint32> &hdps, const QVector<quint32> &xrefs)
            : m_index(index), m_father(father), m_text(text),
              m_bottomFlag(bottomFlag), m_level(level), m_title(title),
              m_hdps(hdps), m_xrefs(xrefs)
{
}

// Custom streaming operator (for debug)
QDebug operator<<(QDebug dbg, const Hierarchy &hierarchy)
{
    const QString contents = hierarchy.title() + ": index = " + QString::number(hierarchy.index());
    if (contents.isEmpty())
        dbg.nospace().noquote() << "Hierarchy()";
    else {
        dbg.nospace().noquote() << "Hierarchy(" << contents << ")";
        QVector<quint32> references = hierarchy.xrefs();
        if (!references.isEmpty()) {
            dbg.nospace().noquote() << " with references ";
            for (qint32 i = 0; i < references.size(); i++) {
                dbg.nospace().noquote() << references[i];
                if (i != references.size() - 1) dbg.nospace().noquote() << ", ";
            }
        }
    }

    return dbg.maybeSpace();
}

// Get functions ------------------------------------------------------------------------------------------------------
quint32 Hierarchy::index() const
{
    return m_index;
}

bool Hierarchy::bottomFlag() const
{
    return m_bottomFlag;
}

QString Hierarchy::title() const
{
    return m_title;
}

QVector<quint32> Hierarchy::hdps() const
{
    return m_hdps;
}

// Note: The cross-references are already populated.  Each reference points to an
// heirarchical record index.
QVector<quint32> Hierarchy::xrefs() const
{
    return m_xrefs;
}


