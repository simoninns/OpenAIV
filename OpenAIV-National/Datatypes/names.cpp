/************************************************************************

    names.cpp

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

#include "names.h"

Names::Names(const QString &label, const qint32 &type, const qint32 &destinationOffset)
            : m_label(label), m_type(type), m_destinationOffset(destinationOffset)
{
}

// Custom streaming operator (for debug)
QDebug operator<<(QDebug dbg, const Names &names)
{
    const QString contents = names.label() + ": type = " + names.typeName();
    if (contents.isEmpty())
        dbg.nospace().noquote() << "Names()";
    else
        dbg.nospace().noquote() << "Names(" << contents << ")";

    return dbg.maybeSpace();
}

// Get functions ------------------------------------------------------------------------------------------------------
QString Names::label() const
{
    return m_label;
}

// Returns both the type and the label as descriptive text
QString Names::typeAndLabel() const
{
    return typeName() + ": " + m_label;
}

qint32 Names::type() const
{
    return m_type;
}

// Return a descriptive name for the type
QString Names::typeName() const
{
    QString description;

    switch(m_type){
    case 1: // Map type 1
        description = "Map";
        break;
    case 3: // Map type 2
        description = "Map";
        break;
    case 4: // Data
        description = "Data";
        break;
    case 6: // Text type 1
        description = "Text";
        break;
    case 7: // Text type 2
        description = "Text";
        break;
    case 8: // Picture set
        description = "Pic";
        break;
    case 9: // Surrogate walk
        description = "Walk";
        break;
    case 10:
        description = "Film";
        break;
    default:
        description = "Unknown";
        qDebug() << "Names type" << m_type << "is unknown!";
    }

    return description;
}

qint32 Names::destinationOffset() const
{
    return m_destinationOffset;
}
