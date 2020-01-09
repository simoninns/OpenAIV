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
    // Ensure dynamic objects are cleared before exit
    clearDataSet();
    delete ui;
}

// Show a national data set item
void DataSetDialog::showDataSet(Names namesRecord)
{
    if (namesRecord.itemType() != 4) {
        qDebug() << "namesRecord is not a valid data set!";
        return;
    }

    qDebug() << "Showing national data set:";
    qDebug() << "    Item label:" << namesRecord.itemName();
    qDebug() << "     Item type:" << namesRecord.itemType();
    qDebug() << "  Item address:" << namesRecord.itemAddress();

    // Clear the current GUI
    clearDataSet();

    // Get the essay from the data file based on the names record's item address
    DataFile dataFile("/home/sdi/tmp/Indiv_files/National/DATA1",
                        "/home/sdi/tmp/Indiv_files/National/DATA2");

    DataSet dataSet = dataFile.readDataSetRecord(namesRecord.itemAddress());
    qDebug() << "Got" << dataSet;

    // Render the required UI
    this->setWindowTitle("National Chart Data Set - " + dataSet.chartLabels()[0]);

    // Process each chart variable in turn
    qDebug() << "There are" << dataSet.variableLabels().size() << "variables in the dataset";

    // Add a primary variable selection - this allows the user to pick the required chart
    // variable for the rendered view of the data
    variableSelectionGroupBoxLayout = new QVBoxLayout;
    variableSelectionGroupBox = new QGroupBox(this);
    variableSelectionGroupBox->setTitle("Primary variable:");
    for (qint32 i = 0; i < dataSet.variableLabels().size(); i++) {
        variableSelectionRadioButtons.append(new QRadioButton(this));
        variableSelectionRadioButtons.last()->setText(dataSet.variableLabels()[i]);
        if (i == 0) variableSelectionRadioButtons.last()->setChecked(true);
        else variableSelectionRadioButtons.last()->setChecked(false);
        variableSelectionRadioButtons.last()->setProperty("variable", i);

        variableSelectionGroupBoxLayout->layout()->addWidget(variableSelectionRadioButtons.last());
    }
    variableSelectionGroupBox->setLayout(variableSelectionGroupBoxLayout);
    ui->options_verticalLayout2->addWidget(variableSelectionGroupBox);

    // Add checkboxes to allow the user to select from the available chart variable dimensions
    // including a 'select all' box to make things easy.
    dimensionCheckBoxes.resize(dataSet.variableLabels().size()); // Set to the number of variables
    for (qint32 i = 0; i < dataSet.variableLabels().size(); i++) {
        // Add a label for the chart variable
        variableGroupBoxes.append(new QGroupBox(this));
        variableGroupBoxes.last()->setTitle(dataSet.variableLabels()[i] + ":");

        // Generate a layout object
        variableGroupBoxLayouts.append(new QVBoxLayout());

        // Add labels for the chart variable dimensions
        for (qint32 j = 0; j < dataSet.dimensionLabels()[i].size(); j++) {
            dimensionCheckBoxes[i].append(new QCheckBox(this));
            dimensionCheckBoxes[i].last()->setText(dataSet.dimensionLabels()[i][j]);
            dimensionCheckBoxes[i].last()->setChecked(true);

            // Store the variable and dimension indexes so we can identify
            // the checkbox later
            dimensionCheckBoxes[i].last()->setProperty("variable", i);
            dimensionCheckBoxes[i].last()->setProperty("dimension", j);

            variableGroupBoxLayouts.last()->layout()->addWidget(dimensionCheckBoxes[i].last());

            // Connect a signal to the checkbox
            connect(dimensionCheckBoxes[i].last(), SIGNAL(toggled(bool)), this, SLOT(dimensionCheckBoxChanged(bool)));
        }

        // Add an 'all' checkbox to the chart variable
        dimensionCheckBoxes[i].append(new QCheckBox(this));
        dimensionCheckBoxes[i].last()->setText("Select all");
        dimensionCheckBoxes[i].last()->setChecked(true);

        // Store the variable and dimension indexes so we can identify
        // the checkbox later (-1 indicates it's a 'All' checkbox for the variable)
        dimensionCheckBoxes[i].last()->setProperty("variable", i);
        dimensionCheckBoxes[i].last()->setProperty("dimension", -1);

        variableGroupBoxLayouts.last()->layout()->addWidget(dimensionCheckBoxes[i].last());

        // Connect a signal to the checkbox
        connect(dimensionCheckBoxes[i].last(), SIGNAL(toggled(bool)), this, SLOT(dimensionCheckBoxChanged(bool)));

        // Add the resulting group box to the parent object in the form
        variableGroupBoxes.last()->setLayout(variableGroupBoxLayouts.last());
        ui->options_verticalLayout2->addWidget(variableGroupBoxes.last());
    }

    // Generate the data views
    generateDataView(dataSet);
}

