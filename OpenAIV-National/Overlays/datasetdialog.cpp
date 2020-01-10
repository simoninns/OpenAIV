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

// TODO: Add other graphing types
// support non-additive data correctly
// Support save as CSV from table view
// Display dataset footnotes essay record

DataSetDialog::DataSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataSetDialog)
{
    ui->setupUi(this);

    chartWidgetExists = false;
}

DataSetDialog::~DataSetDialog()
{
    // Ensure dynamic objects are cleared before exit
    clearDataSet();

    delete ui;
}

// Show a national data set item
void DataSetDialog::showDataSet(Names namesRecord, QDir nationalFileDirectory)
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
    QFileInfo data1FileInfo(nationalFileDirectory, "DATA1");
    QFileInfo data2FileInfo(nationalFileDirectory, "DATA2");
    DataFile dataFile(data1FileInfo.filePath(), data2FileInfo.filePath());

    dataSet = dataFile.readDataSetRecord(namesRecord.itemAddress());
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

        // Connect a signal to the radiobutton
        connect(variableSelectionRadioButtons.last(), SIGNAL(toggled(bool)), this, SLOT(variableRadioButtonChanged(bool)));

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
    generateDataView();
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
    primaryVariable = 0;
}

// Method to generate a view of the data based on the user's selections
void DataSetDialog::generateDataView()
{
    // Get the details for the view rendering -------------------------------------------------------------------------

    qDebug() << "Generating data view in table widget";
    qDebug() << "  Dataset name:" << dataSet.chartLabels()[0];
    qDebug() << "  Primary unit:" << dataSet.chartLabels()[1];
    qDebug() << "  Total data points:" << dataSet.dataPoints().size();

    // Additive or non-additive data?
    if (dataSet.additiveDataFlag()) {
        qDebug() << "  Data is additive (WARNING: NOT CORRECTLY SUPPORTED YET!)";
    } else {
        qDebug() << "  Data is not additive";
    }

    // Get the primary variable label
    for (qint32 i = 0; i < dataSet.variableLabels().size(); i++) {
        if (variableSelectionRadioButtons[i]->isChecked()) primaryVariable = i;
    }
    qDebug() << "  Primary variable:" << dataSet.variableLabels()[primaryVariable];
    QVector<qreal> primaryVariableTotals;
    primaryVariableTotals.fill(0, dataSet.dimensionLabels()[primaryVariable].size());

    // Initialise the dimension counters
    QVector<qint32> dimensionCounter;
    dimensionCounter.fill(0, dataSet.variableLabels().size());

    // Create a matrix of the dimensions to be included from the checkbox selections
    QVector<QVector<bool>> dimensionMatrix;
    dimensionMatrix.resize(dataSet.variableLabels().size());
    for (qint32 vl = 0; vl < dataSet.variableLabels().size(); vl++) {
        dimensionMatrix[vl].resize(dataSet.dimensionLabels()[vl].size());

        for (qint32 dim = 0; dim < dataSet.dimensionLabels()[vl].size(); dim++) {
            if (dimensionCheckBoxes[vl][dim]->checkState()) dimensionMatrix[vl][dim] = true;
            else dimensionMatrix[vl][dim] = false;
            //qDebug() << dataSet.variableLabels()[vl] << "-" << dataSet.dimensionLabels()[vl][dim] << "=" << dimensionMatrix[vl][dim];
        }
    }

    // Get the required data point sums -------------------------------------------------------------------------------

    // For each dataPoint, work out the dimensions it belongs to and sum accordingly
    for (qint32 dataPointCounter = 0; dataPointCounter < dataSet.dataPoints().size(); dataPointCounter++) {
        QString debugS;
        bool includeDataPoint = true;
        for (qint32 chartVariable = 0; chartVariable < dataSet.variableLabels().size(); chartVariable++) {
            debugS += dataSet.dimensionLabels()[chartVariable][dimensionCounter[chartVariable]];

            // Is the dimension selected for inclusion?  If any dimensions included are not selected
            // the dataPoint should not be included.
            if (dimensionMatrix[chartVariable][dimensionCounter[chartVariable]]) {
                debugS += " T";
            } else {
                debugS += " F";
                includeDataPoint = false;
            }

            debugS +=  + " - ";
        }

        debugS += "dataPoint = " + QString::number(dataSet.dataPoints()[dataPointCounter], 'f', 2);

        // Add data point to primary variable totals if selected
        if (includeDataPoint) {
            primaryVariableTotals[dimensionCounter[primaryVariable]] += dataSet.dataPoints()[dataPointCounter];
            //qDebug() << "DP" << dataPointCounter << ": " << debugS;
        }
        debugS.clear();

        // Move to the next set of dimensions
        for (qint32 chartVariable = 0; chartVariable < dataSet.variableLabels().size(); chartVariable++) {
            if (chartVariable == 0) {
                dimensionCounter[chartVariable]++;

                if (dimensionCounter[chartVariable] > dataSet.dimensionLabels()[chartVariable].size() - 1) {
                    dimensionCounter[chartVariable] = 0;
                    if (chartVariable < dataSet.variableLabels().size() - 1) dimensionCounter[chartVariable + 1]++;
                }
            } else {
                if (dimensionCounter[chartVariable] > dataSet.dimensionLabels()[chartVariable].size() - 1) {
                    dimensionCounter[chartVariable] = 0;
                    if (chartVariable < dataSet.variableLabels().size() - 1) dimensionCounter[chartVariable + 1]++;
                }
            }
        }
    }

    // Update the overlay GUI -----------------------------------------------------------------------------------------
    updateTable(primaryVariableTotals);
    updateChart(primaryVariableTotals);
}

