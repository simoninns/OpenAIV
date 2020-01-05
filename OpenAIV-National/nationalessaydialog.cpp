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

    ui->textEdit->clear();
    QString text = "";
    for (qint32 page = 0; page < essay.numberOfPages(); page++) {
        text = text + essay.pages()[page];
    }

    // Make text fixed 39 character width
    for (qint32 i = 39; i <= text.size(); i += (39 + 1))
        text.insert(i, '\n');


    ui->textEdit->setText(text);
}
