/************************************************************************

    mainwindow.h

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtDebug>
#include <QLabel>
#include <QTreeView>
#include <QStandardItem>

#include "configuration.h"
#include "aboutdialog.h"
#include "Overlays/essaydialog.h"
#include "Overlays/picturesetdialog.h"
#include "Overlays/datasetdialog.h"
#include "Overlays/walkdialog.h"
#include "nationalmodel.h"
#include "hierarchysortfilter.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QDir _nationalFileLocation, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_AIV_data_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_OpenAIV_triggered();
    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_clearSearch_pushButton_clicked();
    void on_search_lineEdit_returnPressed();

    void on_type_gmap_checkBox_clicked();
    void on_type_amap_checkBox_clicked();
    void on_type_data_checkBox_clicked();
    void on_type_text_checkBox_clicked();
    void on_type_picture_checkBox_clicked();
    void on_type_walk_checkBox_clicked();
    void on_type_film_checkBox_clicked();    
    void on_type_selectall_checkBox_clicked();

    void on_tree_expand_pushButton_clicked();
    void on_tree_collapse_pushButton_clicked();

private:
    Ui::MainWindow *ui;

    // Dialogues
    AboutDialog* aboutDialog;
    EssayDialog* essayDialog;
    PictureSetDialog* pictureSetDialog;
    DataSetDialog* dataSetDialog;
    WalkDialog* walkDialog;

    // Class globals
    QLabel applicationStatus;
    Configuration configuration;
    NationalModel *nationalModel;
    HierarchySortFilter *hierarchySortFilter;

    QDir nationalFileLocation;

    void updateHierarchyView();
    void setFilter(QString filterString);
};
#endif // MAINWINDOW_H
