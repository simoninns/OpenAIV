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

    currentEssay = dataFile.readEssayRecord(namesRecord.itemAddress());
    qDebug() << "Got" << currentEssay.numberOfPages() << "pages from essay file";

    displayEssay();
}

// Format essay text and display
void EssayDialog::displayEssay()
{
    // If you set this to display without line endings, it takes
    // a very long time... need to fix it somehow

    qDebug() << "Formatting and displaying essay text";
    QTextCharFormat normalText;
    QTextCharFormat headlineText;
    QTextCharFormat linkText;

    normalText.setFontWeight(QFont::Normal);
    headlineText.setFontWeight(QFont::Bold);
    linkText.setFontWeight(QFont::Normal);

    normalText.setForeground(Qt::darkBlue);
    headlineText.setForeground(Qt::darkYellow);
    linkText.setForeground(Qt::darkGreen);

    // Get the textEdit widget ready
    ui->textBrowser->setTextColor(Qt::darkBlue);
    ui->textBrowser->clear();

    // Process the essay one page at a time
    ui->textBrowser->setUpdatesEnabled(false);
    bool performMarkUp = ui->markUp_checkBox->isChecked();
    bool insertLineEndings = ui->fixedWidth_checkBox->isChecked();
    QTextCursor cursor = ui->textBrowser->textCursor();

    for (qint32 page = 0; page < currentEssay.numberOfPages(); page++) {
        // Output the text 39 characters at a time
        qint32 lineCounter = 0;
        for (qint32 c = 0; c < currentEssay.pages()[page].size(); c++) {
            QString currentChar = currentEssay.pages()[page].mid(c, 1);

            // Perform mark up
            bool ignoreChar = false;
            if (performMarkUp) {
                if (currentChar == "{") {
                    cursor.setCharFormat(headlineText);
                    ignoreChar = true;
                }
                if (currentChar == "}") {
                    cursor.setCharFormat(normalText);
                    ignoreChar = true;
                }
                if (currentChar == "[") cursor.setCharFormat(linkText);
                if (currentChar == "]") cursor.setCharFormat(normalText);
            }

            if (!ignoreChar) cursor.insertText(currentChar);

            lineCounter++;
            if (lineCounter == 39 && insertLineEndings) {
                cursor.insertText("\n");
                lineCounter = 0;
            }
        }
        // End of current page
    }

    // End of current essay
    cursor.setPosition(0);
    ui->textBrowser->setTextCursor(cursor);
    ui->textBrowser->setUpdatesEnabled(true);
}

// Fixed width text checkbox clicked
void EssayDialog::on_fixedWidth_checkBox_clicked()
{
    displayEssay();
}

// Mark up checkbox clicked
void EssayDialog::on_markUp_checkBox_clicked()
{
    displayEssay();
}
