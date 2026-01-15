#include "GameWidget.h"

#include <QRandomGenerator>
#include <QMessageBox>

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , upPressed(false)
    , downPressed(false)
    , leftPressed(false)
    , rightPressed(false)
    , zPressed(false)
    , score(0)
    , gameOver(false)
{
    ui = new Ui::Form();
    ui->setupUi(this);
    setWindowTitle("Qt飞机大战");

    m_enemyTypePool.clear();
    m_weightList.clear();
    m_totalWeight = 0;
    //初始化敌人生成池
    initEnemyTypePool();
    //初始化自机
    playerSpeed = 8;
    m_player = new Player(this->width()/2 - 20, this->height() - 50);
    m_player->setParent(this);
    QTimer::singleShot(0, this, &GameWidget::initGame);
    
}
void GameWidget::initGame()
{ 
    QEventLoop initLoop; // 创建局部事件循环，作用域仅限本函数
    QTimer::singleShot(0, &initLoop, &QEventLoop::quit); // 零延迟退出循环
    initLoop.exec();

    // 加载本地自机图片 + 指定尺寸
    m_player->loadImgFromFile("player", 50, 50);

    gameOver = false;
    //初始化计时器
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, this, &GameWidget::gameUpdate);
    gameTimer->start(1000/60);//刷新帧率
    
    enemyTimer = new QTimer(this);
    connect(enemyTimer, &QTimer::timeout, this, &GameWidget::spawnEnemy);
    QTimer::singleShot(100, this, [=](){ enemyTimer->start(1000); });      
}
void GameWidget::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event); // 执行Qt原生的显示逻辑
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
    //绘制敌人
    drawAllEnemies(painter);
    //绘制子弹
    drawAllBullets(painter); 
}

void GameWidget::gameUpdate()
{
    if (gameOver || m_player == nullptr || !m_player->isAlive()) return;
    // 自机控制逻辑
    // 自机移动
    if(upPressed)
        m_player->moveUp();
    if(downPressed)
        m_player->moveDown(this->height());
    if(leftPressed)
        m_player->moveLeft();
    if(rightPressed)
        m_player->moveRight(this->width());
    // 自机发射子弹
    if(zPressed)
        m_player->shootBullet();
    // 敌人相关
    updateAllEnemies();
    checkPlayerEnemyCollision();

    // 子弹相关
    updateAllBullets();
    checkBulletCollisions();

    // 更新UI里的得分标签
    ui->label->setText(QString("得分：%1").arg(score));

    
    update(); // 刷新画面，移动生效
}
