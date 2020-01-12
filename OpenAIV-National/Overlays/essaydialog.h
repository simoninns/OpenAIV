/************************************************************************

    essaydialog.h

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

#ifndef ESSAYDIALOG_H
#define ESSAYDIALOG_H

#include <QDialog>
#include <QDir>

#include "Datatypes/names.h"
#include "Filetypes/datafile.h"

namespace Ui {
class EssayDialog;
}

class EssayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EssayDialog(QWidget *parent = nullptr);
    ~EssayDialog();

    void showEssay(Names namesRecord, QDir nationalFileDirectory);

private slots:
    void on_fixedWidth_checkBox_clicked();

    void on_markUp_checkBox_clicked();

private:
    Ui::EssayDialog *ui;

    QString formatEssay(Essay essay);
};

#endif // ESSAYDIALOG_H
