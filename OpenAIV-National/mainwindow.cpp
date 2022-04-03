/************************************************************************

    mainwindow.cpp

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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QDir _nationalFileLocation, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    nationalFileLocation = _nationalFileLocation;

    // Create dialogues
    aboutDialog = new AboutDialog(this);
    essayDialog = new EssayDialog(this);
    pictureSetDialog = new PictureSetDialog(nationalFileLocation, this);
    dataSetDialog = new DataSetDialog(this);
    walkDialog = new WalkDialog(this);

    // Add a status bar to show the state of the source video file
    ui->statusbar->addWidget(&applicationStatus);
    applicationStatus.setText(tr("No AIV data is loaded"));

    // Load the window geometry and settings from the configuration
    restoreGeometry(configuration.getMainWindowGeometry());
    essayDialog->restoreGeometry(configuration.getEssayDialogGeometry());
    pictureSetDialog->restoreGeometry(configuration.getPictureSetDialogGeometry());
    dataSetDialog->restoreGeometry(configuration.getDataSetDialogGeometry());
    walkDialog->restoreGeometry(configuration.getWalkDialogGeometry());

    nationalModel = new NationalModel(nationalFileLocation);
    hierarchySortFilter = new HierarchySortFilter(this);
    hierarchySortFilter->setSourceModel(nationalModel);
    ui->treeView->setModel(hierarchySortFilter);

    updateHierarchyView();
}

MainWindow::~MainWindow()
{
    // Save the window and dialog geometry to the configuration
    configuration.setMainWindowGeometry(saveGeometry());
    configuration.setEssayDialogGeometry(essayDialog->saveGeometry());
    configuration.setPictureSetDialogGeometry(pictureSetDialog->saveGeometry());
    configuration.setDataSetDialogGeometry(dataSetDialog->saveGeometry());
    configuration.setWalkDialogGeometry(walkDialog->saveGeometry());
    configuration.writeConfiguration();

    // Remove dialogues
    delete aboutDialog;
    delete essayDialog;
    delete pictureSetDialog;
    delete dataSetDialog;

    // Remove the models
    delete nationalModel;

    // Remove the UI
    delete ui;
}

void MainWindow::updateHierarchyView()
{
    hierarchySortFilter->forceUpdate();
    ui->treeView->expandAll();
    applicationStatus.setText(tr("Loaded ") + QString::number(nationalModel->totalHierarchyRecords()) +
                              tr(" hierarchy categories containing ") + QString::number(nationalModel->totalNamesRecords()) +
                              tr(" named items"));
}

// Menu bar signal handlers -------------------------------------------------------------------------------------------

// File->Open AIV data menu bar action
void MainWindow::on_actionOpen_AIV_data_triggered()
{
}

// File->Exit menu bar action
void MainWindow::on_actionExit_triggered()
{
    qDebug() << "Menu bar, file->Exit triggered";

    // Quit the application
    qApp->quit();
}

// Help->About menu bar action
void MainWindow::on_actionAbout_OpenAIV_triggered()
{
    // Show the about dialogue
    aboutDialog->show();
}

// GUI signal handlers

// Double-click on hierarchy treeView
void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    NationalItem* clickedItem = reinterpret_cast<NationalItem *>(hierarchySortFilter->mapToSource(index).internalPointer());
    NationalItem::ItemType itemType = clickedItem->itemType();

    // Check that the clicked item is a names record
    if (itemType != NationalItem::ItemType::unknown && itemType != NationalItem::ItemType::hieararchy) {
        Names namesRecord = clickedItem->data(0).value<Names>();

        // If the type is currently supported by this application, pop it in a dialogue
        if (itemType == NationalItem::ItemType::named_text) {
            // Essay record
            qDebug() << "User clicked on" << namesRecord.itemName() << "with type" << clickedItem->itemTypeAsString() <<
                        "- opening essay dialogue";
            essayDialog->showEssay(namesRecord, nationalFileLocation);
            essayDialog->show();
        } else if (itemType == NationalItem::ItemType::named_pic) {
            // Picture set record
            qDebug() << "User clicked on" << namesRecord.itemName() << "with type" << clickedItem->itemTypeAsString() <<
                        "- opening picture set dialogue";
            pictureSetDialog->showPictureSet(namesRecord);
            pictureSetDialog->show();
        } else if (itemType == NationalItem::ItemType::named_data) {
            // Data set record
            qDebug() << "User clicked on" << namesRecord.itemName() << "with type" << clickedItem->itemTypeAsString() <<
                        "- opening data set dialogue";
            dataSetDialog->showDataSet(namesRecord, nationalFileLocation);
            dataSetDialog->show();
        } else if (itemType == NationalItem::ItemType::named_walk) {
            // Data set record
            qDebug() << "User clicked on" << namesRecord.itemName() << "with type" << clickedItem->itemTypeAsString() <<
                        "- opening walk dialogue";
            walkDialog->showWalk(namesRecord, nationalFileLocation);
            walkDialog->show();
        } else {
            qDebug() << "Type" << clickedItem->itemTypeAsString() << "is not yet supported by OpenAIV";
        }
    }
}

// Hierarchy search clear button clicked
void MainWindow::on_clearSearch_pushButton_clicked()
{
    ui->search_lineEdit->clear();
    setFilter(QString());
    updateHierarchyView();
}

// Hierarchy search line editor (return pressed)
void MainWindow::on_search_lineEdit_returnPressed()
{
    setFilter(ui->search_lineEdit->text());
    updateHierarchyView();
}

// Expand all in tree
void MainWindow::on_tree_expand_pushButton_clicked()
{
    ui->treeView->expandAll();
}

// Collapse all in tree
void MainWindow::on_tree_collapse_pushButton_clicked()
{
    ui->treeView->collapseAll();
}

void MainWindow::setFilter(QString filterString)
{
    // Clear select all if something isn't checked
    if (!(ui->type_gmap_checkBox->isChecked() &&
            ui->type_amap_checkBox->isChecked() &&
            ui->type_data_checkBox->isChecked() &&
            ui->type_text_checkBox->isChecked() &&
            ui->type_picture_checkBox->isChecked() &&
            ui->type_walk_checkBox->isChecked() &&
            ui->type_film_checkBox->isChecked())) {
        ui->type_selectall_checkBox->blockSignals(true);
        ui->type_selectall_checkBox->setChecked(false);
        ui->type_selectall_checkBox->blockSignals(false);
    }

    hierarchySortFilter->setFilter(filterString,
                                   ui->type_gmap_checkBox->isChecked(),
                                   ui->type_amap_checkBox->isChecked(),
                                   ui->type_data_checkBox->isChecked(),
                                   ui->type_text_checkBox->isChecked(),
                                   ui->type_picture_checkBox->isChecked(),
                                   ui->type_walk_checkBox->isChecked(),
                                   ui->type_film_checkBox->isChecked());
}

// Record type selection checkbox handlers
void MainWindow::on_type_gmap_checkBox_clicked()
{
    setFilter(ui->search_lineEdit->text());
    updateHierarchyView();
}

void MainWindow::on_type_amap_checkBox_clicked()
{
    setFilter(ui->search_lineEdit->text());
    updateHierarchyView();
}

void MainWindow::on_type_data_checkBox_clicked()
{
    setFilter(ui->search_lineEdit->text());
    updateHierarchyView();
}

void MainWindow::on_type_text_checkBox_clicked()
{
    setFilter(ui->search_lineEdit->text());
    updateHierarchyView();
}

void MainWindow::on_type_picture_checkBox_clicked()
{
    setFilter(ui->search_lineEdit->text());
    updateHierarchyView();
}

void MainWindow::on_type_walk_checkBox_clicked()
{
    setFilter(ui->search_lineEdit->text());
    updateHierarchyView();
}

void MainWindow::on_type_film_checkBox_clicked()
{
    setFilter(ui->search_lineEdit->text());
    updateHierarchyView();
}

void MainWindow::on_type_selectall_checkBox_clicked()
{
    ui->type_gmap_checkBox->blockSignals(true);
    ui->type_amap_checkBox->blockSignals(true);
    ui->type_data_checkBox->blockSignals(true);
    ui->type_text_checkBox->blockSignals(true);
    ui->type_picture_checkBox->blockSignals(true);
    ui->type_walk_checkBox->blockSignals(true);
    ui->type_film_checkBox->blockSignals(true);

    if (ui->type_selectall_checkBox->isChecked()) {
        // Select all
        ui->type_gmap_checkBox->setChecked(true);
        ui->type_amap_checkBox->setChecked(true);
        ui->type_data_checkBox->setChecked(true);
        ui->type_text_checkBox->setChecked(true);
        ui->type_picture_checkBox->setChecked(true);
        ui->type_walk_checkBox->setChecked(true);
        ui->type_film_checkBox->setChecked(true);
    } else {
        // Select none
        ui->type_gmap_checkBox->setChecked(false);
        ui->type_amap_checkBox->setChecked(false);
        ui->type_data_checkBox->setChecked(false);
        ui->type_text_checkBox->setChecked(false);
        ui->type_picture_checkBox->setChecked(false);
        ui->type_walk_checkBox->setChecked(false);
        ui->type_film_checkBox->setChecked(false);
    }

    ui->type_gmap_checkBox->blockSignals(false);
    ui->type_amap_checkBox->blockSignals(false);
    ui->type_data_checkBox->blockSignals(false);
    ui->type_text_checkBox->blockSignals(false);
    ui->type_picture_checkBox->blockSignals(false);
    ui->type_walk_checkBox->blockSignals(false);
    ui->type_film_checkBox->blockSignals(false);

    setFilter(ui->search_lineEdit->text());
    updateHierarchyView();
}


