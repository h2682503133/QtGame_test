#include "GameWidget.h"

#include <QRandomGenerator>
#include <QMessageBox>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , upPressed(false)
    , downPressed(false)
    , leftPressed(false)
    , rightPressed(false)
    , score(0)
    , gameOver(false)
{
    ui = new Ui::gamewidget();
    ui->setupUi(this);
    setWindowTitle("Qt飞机大战");

    //初始化计时器
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameWidget::gameUpdate);
    gameTimer->start(16);

    enemyTimer = new QTimer(this);
    connect(enemyTimer, &QTimer::timeout, this, &GameWidget::spawnEnemy);
    enemyTimer->start(1000);
    
    //初始化自机
    playerSpeed = 8;
    m_player = new Player(this->width()/2 - 20, this->height() - 50);
    // 加载本地自机图片 + 指定尺寸
    m_player->loadImgFromFile("./img/player.png", 50, 50);
}

GameWidget::~GameWidget()
{
    //释放指针
    delete m_player;
    delete gameTimer;
    delete enemyTimer;
    delete ui;
}

void GameWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (gameOver) {
        painter.setPen(Qt::red);
        painter.setFont(QFont("Arial", 30));
        painter.drawText(rect(), Qt::AlignCenter, QString("游戏结束\n得分：%1").arg(score));
        ui->label->hide(); //游戏结束隐藏得分标签
        return;
    }
    //绘制自机
    if(m_player != nullptr && m_player->isAlive())
    {
        painter.drawPixmap(m_player->getImgRect(), m_player->getPixmap());
    }
}

void GameWidget::keyPressEvent(QKeyEvent *event)
{
    if (gameOver) return;

    switch (event->key()) {
    case Qt::Key_Up:
        upPressed = true;
        break;
    case Qt::Key_Down:
        downPressed = true;
        break;
    case Qt::Key_Left:
        leftPressed = true;
        break;
    case Qt::Key_Right:
        rightPressed = true;
        break;
    case Qt::Key_Space:
        break;
    default:
        QWidget::keyPressEvent(event);
    }
}

void GameWidget::keyReleaseEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        upPressed = false;
        break;
    case Qt::Key_Down:
        downPressed = false;
        break;
    case Qt::Key_Left:
        leftPressed = false;
        break;
    case Qt::Key_Right:
        rightPressed = false;
        break;
    default:
         QWidget::keyReleaseEvent(event);
    }
}

void GameWidget::gameUpdate()
{
    if (gameOver || m_player == nullptr || !m_player->isAlive()) return;

    // 自机移动逻辑，调用Player类的移动函数
    if(upPressed)
        m_player->moveUp();
    if(downPressed)
        m_player->moveDown(this->height());
    if(leftPressed)
        m_player->moveLeft();
    if(rightPressed)
        m_player->moveRight(this->width());

    //更新UI里的得分标签
    ui->label->setText(QString("得分：%1").arg(score));

    update(); // 刷新画面，移动生效
}

void GameWidget::spawnEnemy()
{
    if (gameOver) return;
}