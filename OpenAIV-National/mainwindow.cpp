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