// Method to update the table view
void DataSetDialog::updateTable(QVector<qreal> primaryVariableTotals)
{
    // Disable editing of the table widget
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Clear all existing items
    ui->tableWidget->setRowCount(0);

    // Set the required number of columns and rows (including title and total)
    ui->tableWidget->setRowCount(primaryVariableTotals.size() + 2);
    ui->tableWidget->setColumnCount(2);

    // Display the resulting data in the tableWidget
    qDebug() << "Result for" << dataSet.variableLabels()[primaryVariable];
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(dataSet.variableLabels()[primaryVariable]));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(dataSet.chartLabels()[1]));
    ui->tableWidget->item(0, 0)->setBackgroundColor(Qt::lightGray);
    ui->tableWidget->item(0, 1)->setBackgroundColor(Qt::lightGray);
    qreal dataSum = 0;
    for (qint32 i = 0; i < primaryVariableTotals.size(); i++) {
        qDebug() << dataSet.dimensionLabels()[primaryVariable][i] << "=" << QString::number(primaryVariableTotals[i], 'f', 2);
        dataSum += primaryVariableTotals[i];
        ui->tableWidget->setItem(i + 1, 0, new QTableWidgetItem(dataSet.dimensionLabels()[primaryVariable][i]));
        ui->tableWidget->setItem(i + 1, 1, new QTableWidgetItem(QString::number(primaryVariableTotals[i], 'f', 2)));
    }
    qDebug() << "Datasum =" << QString::number(dataSum, 'f', 2);
    qint32 finalRow = static_cast<qint32>(primaryVariableTotals.size()) + 1;
    ui->tableWidget->setItem(finalRow, 0, new QTableWidgetItem("TOTAL"));
    ui->tableWidget->setItem(finalRow, 1, new QTableWidgetItem(QString::number(dataSum, 'f', 2)));
    ui->tableWidget->item(finalRow, 1)->setTextColor(Qt::darkGreen);
}

// Method to update the chart view
void DataSetDialog::updateChart(QVector<qreal> primaryVariableTotals)
{
    if (chartWidgetExists) {
        ui->chartView_verticalLayout->removeWidget(chartView);
        delete barSet;
        delete barSeries;
        delete chart;
        delete axisX;
        delete axisY;
        delete chartView;
        chartWidgetExists = false;
    }

    // Declare barchart objects
    barSet = new QBarSet("label");
    barSeries = new QBarSeries();
    chart = new QChart();
    axisX = new QBarCategoryAxis();
    axisY = new QValueAxis();
    chartView = new QChartView(chart);

    // There is a bug in Qt 5.9 under Windows where Qt Chart crashes if the
    // data set is all zeros due to the min and max of the axis both being
    // set to 0.0.  This is a work around...
    qreal chartSum = 0;
    for (qint32 i = 0; i < primaryVariableTotals.size(); i++) chartSum += primaryVariableTotals[i];
    if (chartSum == 0) axisY->setRange(0,1);
    // End of work around

    barSet->setLabel(dataSet.chartLabels()[1]);
    for (qint32 dp = 0; dp < primaryVariableTotals.size(); dp++) {
        barSet->append(primaryVariableTotals[dp]);
    }

    barSeries->append(barSet);
    chart->addSeries(barSeries);
    chart->setTitle(dataSet.chartLabels()[0]);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList varTitles;
    for (qint32 vl = 0; vl < dataSet.dimensionLabels()[primaryVariable].size(); vl++) {
        varTitles.append(dataSet.dimensionLabels()[primaryVariable][vl]);
    }

    axisX->append(varTitles);
    chart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    chart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    chartView->setRenderHint(QPainter::Antialiasing);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    ui->chartView_verticalLayout->addWidget(chartView);
    chartWidgetExists = true;
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

    // Regenerate the view
    generateDataView();
}

// Method to handle signal from the chart variable radiobuttons
void DataSetDialog::variableRadioButtonChanged(bool state)
{
    // Get the sender object's details (so we know which check box was changed)
    QObject * senderObject = sender();
    qint32 chartVariable = senderObject->property("variable").toInt();
    qDebug() << "Chart Variable" << chartVariable << "toggled to" << state;

    if (state) {
        primaryVariable = chartVariable;

        // Regenerate the view
        generateDataView();
    }
}
