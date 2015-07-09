#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMatrix4x4>

#include <iostream>

#include <map>

const int CHARACTERS_X = 240;
const int CHARACTERS_Y = 120;

#define M_PI    3.14159265358979323846


template<typename A, typename B>
std::pair<B,A> flip_pair(const std::pair<A,B> &p)
{
    return std::pair<B,A>(p.second, p.first);
}

template<typename A, typename B>
std::multimap<B,A> flip_map(const std::map<A,B> &src)
{
    std::multimap<B,A> dst;
    std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
                   flip_pair<A,B>);
    return dst;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->menuBar->setVisible(false);

    // Setup
    m_Spinning = false;
    m_Degrees = 0;
    m_TargetDegrees = 180;
    m_UpdateTimer = new QTimer();
    m_ElapsedTimer = new QTime();

    setFixedSize(480, 460);
    srand(time(NULL));

    SetTitle();
    SetCharacters();
    SetCheckboxes();

    btn_Spin = new QPushButton();
    btn_Spin->setParent(ui->centralWidget);
    btn_Spin->setText("Roll");
    btn_Spin->setGeometry(CHARACTERS_X-40, CHARACTERS_Y+140, 80, 30);
    connect(btn_Spin, SIGNAL(clicked(bool)), this, SLOT(BtnSpin_Clicked()));

    m_UpdateTimer = new QTimer(this);
    connect(m_UpdateTimer, SIGNAL(timeout()), this, SLOT(Update()));

    PositionCharacters(0);
}

// Position the title text of the window
void MainWindow::SetTitle()
{
    QLabel* lblTitle = new QLabel();
    lblTitle->setParent(ui->centralWidget);
    QFont font = lblTitle->font();
    font.setPointSize(24);
    font.setBold(true);
    lblTitle->setFont(font);
    lblTitle->setAlignment(Qt::AlignCenter);
    lblTitle->setText("The Binding of Isaac\nCharacter Selector");
    lblTitle->setGeometry(QRect(CHARACTERS_X/3, 20, 1, 1));
    lblTitle->adjustSize();
}

// Set up labels and load images
void MainWindow::SetCharacters()
{
    for (int i = 0; i < NUM_CHARACTERS; ++i)
    {
        bool_IsSelectable[i] = true;

        img_Characters[i] = new QLabel();
        img_Characters[i]->setParent(ui->centralWidget);
        img_Characters[i]->setScaledContents(true);

        std::string path = ":/images/img_";
        std::string extension = ".png";
        std::string image = path + characters[i] + extension;

        images[i] = QPixmap(image.c_str());
        img_Characters[i]->setPixmap(images[i]);
    }
}

// Place and label checkboxes
void MainWindow::SetCheckboxes()
{
    int startX = CHARACTERS_X+20;
    int startY = CHARACTERS_Y + 160;
    for (int i = 0; i < NUM_CHARACTERS; ++i)
    {
        box_IsSelectable[i] = new QCheckBox();
        box_IsSelectable[i]->setParent(ui->centralWidget);
        box_IsSelectable[i]->setChecked(true);

        int x = startX;

        if (i % 2 != 1)
        {
            x -= 80;
            startY += 20;
        }

        int y = startY;

        if (i == NUM_CHARACTERS-1 && i%2 == 0)
            x = startX -80+ 40;

        box_IsSelectable[i]->setGeometry(QRect(x, y, 65, 15));
        box_IsSelectable[i]->setText(characters[i].c_str());

        connect(box_IsSelectable[i], SIGNAL(toggled(bool)), this, SLOT(Box_Toggled(bool)));
    }
}


void MainWindow::Update()
{
    // If the spin has just ended
    if (!m_Spinning)
    {
        btn_Spin->setEnabled(true);
        for (int i = 0; i < NUM_CHARACTERS; ++i)
            box_IsSelectable[i]->setEnabled(true);

        ShowMessageBox();
        m_UpdateTimer->stop();
        return;
    }

    Animate();
    PositionCharacters(m_Degrees);
}

