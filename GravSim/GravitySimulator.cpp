#include "GravitySimulator.h"
#include "ui_GravitySimulator.h"

GravitySimulator::GravitySimulator(QWidget *parent) :
    QMainWindow(parent), m_ui(new Ui::GravitySimulator),  m_clickModeActivated(false)
{
    m_ui->setupUi(this);

    setWindowTitle("Gravity Simulator");

    m_simulator = new Simulator(QPoint(0,0), QSize(200,200), this);
    m_ui->simulatorLayout->addWidget(m_simulator);

    /// Shortcuts
    m_ui->actionNew->setShortcut(QKeySequence::New);
    m_ui->actionOpen->setShortcut(QKeySequence::Open);
    m_ui->actionSave_as->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    m_ui->actionQuit->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    m_ui->actionStart_Pause->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    m_ui->actionReset->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
    m_ui->actionSet_Checkpoint->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));

    m_ui->settingsLayout->setSizeConstraint(QLayout::SetMaximumSize);

    /// Connections
    connect(m_ui->actionNew, SIGNAL(triggered()) , m_simulator , SLOT(clearAll()));
    connect(m_ui->actionOpen, SIGNAL(triggered()) , m_simulator , SLOT(loadExistingSaveFile()));
    connect(m_ui->actionReset, SIGNAL(triggered()) , m_simulator , SLOT(reset()));
    connect(m_ui->resetButton, SIGNAL(clicked()) , m_simulator , SLOT(reset()));
    connect(m_ui->actionSave_as, SIGNAL(triggered()) , m_simulator , SLOT(createSaveFile()));
    connect(m_ui->actionStart_Pause, SIGNAL(triggered()) , m_simulator , SLOT(startPause()));
    connect(m_ui->startPauseButton, SIGNAL(clicked()) , m_simulator , SLOT(startPause()));
    connect(m_ui->actionSet_Checkpoint, SIGNAL(triggered()) , m_simulator , SLOT(setCheckPoint()));
}

GravitySimulator::~GravitySimulator()
{
    delete m_ui;
}

void GravitySimulator::on_createParticuleButton_clicked()
{
    double mass = m_ui->massLineEdit->text().toDouble();
    sf::Vector2f position(m_ui->positionXLineEdit->text().toDouble() , m_ui->positionYLineEdit->text().toDouble());
    sf::Vector2f vitesse(m_ui->velocityXLineEdit->text().toDouble() , m_ui->velocityYLineEdit->text().toDouble());

    m_simulator->receiveParticuleData(mass, position, vitesse);
}

void GravitySimulator::on_clickModeButton_clicked()
{
    if(m_clickModeActivated)
    {
        m_clickModeActivated = false;
        m_ui->clickModeButton->setText("Click Mode Disabled");
    }
    else
    {
        m_clickModeActivated = true;
        m_ui->clickModeButton->setText("Click Mode Enabled");
    }

}

void GravitySimulator::mousePressEvent(QMouseEvent *event)
{
    if(m_clickModeActivated)
    {
        double x = event->x();
        double y = event->y() - menuBar()->height();

        if (QRect(m_simulator->pos(), m_simulator->size()).contains(QPoint(x,y)) )
        {
            x = (x - m_simulator->x())/ m_simulator->size().width() * m_simulator->GetView().GetRect().GetWidth() + m_simulator->GetView().GetRect().Left;
            y = (y - m_simulator->y())/m_simulator->size().height() * m_simulator->GetView().GetRect().GetHeight() + m_simulator->GetView().GetRect().Top;

            double mass = m_ui->massLineEdit->text().toDouble();
            if (mass <= 0)
            {
                mass = 1;
            }
            sf::Vector2f position(x,y);
            sf::Vector2f vitesse(m_ui->velocityXLineEdit->text().toDouble() , m_ui->velocityYLineEdit->text().toDouble());

            m_simulator->receiveParticuleData(mass, position, vitesse);
        }
    }
}

void GravitySimulator::on_FPSLineEdit_textEdited(const QString &text)
{
   double newFPS = text.toDouble();
        m_simulator->setFPS(newFPS);
}

void GravitySimulator::on_TimeFactorLineEdit_textEdited(const QString &text)
{
    double newTimeFactor = text.toDouble();
    if (newTimeFactor <= 0)
    {
        newTimeFactor = 1;
    }
    m_simulator->setTimeFactor(newTimeFactor);
}

void GravitySimulator::on_CalculPerFrameLineEdit_textEdited(const QString &text)
{
    double newCalculPerFrame = text.toUInt();
    if (newCalculPerFrame < 0)
    {
        newCalculPerFrame = 1;
    }
    m_simulator->setCalculPerFrame(newCalculPerFrame);
}






