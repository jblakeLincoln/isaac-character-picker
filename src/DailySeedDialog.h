#ifndef DAILYSEEDDIALOG_H
#define DAILYSEEDDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class DailySeedDialog;
}

class DailySeedDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DailySeedDialog(QString seed, QString date, QPixmap img, QWidget *parent = 0);
    ~DailySeedDialog();

private:
    Ui::DailySeedDialog *ui;
};

#endif // DAILYSEEDDIALOG_H
