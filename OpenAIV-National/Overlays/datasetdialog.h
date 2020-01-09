/************************************************************************

    datasetdialog.h

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

#ifndef DATASETDIALOG_H
#define DATASETDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QGroupBox>
#include <QLayout>
#include <QCheckBox>
#include <QRadioButton>

#include "Datatypes/names.h"
#include "Filetypes/datafile.h"

namespace Ui {
class DataSetDialog;
}

class DataSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataSetDialog(QWidget *parent = nullptr);
    ~DataSetDialog();

    void showDataSet(Names namesRecord);

private slots:
    void dimensionCheckBoxChanged(bool state);

private:
    Ui::DataSetDialog *ui;

    QGroupBox* variableSelectionGroupBox;
    QVBoxLayout* variableSelectionGroupBoxLayout;
    QVector<QRadioButton*> variableSelectionRadioButtons;

    QVector<QGroupBox*> variableGroupBoxes;
    QVector<QVBoxLayout*> variableGroupBoxLayouts;
    QVector<QVector<QCheckBox*>> dimensionCheckBoxes;

    void clearDataSet();
    void generateDataView(DataSet dataSet);
};

#endif // DATASETDIALOG_H
