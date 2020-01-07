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

Names::Names(const QString &itemName, const qint32 &itemType, const quint32 &itemAddress)
            : m_itemName(itemName), m_itemType(itemType), m_itemAddress(itemAddress)
{
}

// Custom streaming operator (for debug)
QDebug operator<<(QDebug dbg, const Names &names)
{
    const QString contents = names.itemName() + ": type = " + names.itemTypeDescription();
    if (contents.isEmpty())
        dbg.nospace().noquote() << "Names()";
    else
        dbg.nospace().noquote() << "Names(" << contents << ")";

    return dbg.maybeSpace();
}

// Get functions ------------------------------------------------------------------------------------------------------
QString Names::itemName() const
{
    return m_itemName;
}

// Returns both the type and the name as descriptive text
QString Names::itemTypeAndItemName() const
{
    return itemTypeDescription() + ": " + m_itemName;
}

qint32 Names::itemType() const
{
    return m_itemType;
}

// Return a descriptive name for the type
QString Names::itemTypeDescription() const
{
    QString description;

    switch(m_itemType){
    case 1: // GMap
        description = "GMap"; // Grid-Mappable data
        break;
    case 3: // AMap
        description = "AMap"; // Areal-Mappable data
        break;
    case 4: // Data
        description = "Data";
        break;
//    case 5: // Plan
//        description = "Plan";
//        break;
    case 6: // Text type 1
    case 7: // Text type 2
        description = "Text";
        break;
    case 8: // Picture set
        description = "Pic.";
        break;
    case 9: // Surrogate walk
        description = "Walk";
        break;
    case 10:
        description = "Film";
        break;
    default:
        description = "Unknown";
        qDebug() << "Names type" << m_itemType << "is unknown!";
    }

    return description;
}

quint32 Names::itemAddress() const
{
    return m_itemAddress;
}
