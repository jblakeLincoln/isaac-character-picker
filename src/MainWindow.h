#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>

#include <QMessageBox>
#include <QTimer>
#include <QTime>

#include <QDir>
#include <QFileDialog>
#include <QFontDatabase>
#include <QGraphicsEffect>
#include <QSettings>

#include <cmath>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <time.h>

#include "DailySeedDialog.h"

const int NUM_CHARACTERS = 11; // Subject to change when Afterbirth releases.
const int NUM_EDEN_SPRITES = 10;

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
    //bool bool_IsSelectable[NUM_CHARACTERS];
    QCheckBox* box_IsSelectable[NUM_CHARACTERS];

    QPixmap images[NUM_CHARACTERS];
    QLabel* img_Characters[NUM_CHARACTERS];

    void SetTitle();
    void SetCharacters();
    void SetCheckboxes();
    void Animate();
    void ShowMessageBox();

    QPushButton* btn_Spin;
    QPushButton* btn_Launch;
    bool m_JustTriedDialog;

    bool m_Spinning;
    QTimer* m_UpdateTimer;
    QTime* m_ElapsedTimer;
    double m_Degrees;
    double m_TargetDegrees;
    double m_SpinSpeed;
    double m_SpinningStartTime;
    int m_SelectedCharacter;

    QSettings* m_Settings;

    bool m_ShouldShowMessageBox;
    bool m_ShouldPlayAnimation;

     QGraphicsOpacityEffect* m_OpacityEffect;
    
private slots:
    void BtnSpin_Clicked();
    void BtnLaunch_Clicked();
    void Update();

    void PositionCharacters(double theta);
    void Box_Toggled(bool checked);

    void Titlebar_File_DailySeed_triggered();
    void Titlebar_File_Quit_triggered();
    void Titlebar_Help_About_triggered();
    void Titlebar_Edit_ShowMessageBox_toggled(bool checked);
    void Titlebar_Edit_PlayAnimation_toggled(bool checked);
    void Titlebar_Edit_ShowLaunchButton_toggled(bool checked);
};

#endif // MAINWINDOW_H
