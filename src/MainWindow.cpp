#include "MainWindow.h"
#include "ui_MainWindow.h"

// Position of circle of characters
const int CHARACTERS_X = 240;
const int CHARACTERS_Y = 120;

#define M_PI    3.14159265358979323846

// Couple of functions to flip maps
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


    setFixedSize(480, 460);
    setWindowTitle("The Binding of Isaac Character Picker");
    m_Settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "bbb", "aaa");

    //////
    // Load settings
    /////
    if (!m_Settings->value("HasRunBefore").toBool())
    {
        m_Settings->setValue("Titlebar_ShouldPlayAnimation", true);
        m_Settings->setValue("Titlebar_ShouldShowMessageBox", true);
    }

    m_ShouldPlayAnimation = m_Settings->value("Titlebar_ShouldPlayAnimation").toBool();
    m_ShouldShowMessageBox = m_Settings->value("Titlebar_ShouldShowMessageBox").toBool();
    ui->actionPlay_animation->setChecked(m_ShouldPlayAnimation);
    ui->actionShow_message_box->setChecked(m_ShouldShowMessageBox);

    // Setup
    m_JustTriedDialog = false;
    m_Spinning = false;
    m_Degrees = 0;
    m_TargetDegrees = 0;
    m_UpdateTimer = new QTimer();
    m_ElapsedTimer = new QTime();

    srand(time(NULL));

    SetTitle();
    SetCharacters();
    SetCheckboxes();

    // Roll button setup
    btn_Spin = new QPushButton();
    btn_Spin->setParent(ui->centralWidget);
    btn_Spin->setText("Roll");
    btn_Spin->setGeometry(CHARACTERS_X-40, CHARACTERS_Y+140, 80, 30);
    connect(btn_Spin, SIGNAL(clicked(bool)), this, SLOT(BtnSpin_Clicked()));

    btn_Launch = new QPushButton();
    btn_Launch->setParent(ui->centralWidget);
    btn_Launch->setText("Launch Game");
    btn_Launch->setGeometry(CHARACTERS_X-50, box_IsSelectable[NUM_CHARACTERS-1]->pos().y()+40, 100, 30);
    connect(btn_Launch, SIGNAL(clicked(bool)), this, SLOT(BtnLaunch_Clicked()));

    if (m_Settings->value("LaunchFilePath").toString() != "" && m_Settings->value("ShouldShowLaunchButton").toBool() == true)
    {
        btn_Launch->show();
        setFixedSize(480, 500);
        ui->actionShow_launch_button->setChecked(true);
    }
    else
        btn_Launch->hide();

    // Attach functions to titlebar entries
    connect(ui->actionDaily_Seed, SIGNAL(triggered(bool)), this, SLOT(Titlebar_File_DailySeed_triggered()));
    connect(ui->actionQuit_Alt_F4, SIGNAL(triggered(bool)), this, SLOT(Titlebar_File_Quit_triggered()));
    connect(ui->actionShow_message_box, SIGNAL(toggled(bool)), this, SLOT(Titlebar_Edit_ShowMessageBox_toggled(bool)));
    connect(ui->actionPlay_animation, SIGNAL(toggled(bool)), this, SLOT(Titlebar_Edit_PlayAnimation_toggled(bool)));
    connect(ui->actionShow_launch_button, SIGNAL(toggled(bool)), this, SLOT(Titlebar_Edit_ShowLaunchButton_toggled(bool)));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(Titlebar_Help_About_triggered()));

    // Timer for update loop, activated when roll button is pressed
    m_UpdateTimer = new QTimer(this);
    connect(m_UpdateTimer, SIGNAL(timeout()), this, SLOT(Update()));

    PositionCharacters(0);
    m_Settings->setValue("HasRunBefore", true);
}

