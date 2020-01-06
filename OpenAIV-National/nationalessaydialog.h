#ifndef NATIONALESSAYDIALOG_H
#define NATIONALESSAYDIALOG_H

#include <QDialog>
#include <QFontDatabase>

#include "Datatypes/names.h"
#include "Filetypes/datafile.h"

namespace Ui {
class NationalEssayDialog;
}

class NationalEssayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NationalEssayDialog(QWidget *parent = nullptr);
    ~NationalEssayDialog();

    void showEssay(Names namesRecord);

private:
    Ui::NationalEssayDialog *ui;

    QString formatEssay(Essay essay);
};

#endif // NATIONALESSAYDIALOG_H
