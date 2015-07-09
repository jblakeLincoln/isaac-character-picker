#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <string>
#include <QTimer>
#include <QCheckBox>
#include <QGraphicsEffect>
#include <QRgb>
#include <QMessageBox>
#include <time.h>
#include <QIcon>

const int NUM_CHARACTERS = 11; // Subject to change when Afterbirth releases.

const std::string characters[] = {
    "Isaac",
    "Maggy",
    "Cain",
    "Judas",
    "BlueBaby",
    "Eve",
    "Samson",
    "Azazel",
    "Lazarus",
    "Eden",
    "Lost",
};


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow* ui;
    bool bool_IsSelectable[NUM_CHARACTERS];
    QCheckBox* box_IsSelectable[NUM_CHARACTERS];

    QPixmap images[NUM_CHARACTERS];
    QLabel* img_Characters[10];

    void SetCharacters();
    void SetCheckboxes();
    
private slots:
    void BtnSpin_Clicked();
    void Update();

    void PositionCharacters(double theta);
    void Box_Toggled(bool checked);
};

#endif // MAINWINDOW_H
