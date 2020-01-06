#include "nationalessaydialog.h"
#include "ui_nationalessaydialog.h"

NationalEssayDialog::NationalEssayDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NationalEssayDialog)
{
    ui->setupUi(this);
}

NationalEssayDialog::~NationalEssayDialog()
{
    delete ui;
}

// Show a national essay item
void NationalEssayDialog::showEssay(Names namesRecord)
{
    qDebug() << "Showing national essay:";
    qDebug() << "    Item label:" << namesRecord.itemName();
    qDebug() << "     Item type:" << namesRecord.itemType();
    qDebug() << "  Item address:" << namesRecord.itemAddress();

    // Get the essay
    EssayFile essayFile("/home/sdi/tmp/Indiv_files/National/DATA1",
                        "/home/sdi/tmp/Indiv_files/National/DATA2");

    Essay essay = essayFile.readEssay(namesRecord.itemAddress());
    qDebug() << "Got" << essay.numberOfPages() << "pages from essay file";

    ui->textBrowser->clear();
    ui->textBrowser->setLineWrapMode(QTextEdit::NoWrap);
    ui->textBrowser->setText(formatEssay(essay));
}

// Format essay text for display
QString NationalEssayDialog::formatEssay(Essay essay)
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
