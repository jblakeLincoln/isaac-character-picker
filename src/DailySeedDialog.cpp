#include "DailySeedDialog.h"
#include "ui_DailySeedDialog.h"

DailySeedDialog::DailySeedDialog(QString seed, QString date, QPixmap img, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DailySeedDialog)
{
    ui->setupUi(this);

    QFont font = ui->lbl_Seed->font();
    font.setPixelSize(24);
    font.setFamily("Upheaval TT (BRK)");
    ui->lbl_Seed->setFont(font);
    ui->lbl_Seed->setAlignment(Qt::AlignCenter);
    ui->lbl_Seed->setText(seed);
    ui->lbl_Seed->adjustSize();

    ui->lbl_Date->setText(date);
    ui->lbl_Date->setAlignment(Qt::AlignCenter);
    ui->lbl_Date->setFont(font);
    ui->lbl_Date->adjustSize();

    ui->img_Character->setAlignment(Qt::AlignCenter);
    ui->img_Character->setPixmap(img.scaled(128, 128, Qt::KeepAspectRatio));
    ui->img_Character->adjustSize();
}

DailySeedDialog::~DailySeedDialog()
{
    delete ui;
}