// Method to remove all dynamically created objects from the dialogue
void DataSetDialog::clearDataSet()
{
    // Variable selection tidy-up:
    qDebug() << "Clearing data set";
    if (variableSelectionRadioButtons.size() > 0) {
        ui->options_verticalLayout2->removeWidget(variableSelectionGroupBox);
        for (qint32 i = 0; i < variableSelectionRadioButtons.size(); i++) {
            delete variableSelectionRadioButtons[i];
        }
        delete variableSelectionGroupBoxLayout;
        delete variableSelectionGroupBox;
        variableSelectionRadioButtons.clear();
    }

    // Dimenension selection tidy-up:
    // Remove the dynamically added group boxes
    for (qint32 i = 0; i < variableGroupBoxes.size(); i++) {
        ui->options_verticalLayout2->removeWidget(variableGroupBoxes[i]);
    }

    // Clean up the dynamic objects
    for (qint32 i = 0; i < variableGroupBoxes.size(); i++) {
        for (qint32 j = 0; j < dimensionCheckBoxes[i].size(); j++) {
            delete dimensionCheckBoxes[i][j];
        }
        delete variableGroupBoxLayouts[i];
        delete variableGroupBoxes[i];
    }

    variableGroupBoxes.clear();
    variableGroupBoxLayouts.clear();
    dimensionCheckBoxes.clear();

    this->setWindowTitle("National Chart Data Set");
}

// Method to generate a view of the data based on the user's selections
void DataSetDialog::generateDataView(DataSet dataSet)
{
    qDebug() << "Generating data view in table widget";
    qDebug() << "  Dataset name:" << dataSet.chartLabels()[0];
    qDebug() << "  Primary unit:" << dataSet.chartLabels()[1];
    qDebug() << "  Total data points:" << dataSet.dataPoints().size();

    // Display the primary variable label
    qint32 primaryVariable = 0;
    for (qint32 i = 0; i < dataSet.variableLabels().size(); i++) {
        if (variableSelectionRadioButtons[i]->isChecked()) primaryVariable = i;
    }
    qDebug() << "  Primary variable:" << dataSet.variableLabels()[primaryVariable];
    QVector<qreal> primaryVariableTotals;
    primaryVariableTotals.fill(0, dataSet.dimensionLabels()[primaryVariable].size());

    QVector<qint32> dimCounter;
    dimCounter.fill(0, dataSet.variableLabels().size());

    // Display the n-dimensional data according to the dimension labeling
    for (qint32 dp = 0; dp < dataSet.dataPoints().size(); dp++) {
        QString debugS;
        for (qint32 cat = 0; cat < dataSet.variableLabels().size(); cat++) {
            debugS += dataSet.dimensionLabels()[cat][dimCounter[cat]] + " - ";
        }

        debugS += "dataPoint = " + QString::number(dataSet.dataPoints()[dp], 'f', 2);

        for (qint32 cat = dataSet.variableLabels().size() - 1; cat >= 0; cat--) {
            if (cat == dataSet.variableLabels().size() - 1) {
                dimCounter[cat]++;

                if (dimCounter[cat] > dataSet.dimensionLabels()[cat].size() - 1) {
                    dimCounter[cat] = 0;
                    if (cat != 0) dimCounter[cat - 1]++;
                }
            } else {
                if (dimCounter[cat] > dataSet.dimensionLabels()[cat].size() - 1) {
                    dimCounter[cat] = 0;
                    if (cat != 0) dimCounter[cat - 1]++;
                }
            }
        }

        qDebug() << "DP" << dp << ": " << debugS;
        debugS.clear();

        // Add data point to primary variable totals
        primaryVariableTotals[dimCounter[primaryVariable]] += dataSet.dataPoints()[dp];
    }

    // Display the resulting data
    qDebug() << "Result for" << dataSet.variableLabels()[primaryVariable];
    qreal dataSum = 0;
    for (qint32 i = 0; i < primaryVariableTotals.size(); i++) {
        qDebug() << dataSet.dimensionLabels()[primaryVariable][i] << "=" << QString::number(primaryVariableTotals[i], 'f', 2);
        dataSum += primaryVariableTotals[i];
    }
    qDebug() << "Datasum =" << QString::number(dataSum, 'f', 2);

    // Disable editing
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setColumnCount(3);

}

// Method to handle signal from the chart dimension checkboxes
void DataSetDialog::dimensionCheckBoxChanged(bool state)
{
    // Get the sender object's details (so we know which check box was changed)
    QObject * senderObject = sender();
    qint32 chartVariable = senderObject->property("variable").toInt();
    qint32 chartVariableDimension = senderObject->property("dimension").toInt();
    //qDebug() << "Chart Variable" << chartVariable << "dimension" << chartVariableDimension << "toggled to" << state;

    // Temporarily disable signals from the checkbox group
    for (qint32 i = 0; i < dimensionCheckBoxes[chartVariable].size(); i++) {
        dimensionCheckBoxes[chartVariable][i]->blockSignals(true);
    }

    // Select all toggled for a chart variable?
    if (chartVariableDimension == -1) {
        for (qint32 i = 0; i < dimensionCheckBoxes[chartVariable].size() - 1; i++) {
            dimensionCheckBoxes[chartVariable][i]->setChecked(state);
        }
    }

    // If it's not a select all, make sure select all is unchecked if individual
    // entries are unchecked
    if (chartVariableDimension != -1) {
        if (!state) {
            // The select all is always the last checkbox of the chart variable
            dimensionCheckBoxes[chartVariable][dimensionCheckBoxes[chartVariable].size() - 1]->setChecked(false);
        }
    }

    // Enable signals from the checkbox group
    for (qint32 i = 0; i < dimensionCheckBoxes[chartVariable].size(); i++) {
        dimensionCheckBoxes[chartVariable][i]->blockSignals(false);
    }
}
