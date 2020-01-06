/************************************************************************

    datasetdialog.cpp

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

#include "datasetdialog.h"
#include "ui_datasetdialog.h"

DataSetDialog::DataSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataSetDialog)
{
    ui->setupUi(this);
}

DataSetDialog::~DataSetDialog()
{
    delete ui;
}

// Show a national data set item
void DataSetDialog::showDataSet(Names namesRecord)
{
    if (namesRecord.itemType() != 6 && namesRecord.itemType() != 7) {
        qDebug() << "namesRecord is not a valid data set!";
        return;
    }

    qDebug() << "Showing national data set:";
    qDebug() << "    Item label:" << namesRecord.itemName();
    qDebug() << "     Item type:" << namesRecord.itemType();
    qDebug() << "  Item address:" << namesRecord.itemAddress();

    // Get the essay from the data file based on the names record's item address
    DataFile dataFile("/home/sdi/tmp/Indiv_files/National/DATA1",
                        "/home/sdi/tmp/Indiv_files/National/DATA2");

    DataSet dataSet = dataFile.readDataSetRecord(namesRecord.itemAddress());
    qDebug() << "Got" << dataSet;
}
