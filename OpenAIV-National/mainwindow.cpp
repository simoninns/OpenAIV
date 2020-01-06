/************************************************************************

    mainwindow.cpp

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

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create dialogues
    aboutDialog = new AboutDialog(this);
    essayDialog = new EssayDialog(this);
    pictureSetDialog = new PictureSetDialog(this);

    // Add a status bar to show the state of the source video file
    ui->statusbar->addWidget(&applicationStatus);
    applicationStatus.setText(tr("No AIV data is loaded"));

    // Load the window geometry and settings from the configuration
    restoreGeometry(configuration.getMainWindowGeometry());

    ui->treeView->setModel(&nationalModel);
    ui->treeView->expandAll();
    applicationStatus.setText(tr("Loaded ") + QString::number(nationalModel.totalHierarchyRecords()) +
                              tr(" hierarchy records containing ") + QString::number(nationalModel.totalNamesRecords()) +
                              tr(" names records"));
}

MainWindow::~MainWindow()
{
    // Remove dialogues
    delete aboutDialog;
    delete essayDialog;
    delete pictureSetDialog;

    // Remove the UI
    delete ui;
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

// Double-click on treeView
void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    NationalItem* clickedItem = reinterpret_cast<NationalItem *>(index.internalPointer());

    // Check that the clicked item is a names record
    QString typeName = QString::fromUtf8(clickedItem->data(0).typeName());
    // If the column is a Names object, return the names record label string
    if (typeName == "Names") {
        Names namesRecord = clickedItem->data(0).value<Names>();

        // If the type is currently supported by this application, pop it in a dialogue
        if (namesRecord.itemType() == 6 || namesRecord.itemType() == 7) {
            // Essay record
            qDebug() << "User clicked on" << namesRecord.itemName() << "with type" << namesRecord.itemTypeDescription() <<
                        "- opening essay dialogue";
            essayDialog->showEssay(namesRecord);
            essayDialog->show();
        } else if (namesRecord.itemType() == 8) {
            // Picture set record
            qDebug() << "User clicked on" << namesRecord.itemName() << "with type" << namesRecord.itemTypeDescription() <<
                        "- opening picture set dialogue";
            pictureSetDialog->showPictureSet(namesRecord);
            pictureSetDialog->show();
        } else {
            qDebug() << "Type" << namesRecord.itemTypeDescription() << "is not yet supported by OpenAIV";
        }
    }
}
