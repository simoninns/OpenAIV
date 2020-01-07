/************************************************************************

    picturesetdialog.cpp

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

#include "picturesetdialog.h"
#include "ui_picturesetdialog.h"

PictureSetDialog::PictureSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PictureSetDialog)
{
    ui->setupUi(this);
    currentPictureNumber = 0;

    // Set up video player
    mediaPlayer = new QMediaPlayer(this);
    mediaPlayer->setAudioRole(QAudio::VideoRole);
    mediaPlayer->setMedia(QUrl::fromLocalFile("/home/sdi/tmp/Indiv_files/National/national_ds4.mp4"));
    mediaPlayer->setVideoOutput(ui->videoWidget);
}

PictureSetDialog::~PictureSetDialog()
{
    delete mediaPlayer;
    delete ui;
}

// Show a national essay item
void PictureSetDialog::showPictureSet(Names namesRecord)
{
    if (namesRecord.itemType() != 8) {
        qDebug() << "namesRecord is not a valid picture set!";
        return;
    }

    qDebug() << "Showing national picture set:";
    qDebug() << "    Item label:" << namesRecord.itemName();
    qDebug() << "     Item type:" << namesRecord.itemType();
    qDebug() << "  Item address:" << namesRecord.itemAddress();

    // Get the picture set from the data file based on the names record's item address
    DataFile dataFile("/home/sdi/tmp/Indiv_files/National/DATA1",
                        "/home/sdi/tmp/Indiv_files/National/DATA2");

    pictureSet = dataFile.readPictureSetRecord(namesRecord.itemAddress());
    qDebug() << "Got" << pictureSet.numberOfPictures() << "pictures from picture set file";
    qDebug() << "PictureSet: " << pictureSet;

    // Start with the first picture
    currentPictureNumber = 1;
    showPicture(currentPictureNumber);
}

// Show a picture (expects picture number starting from 1)
void PictureSetDialog::showPicture(qint32 pictureNumber)
{
    // Ensure the picture number is valid
    if (pictureNumber < 1) return;
    if (pictureNumber > pictureSet.numberOfPictures()) return;

    // Show the short caption
    ui->shortCaption_label->setText(pictureSet.shortCaptions()[pictureNumber - 1]);

    // Show the long caption
    ui->longCaption_textEdit->setText(formatLongCaption(pictureSet.longCaptions()[pictureNumber - 1]));

    // Set the status label
    QString statusString = "Showing picture " + QString::number(pictureNumber) + " of " + QString::number(pictureSet.numberOfPictures()) +
            " - Frame #" + QString::number(pictureSet.frameNumbers()[pictureNumber - 1]);
    ui->status_label->setText(statusString);

    // Show the frame from the LaserDisc
    mediaPlayer->pause();
    mediaPlayer->setPosition(convertFrameToPosition(pictureSet.frameNumbers()[pictureNumber - 1]));
}

// Format essay text for display
QString PictureSetDialog::formatLongCaption(QString longCaption)
{
    // Format text with a fixed 39 characters per line
    for (qint32 i = 39; i <= longCaption.size(); i += (39 + 1))
        longCaption.insert(i, '\n');

    return longCaption;
}

// Methods to handle navigation clicks
void PictureSetDialog::on_firstPicture_pushButton_clicked()
{
    currentPictureNumber = 1;
    showPicture(currentPictureNumber);
}

void PictureSetDialog::on_last_pushButton_clicked()
{
    currentPictureNumber = pictureSet.numberOfPictures();
    showPicture(currentPictureNumber);
}

void PictureSetDialog::on_previous_pushButton_clicked()
{
    currentPictureNumber -= 1;
    if (currentPictureNumber < 1) {
        currentPictureNumber = 1;
        return;
    }
    showPicture(currentPictureNumber);
}

void PictureSetDialog::on_next_pushButton_clicked()
{
    currentPictureNumber += 1;
    if (currentPictureNumber > pictureSet.numberOfPictures()) {
        currentPictureNumber = pictureSet.numberOfPictures();
        return;
    }
    showPicture(currentPictureNumber);
}

// Method to convert a LaserDisc frame number to a media player position value
qint64 PictureSetDialog::convertFrameToPosition(qint32 frameNumber)
{
    // Position is milliseconds from the beginning of the video file

    // Video is PAL and always 25 frames per second, so 1000/25
    // is 40 milliseconds per frame

    return (static_cast<qint64>(frameNumber-1)) * 40;
}
