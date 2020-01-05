#ifndef NATIONALESSAYDIALOG_H
#define NATIONALESSAYDIALOG_H

#include <QDialog>

#include "Datatypes/names.h"
#include "Filetypes/essayfile.h"

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
};

#endif // NATIONALESSAYDIALOG_H
