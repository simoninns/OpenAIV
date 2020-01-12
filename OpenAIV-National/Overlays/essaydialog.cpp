/************************************************************************

    essaydialog.cpp

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

#include "essaydialog.h"
#include "ui_essaydialog.h"

// TODO:
// Support better formatting of the 39 character fixed-width text
// Support picture links
// Support markdown {} and []
// Auto-fix simple formatting
// Auto-fix broken ASCII characters

EssayDialog::EssayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EssayDialog)
{
    ui->setupUi(this);
}

EssayDialog::~EssayDialog()
{
    delete ui;
}

// Show a national essay item
void EssayDialog::showEssay(Names namesRecord, QDir nationalFileDirectory)
{
    if (namesRecord.itemType() != 6 && namesRecord.itemType() != 7) {
        qDebug() << "namesRecord is not a valid essay!";
        return;
    }

    qDebug() << "Showing national essay:";
    qDebug() << "    Item label:" << namesRecord.itemName();
    qDebug() << "     Item type:" << namesRecord.itemType();
    qDebug() << "  Item address:" << namesRecord.itemAddress();

    // Get the essay from the data file based on the names record's item address
    QFileInfo data1FileInfo(nationalFileDirectory, "DATA1");
    QFileInfo data2FileInfo(nationalFileDirectory, "DATA2");
    DataFile dataFile(data1FileInfo.filePath(), data2FileInfo.filePath());

    Essay essay = dataFile.readEssayRecord(namesRecord.itemAddress());
    qDebug() << "Got" << essay.numberOfPages() << "pages from essay file";

    ui->textBrowser->clear();
    ui->textBrowser->setLineWrapMode(QTextEdit::NoWrap);
    ui->textBrowser->setText(formatEssay(essay));
}

// Format essay text for display
QString EssayDialog::formatEssay(Essay essay)
{
    QString browserText;

    // Concatenate essay pages
    for (qint32 page = 0; page < essay.numberOfPages(); page++) {
        browserText = browserText + essay.pages()[page];
    }

    // Format text with a fixed 39 characters per line
    for (qint32 i = 39; i <= browserText.size(); i += (39 + 1))
        browserText.insert(i, '\n');

    return browserText;
}

// Fixed width text checkbox clicked
void EssayDialog::on_fixedWidth_checkBox_clicked()
{

}

// Mark up checkbox clicked
void EssayDialog::on_markUp_checkBox_clicked()
{

}
