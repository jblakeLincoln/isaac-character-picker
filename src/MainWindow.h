#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <string>
#include <QElapsedTimer>
#include <QCheckBox>
#include <QGraphicsEffect>
#include <QRgb>
#include <QMessageBox>
#include <time.h>
#include <QIcon>
#include <QTimer>
#include <QTime>

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
    QLabel* img_Characters[NUM_CHARACTERS];

    void SetTitle();
    void SetCharacters();
    void SetCheckboxes();
    void Animate();
    void ShowMessageBox();

    QPushButton* btn_Spin;

    bool m_Spinning;
    QTimer* m_UpdateTimer;
    QTime* m_ElapsedTimer;
    double m_Degrees;
    double m_TargetDegrees;
    double m_SpinningStartTime;
    int m_SelectedCharacter;

     QGraphicsOpacityEffect* m_OpacityEffect;
    
private slots:
    void BtnSpin_Clicked();
    void Update();

    void PositionCharacters(double theta);
    void Box_Toggled(bool checked);
};

#endif // MAINWINDOW_H