// Place characters in an oval layout
void MainWindow::PositionCharacters(double theta)
{
    std::map<QLabel*, double> os;

    double radius = 128;
    theta = theta*(M_PI/180);
    for (int i = 0; i < NUM_CHARACTERS; ++i)
    {
        int size = 128;

        // Evenly space characters in a circle
        double y = std::cos(i*(M_PI*2)/NUM_CHARACTERS);
        double x = -std::sin(i*(M_PI*2)/NUM_CHARACTERS);

        // Rotate the circle based on input
        double cs = std::cos(theta);
        double sn = std::sin(theta);
        double px = x * cs + y * sn;
        double py = x * sn - y * cs;

        os[img_Characters[i]]= py;

        // Scale the circle to the oval shape we want
        px *= 1.3;
        py *= 0.2;
        double sy = py;
        px*=radius;
        py*=radius;

        // Resize characters based on position
        sy = -sy;
        double norm = (1.8-0.2)/(1- -1)*(sy-1.0)+1.5;
        img_Characters[i]->setGeometry(QRect(0, 0, 128*norm, 128*norm));

        // If a character is deselected, make them slightly transparent
        m_OpacityEffect = new QGraphicsOpacityEffect();
        m_OpacityEffect->setOpacity(0.3);
        if (!bool_IsSelectable[i])
            img_Characters[i]->setGraphicsEffect(m_OpacityEffect);
        else
            img_Characters[i]->setGraphicsEffect(0);

        // Finally place them
        img_Characters[i]->setPixmap(images[i].scaled(size*norm, size*norm, Qt::KeepAspectRatio));
        img_Characters[i]->move(QPoint(CHARACTERS_X-px-(128*norm/2), CHARACTERS_Y-(py)   ));
    }

    // Sort by Y value - we've added them to an ordered list.
    // Now we iterate through it and lower elements furthest from the front
    std::multimap<double, QLabel*> dst = flip_map(os);
    std::multimap<double, QLabel*>::iterator it;

    for (it = dst.begin(); it != dst.end(); ++it)
        (*it).second->lower();
}

// Spin the circle until we hit the desired character
void MainWindow::Animate()
{
    float speed = 0;
    float elapsed = m_ElapsedTimer->elapsed();

    float aThresh = 2000;
    float end = 5000;

    if (elapsed > end)
    {
        float t = 360-m_Degrees - m_TargetDegrees;
        if (std::abs(t) < 240)
            end += elapsed;
    }
    else
    {
        float maxSpeed = 10;
        if (elapsed < aThresh)
            speed = elapsed/100;
        else if (elapsed < end-1000)
            speed = 10;
        else if (elapsed < end)
        {
            speed = (end-elapsed)/60;
        }

        if (speed > maxSpeed)
            speed = maxSpeed;
    }

    if (speed < 3 && (m_Degrees < m_TargetDegrees - 2 || m_Degrees > m_TargetDegrees + 2))
        speed = 3;

    m_Degrees+=speed;

    if (m_Degrees > 360)
        m_Degrees-=360;

    if (speed == 0)
        m_Spinning = false;
}

// Show the picked character
void MainWindow::ShowMessageBox()
{
    QMessageBox* mb = new QMessageBox();
    mb->setWindowTitle(" ");
    std::string message = "You're playing as " + characters[m_SelectedCharacter] + "!";
    mb->setText(message.c_str());

    mb->show();
    int x = mb->pos().x();
    int y = mb->pos().y() + 160;

    mb->move(x,y);
}

// Button to spin
void MainWindow::BtnSpin_Clicked()
{

    btn_Spin->setEnabled(false);
    for (int i = 0; i < NUM_CHARACTERS; ++i)
        box_IsSelectable[i]->setEnabled(false);
    m_Spinning = true;

    m_UpdateTimer->stop();
    m_UpdateTimer->start(60);
    m_ElapsedTimer->restart();

    // How many characters are selectable
    int charCount=0;
    for (int i = 0; i < NUM_CHARACTERS; ++i)
        if (bool_IsSelectable[i])
            charCount++;


    // If no characters are selectable, we can't do anything
    if (charCount == 0)
    {
        QMessageBox* mb = new QMessageBox();
        mb->setWindowTitle(" ");
        mb->setText("You need to select some characters, yo!");
        mb->show();
        return;
    }


    // Roll
    int result = std::rand() % charCount;

    // Sort through deselected characters to find the actual picked character
    int counter = 0;
    for (int i = 0; i < NUM_CHARACTERS; ++i)
    {

        if (result == counter && bool_IsSelectable[i])
        {
            m_SelectedCharacter = i;
            break;
        }
        else if (bool_IsSelectable[i])
            counter++;
    }

    // Position of the character in the circle
    m_TargetDegrees = ((float)m_SelectedCharacter/NUM_CHARACTERS) * 360;
}

// Toggle character checkbox
// Find the relevant character based on the label's text
void MainWindow::Box_Toggled(bool checked)
{
    QString text = qobject_cast<QCheckBox*>(sender())->text();

    for (int i = 0; i < NUM_CHARACTERS; ++i)
    {
        if (text == QString(characters[i].c_str()))
        {
            bool_IsSelectable[i] = checked;
            PositionCharacters(m_Degrees);
            return;
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
