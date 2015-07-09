#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMatrix4x4>

#include <iostream>

#include <map>

const int CHARACTERS_X = 240;
const int CHARACTERS_Y = 160;

#define M_PI    3.14159265358979323846

double degrees = 0;
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

    setFixedSize(480, 500);

    srand(time(NULL));

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
    SetCharacters();
    SetCheckboxes();
    

    QPushButton* btnRoll = new QPushButton();
    btnRoll->setParent(ui->centralWidget);
    btnRoll->setText("Roll");
    btnRoll->setGeometry(CHARACTERS_X-40, CHARACTERS_Y+140, 80, 30);
    connect(btnRoll, SIGNAL(clicked(bool)), this, SLOT(BtnSpin_Clicked()));

    QTimer* timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(Update()));
    timer->start(60);
}

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

void MainWindow::SetCheckboxes()
{
    int startX = 180;
    int startY = 340;
    for (int i = 0; i < NUM_CHARACTERS; ++i)
    {
        box_IsSelectable[i] = new QCheckBox();
        box_IsSelectable[i]->setParent(ui->centralWidget);
        box_IsSelectable[i]->setChecked(true);

        int x = startX;

        if (i % 2 != 1)
        {
            x += 80;
            startY += 20;
        }

        int y = startY;

        if (i == NUM_CHARACTERS-1 && i%2 == 0)
            x = startX + 40;

        box_IsSelectable[i]->setGeometry(QRect(x, y, 65, 15));
        box_IsSelectable[i]->setText(characters[i].c_str());

        connect(box_IsSelectable[i], SIGNAL(toggled(bool)), this, SLOT(Box_Toggled(bool)));
    }
}

void MainWindow::Update()
{
    PositionCharacters(0);
}

void MainWindow::PositionCharacters(double theta)
{
    std::map<QLabel*, double> os;

    double radius = 128;
    theta = theta*(M_PI/180);
    for (int i = 0; i < NUM_CHARACTERS; ++i)
    {
        int size = 128;

        double y = std::cos(i*(M_PI*2)/NUM_CHARACTERS);
        double x = -std::sin(i*(M_PI*2)/NUM_CHARACTERS);


        double cs = std::cos(theta);
        double sn = std::sin(theta);

        double px = x * cs + y * sn;
        double py = x * sn - y * cs;

        os[img_Characters[i]]= py;

        px *= 1.3;
        py *= 0.2;
        double sx = px;
        double sy = py;
        px*=radius;
        py*=radius;


        sy = -sy;
        double norm = (1.8-0.2)/(1- -1)*(sy-1.0)+1.5;

        img_Characters[i]->setGeometry(QRect(0, 0, 128*norm, 128*norm));

        QImage image(images[i].size(), QImage::Format_ARGB32_Premultiplied);

        QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect();
        QGraphicsOpacityEffect* oEffect = new QGraphicsOpacityEffect();

        oEffect->setOpacity(0.3);
        effect->setColor(QColor(0, 0, 0, 0));

        if (!bool_IsSelectable[i])
            img_Characters[i]->setGraphicsEffect(oEffect);
        else
            img_Characters[i]->setGraphicsEffect(0);

        img_Characters[i]->setPixmap(images[i].scaled(size*norm, size*norm, Qt::KeepAspectRatio));
        img_Characters[i]->move(QPoint(CHARACTERS_X-px-(128*norm/2), CHARACTERS_Y-(py)   ));
    }

    std::multimap<double, QLabel*> dst = flip_map(os);
    std::multimap<double, QLabel*>::iterator it;

    for (it = dst.begin(); it != dst.end(); ++it)
    {
        (*it).second->lower();
    }

}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::BtnSpin_Clicked()
{ //connect(button, SIGNAL(clicked()), this, SLOT(Btn_Spin_Clicked()));

    int charCount=0;

    for (int i = 0; i < NUM_CHARACTERS; ++i)
        if (bool_IsSelectable[i])
            charCount++;

    QMessageBox* mb = new QMessageBox();
    mb->setWindowTitle(" ");

    if (charCount == 0)
    {
        mb->setText("You need to select some characters, yo!");
        mb->show();
        return;
    }
    int result = std::rand() % charCount;

    std::string characterName;
    int counter = 0;
    for (int i = 0; i < NUM_CHARACTERS; ++i)
    {

        if (result == counter && bool_IsSelectable[i])
        {
            characterName = characters[i];
            break;
        }
        else if (bool_IsSelectable[i])
            counter++;

    }


    std::string message = "You're playing as " + characterName + "!";
    mb->setText(message.c_str());
    mb->show();
}

void MainWindow::Box_Toggled(bool checked)
{
    QString text = qobject_cast<QCheckBox*>(sender())->text();

    for (int i = 0; i < NUM_CHARACTERS; ++i)
    {
        if (text == QString(characters[i].c_str()))
        {
            bool_IsSelectable[i] = !bool_IsSelectable[i];
            return;
        }
    }
}

