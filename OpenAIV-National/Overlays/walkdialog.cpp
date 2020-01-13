/************************************************************************

    walkdialog.cpp

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

#include "walkdialog.h"
#include "ui_walkdialog.h"

WalkDialog::WalkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WalkDialog)
{
    ui->setupUi(this);
}

WalkDialog::~WalkDialog()
{
    delete ui;
}

void WalkDialog::showWalk(Names namesRecord, QDir nationalFileDirectory)
{
    if (namesRecord.itemType() != 9) {
        qDebug() << "namesRecord is not a valid walk!";
        return;
    }

    qDebug() << "Showing national walk:";
    qDebug() << "    Item label:" << namesRecord.itemName();
    qDebug() << "     Item type:" << namesRecord.itemType();
    qDebug() << "  Item address:" << namesRecord.itemAddress();

    // Get the essay from the data file based on the names record's item address
    QFileInfo data1FileInfo(nationalFileDirectory, "DATA1");
    QFileInfo data2FileInfo(nationalFileDirectory, "DATA2");
    DataFile dataFile(data1FileInfo.filePath(), data2FileInfo.filePath());

    currentWalk = dataFile.readWalkRecord(namesRecord.itemAddress());
}
