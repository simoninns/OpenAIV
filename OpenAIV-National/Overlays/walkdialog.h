/************************************************************************

    walkdialog.h

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

#ifndef WALKDIALOG_H
#define WALKDIALOG_H

#include <QDialog>
#include <QDir>

#include "Datatypes/names.h"
#include "Filetypes/datafile.h"

namespace Ui {
class WalkDialog;
}

class WalkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WalkDialog(QWidget *parent = nullptr);
    ~WalkDialog();

    void showWalk(Names namesRecord, QDir nationalFileDirectory);

private:
    Ui::WalkDialog *ui;

    Walk currentWalk;
};

#endif // WALKDIALOG_H
