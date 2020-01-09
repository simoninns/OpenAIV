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

// Includes for QtCharts
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

#include "Datatypes/names.h"
#include "Filetypes/datafile.h"

QT_CHARTS_USE_NAMESPACE

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
    void variableRadioButtonChanged(bool state);

private:
    Ui::DataSetDialog *ui;

    QGroupBox* variableSelectionGroupBox;
    QVBoxLayout* variableSelectionGroupBoxLayout;
    QVector<QRadioButton*> variableSelectionRadioButtons;

    QVector<QGroupBox*> variableGroupBoxes;
    QVector<QVBoxLayout*> variableGroupBoxLayouts;
    QVector<QVector<QCheckBox*>> dimensionCheckBoxes;

    qint32 primaryVariable;
    DataSet dataSet;

    // Supported chart types
    enum ChartType {
        CT_BarGraph,        // Simple bar chart
        CT_BackToBack,      // Back to back bar chart
        CT_Looping,         // Looping Stacked Bar Chart
        CT_PieChart,        // Pie chart
        CT_LineGraph,       // Single line Time Series Line Graph
        CT_MultiLineGraph   // Multi-line Time Series Line Graph
    };

    // Globals for charting
    QBarSet *barSet;
    QBarSeries *barSeries;
    QBarCategoryAxis *axisX;
    QValueAxis *axisY;
    QChart *chart;
    QChartView *chartView;
    bool chartWidgetExists;

    void clearDataSet();
    void generateDataView();
    void updateTable(QVector<qreal> primaryVariableTotals);
    void updateChart(QVector<qreal> primaryVariableTotals);
};

#endif // DATASETDIALOG_H
