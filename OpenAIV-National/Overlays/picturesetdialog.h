/************************************************************************

    picturesetdialog.h

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

#ifndef PICTURESETDIALOG_H
#define PICTURESETDIALOG_H

#include <QDialog>
#include <QMediaPlayer>

#include "Datatypes/names.h"
#include "Filetypes/datafile.h"

namespace Ui {
class PictureSetDialog;
}

class PictureSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PictureSetDialog(QWidget *parent = nullptr);
    ~PictureSetDialog();

    void showPictureSet(Names namesRecord);

private slots:
    void on_firstPicture_pushButton_clicked();
    void on_last_pushButton_clicked();
    void on_previous_pushButton_clicked();
    void on_next_pushButton_clicked();

private:
    Ui::PictureSetDialog *ui;    
    qint32 currentPictureNumber;
    PictureSet pictureSet;

    // Video playback
    QMediaPlayer *mediaPlayer = nullptr;

    void showPicture(qint32 pictureNumber);
    QString formatLongCaption(QString longCaption);
    qint64 convertFrameToPosition(qint32 frameNumber);
};

#endif // PICTURESETDIALOG_H