// Position the title text of the window
void MainWindow::SetTitle()
{
    QLabel* lblTitle = new QLabel();
    lblTitle->setParent(ui->centralWidget);

    int id = QFontDatabase::addApplicationFont(":/images/font_Upheavtt.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont font = lblTitle->font();
    font.setPixelSize(32);
    font.setFamily(family);
    lblTitle->setFont(font);

    lblTitle->setText("The Binding of Isaac\nCharacter Selector");

    lblTitle->adjustSize();
    lblTitle->setAlignment(Qt::AlignCenter);
   // lblTitle->setGeometry(QRect(this->geometry().width()/8, 20, 1, 1));
    lblTitle->move(this->geometry().center().x()/4, 20);

}

// Set up labels and load images
void MainWindow::SetCharacters()
{
    for (int i = 0; i < NUM_CHARACTERS; ++i)
    {
        //bool_IsSelectable[i] = true;

        img_Characters[i] = new QLabel();
        img_Characters[i]->setParent(ui->centralWidget);
        img_Characters[i]->setScaledContents(true);

        std::string path = ":/images/img_";
        std::string extension = ".png";


        std::stringstream image;
        image << path << characters[i];

        if (characters[i] == "Eden")
            image << std::rand() % NUM_EDEN_SPRITES + 1;

        image << extension;

        images[i] = QPixmap(image.str().c_str());
        img_Characters[i]->setPixmap(images[i]);
    }
}

// Place and label checkboxes
void MainWindow::SetCheckboxes()
{
    int startX = CHARACTERS_X+10;
    int startY = CHARACTERS_Y + 160;
    for (int i = 0; i < NUM_CHARACTERS; ++i)
    {
        box_IsSelectable[i] = new QCheckBox();
        box_IsSelectable[i]->setParent(ui->centralWidget);


        // Move every other one to the side
        int x = startX;
        if (i % 2 != 1)
        {
            x -= 80;
            startY += 20;
        }

        int y = startY;

        // If the final one is on its own, center it
        if (i == NUM_CHARACTERS-1 && i%2 == 0)
            x = startX -80 + 50;

        QFont font = box_IsSelectable[i]->font();
        font.setPixelSize(12);
        box_IsSelectable[i]->setFont(font);
        box_IsSelectable[i]->setGeometry(QRect(x, y, 1, 1));
        box_IsSelectable[i]->setText(characters[i].c_str());
        box_IsSelectable[i]->adjustSize();

        std::string characterString = "checkbox" + characters[i];
        if (!m_Settings->value("HasRunBefore").toBool())
        {
            m_Settings->setValue(characterString.c_str(), true);
        }

        box_IsSelectable[i]->setChecked(m_Settings->value(characterString.c_str()).toBool());

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

        if (m_ShouldShowMessageBox)
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
        float opacity = 0.35;

        if (!box_IsSelectable[i]->isChecked())
        {
            m_OpacityEffect = new QGraphicsOpacityEffect();
            m_OpacityEffect->setOpacity(opacity);
            img_Characters[i]->setGraphicsEffect(m_OpacityEffect);
        }
        else
        {
            img_Characters[i]->setGraphicsEffect(0);
        }

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
    float elapsed = m_ElapsedTimer->elapsed();

    float aThresh = 500;
    float end = 3000;

    float maxSpeed = 12;
    if (elapsed < aThresh)
        m_SpinSpeed = maxSpeed*(elapsed/aThresh);
    else if (elapsed <= end)
    {
       m_SpinSpeed = maxSpeed;
    }
    else if (elapsed > end)
    {
        if (m_SpinSpeed > 3.5)
            m_SpinSpeed -=0.1;
    }

    m_Degrees+=m_SpinSpeed;

    if (m_Degrees > 360)
        m_Degrees-=360;

    if (elapsed > end && m_SpinSpeed <= 3.5 && std::abs(m_Degrees - m_TargetDegrees) < 3)
        m_Spinning = false;
}

// Show the picked character
void MainWindow::ShowMessageBox()
{
    QMessageBox* mb = new QMessageBox();
    mb->setWindowTitle(" ");
    std::string message = "You're playing as " + characters[m_SelectedCharacter] + "!";
    mb->setText(message.c_str());

    mb->exec();
    int x = this->pos().x() + (this->width()/2) - (mb->width()/2);
    int y = this->pos().y() + (this->height()/2) - (mb->height()/2) + 162;

    mb->move(x,y);
}

// Button to spin
void MainWindow::BtnSpin_Clicked()
{
    // How many characters are selectable
    int charCount=0;
    for (int i = 0; i < NUM_CHARACTERS; ++i)
        if (box_IsSelectable[i]->isChecked())
            charCount++;

    // If no characters are selectable, we can't do anything
    if (charCount == 0)
    {
        QMessageBox* mb = new QMessageBox();
        mb->setWindowTitle(" ");
        mb->setText("You need to select some characters, yo!");
        mb->exec();
        return;
    }

    if (m_ShouldPlayAnimation)
    {
        btn_Spin->setEnabled(false);
        for (int i = 0; i < NUM_CHARACTERS; ++i)
            box_IsSelectable[i]->setEnabled(false);
        m_Spinning = true;

        m_UpdateTimer->stop();
        m_UpdateTimer->start(60);
        m_ElapsedTimer->restart();
    }

    // Roll
    int result = std::rand() % charCount;

    // Sort through deselected characters to find the actual picked character
    int counter = 0;
    for (int i = 0; i < NUM_CHARACTERS; ++i)
    {

        if (result == counter && box_IsSelectable[i]->isChecked())
        {
            m_SelectedCharacter = i;
            break;
        }
        else if (box_IsSelectable[i]->isChecked())
            counter++;
    }

    // Position of the character in the circle
    m_TargetDegrees = ((float)m_SelectedCharacter/NUM_CHARACTERS) * 360;
    m_SpinSpeed = 0;

    if (!m_ShouldPlayAnimation)
    {
        m_Degrees = m_TargetDegrees;
        PositionCharacters(m_Degrees);
        if (m_ShouldShowMessageBox)
            ShowMessageBox();
    }

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
            std::string characterString = "checkbox" + characters[i];
            m_Settings->setValue(characterString.c_str(), checked);
            PositionCharacters(m_Degrees);
            return;
        }
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::Titlebar_Help_About_triggered()
{
    QMessageBox* mb = new QMessageBox();

    std::string message =
        "A Binding of Isaac character picker.\n\n"
        "Code is under the GPL and available at\nhttps://github.com/jblakeLincoln/isaac-character-picker\n\n"
        "For any problems/suggestions, contact AntiIogical on Reddit.";
    mb->setText(message.c_str());
    mb->setFixedSize(700,100);
    mb->exec();

   // mb->adjustSize();

}

// If anybody has information on how seed generation works, please CONTACT ME
// It would be great to get a custom daily seed!
void MainWindow::Titlebar_File_DailySeed_triggered()
{
    QDateTime dt = QDateTime::currentDateTime();
    std::srand(dt.toString("yyyyMMdd").toInt());

    int character = std::rand() % NUM_CHARACTERS;

    int num;

    std::string output = "";

    for (int i = 0; i < 8; ++i)
    {
        num = (std::rand() % (90-56)) + (56) ;

        if (num < 65)
            num-=7;
        char c = (num);

        if (c == '5')
            c = 'S';
        else if (c == 'I')
            c = '1';
        else if (c == 'O')
            c = '0';
        else if (c == 'U')
            c = 'V';

        output += c;
        if (i == 3)
            output += "\n";

    }

    std::string date = dt.toString("dd").toStdString();
    date += " " + dt.date().shortMonthName(dt.date().month()).toStdString();
    date += " " + dt.toString("yyyy").toStdString();
    DailySeedDialog* mb = new DailySeedDialog(output.c_str(), date.c_str(), images[character], this);
    mb->setWindowTitle("Daily Seed");

    mb->adjustSize();

    int x = this->pos().x();
    int y = this->pos().y();
    x += this->width()/4;
    x += mb->width()/8;
    y += this->height()/4;
    y += mb->height()/8;
    mb->move(x,y);

    mb->show();

    std::srand(time(NULL));
}

void MainWindow::Titlebar_File_Quit_triggered()
{
    this->close();
}

void MainWindow::Titlebar_Edit_ShowMessageBox_toggled(bool checked)
{
    m_ShouldShowMessageBox = checked;
    m_Settings->setValue("Titlebar_ShouldShowMessageBox", checked);
}

void MainWindow::Titlebar_Edit_PlayAnimation_toggled(bool checked)
{
    m_ShouldPlayAnimation = checked;
    m_Settings->setValue("Titlebar_ShouldPlayAnimation", checked);
}

void MainWindow::Titlebar_Edit_ShowLaunchButton_toggled(bool checked)
{
    if (m_JustTriedDialog)
    {
        m_JustTriedDialog = false;
        return;
    }

    // Deselecting - hide everying
    if (!checked)
    {
        btn_Launch->hide();
        m_Settings->setValue("ShouldShowLaunchButton", false);
        setFixedSize(480, 460);
        return;
    }

    QFileDialog* d = new QFileDialog();
    d->setAcceptMode(QFileDialog::AcceptOpen);

    QString dir;
    if (m_Settings->value("LaunchFilePath").toString() == "")
    {

        QMessageBox* mb = new QMessageBox();
        mb->setText("Please navigate to your \'The Binding of Isaac: Rebirth\' directory,\n and select the executable file (\"isaac-ng.exe\") to create a launch shortcut");
        mb->exec();
        dir = "C:/Program Files (x86)/Steam/SteamApps/common/The Binding of Isaac Rebirth";
    }
    else
        dir = m_Settings->value("LaunchFilePath").toString();


    d->setDirectory(dir);

    d->exec();

    if (d->result() == QDialog::Rejected)
    {
        m_JustTriedDialog = true;
        ui->actionShow_launch_button->setChecked(false);
        return;
    }
    else
    {
        QString result =(d->selectedFiles().at(0));
        std::cout << m_Settings->value("LaunchFilePath").toString().toStdString() << std::endl;

        m_Settings->setValue("LaunchFilePath", result);
        m_Settings->setValue("ShouldShowLaunchButton", true);

        btn_Launch->show();
        setFixedSize(480, 500);
    }
}

void MainWindow::BtnLaunch_Clicked()
{
#ifdef _WIN32
    std::string command = "\"" + m_Settings->value("LaunchFilePath").toString().toStdString() + "\"";
    std::cout << command << std::endl;
    system(command.c_str());
#endif

#ifdef linux
    std::string path = m_Settings->value("LaunchFilePath").toString().toStdString();

    std::string dir = "cd \"" + path.substr(0, path.find_last_of('//')) + "\"";

    int i = path.find_last_of('/');
    std::string file = path.substr(i+1, path.length()-i-1);

    std::string command = dir + " && ./" + file;
    system(command.c_str());
#endif
}
