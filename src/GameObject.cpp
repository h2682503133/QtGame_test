#include "GameObject.h"
#include <QMessageBox>
#include <cmath>

// 无参构造
GameObject::GameObject(): QObject(nullptr)
    , m_speed(0)
    , m_collideCenter(0, 0)
    , m_collideRadius(0)
    , m_imgRect(0, 0, 0, 0)
    , m_hp(1)
    , m_maxHp(1)
    , m_damage(1)
    , m_defense(0.0f)
    , m_camp(0)
{
}

// 有参全构造 - 初始化坐标/贴图尺寸/碰撞半径/速度
GameObject::GameObject(int x, int y, int imgWidth, int imgHeight, int collideRadius, int speed): QObject(nullptr)
    , m_speed(speed)
    , m_collideCenter(x + imgWidth/2, y + imgHeight/2) // 碰撞圆心默认居中图像
    , m_collideRadius(collideRadius)
    , m_imgRect(x, y, imgWidth, imgHeight)
    , m_hp(1)
    , m_maxHp(1)
    , m_damage(1)
    , m_defense(0.0f)
    , m_camp(0)
{
}

bool GameObject::isReady() const
{
    return m_isReady;
}

void GameObject::setReady(bool ready)
{
    m_isReady = ready;
}
bool GameObject::isAlive() const
{
    return m_isAlive;
}
void GameObject::setAlive(bool alive)
{
    m_isAlive = alive;
}
QPoint GameObject::getCollideCenter() const
{
    return m_collideCenter;
}

int GameObject::getCollideRadius() const
{
    return m_collideRadius;
}

void GameObject::setCollideCenter(int x, int y)
{
    m_collideCenter = QPoint(x, y);
}

void GameObject::setCollideRadius(int radius)
{
    m_collideRadius = radius;
}

// 圆形碰撞检测
bool GameObject::isCircleCollide(const GameObject &other) const
{
    if (m_collideRadius <= 0 || other.m_collideRadius <= 0) return false;
    const int dx = m_collideCenter.x() - other.m_collideCenter.x();
    const int dy = m_collideCenter.y() - other.m_collideCenter.y();
    const int distanceSqrt = dx*dx + dy*dy;
    return distanceSqrt <= (m_collideRadius + other.m_collideRadius) * (m_collideRadius + other.m_collideRadius);
}
//贴图相关
void GameObject::setPixmap(QPixmap &pix)
{
    m_img = &pix;
}

QPixmap GameObject::getPixmap() const
{
    return *m_img;
}

QRect GameObject::getImgRect() const
{
    return m_imgRect;
}
QPixmap GameObject::loadImgFromFile(const QString&  imgName)
{
    QPixmap temp = QPixmap();
    m_imgRect.setSize(QSize(0, 0));
    QString imgFullPath = "./img/" + imgName + ".png";

    try {
        // 加载图片
        temp = QPixmap(imgFullPath);
        //判断图片是否加载成功
        if(temp.isNull())
        {
            QMessageBox::critical(nullptr,  // 父窗口，nullptr=置顶全局弹窗
                                  "图片加载致命错误",  // 弹窗标题（醒目红色错误标题）
                                  QString("找不到图片文件！\n路径：%1\n请检查图片路径和文件是否存在！").arg(imgFullPath),
                                  QMessageBox::Ok);
            return temp;
        }
        return temp;
    } catch (...) {
        //捕获所有未知异常
        temp = QPixmap();
        m_imgRect.setSize(QSize(0, 0));
        QMessageBox::critical(nullptr,  // 父窗口，nullptr=置顶全局弹窗
                                  "图片加载致命错误",  // 弹窗标题（醒目红色错误标题）
                                  QString("找不到图片文件！\n路径：%1\n请检查图片路径和文件是否存在！").arg(imgFullPath),
                                  QMessageBox::Ok);
        }
        return QPixmap();
}

QPixmap GameObject::loadImgFromFile(const QString&  imgName, int showWidth, int showHeight)
{
    QPixmap temp;
    loadImgFromFile(imgName);
    temp.scaled(showWidth, showHeight,Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_imgRect.setSize(QSize(showWidth, showHeight));
    return temp;
}
void GameObject::setImgSize(int w, int h)
{
    m_imgRect.setSize(QSize(w, h));
}

void GameObject::setImgPos(int x, int y)
{
    m_imgRect.moveTo(x, y);
    // 图像位置改变，碰撞圆心默认居中图像，可选联动，也可手动修改
    m_collideCenter = QPoint(x + m_imgRect.width()/2, y + m_imgRect.height()/2);
}

//血量/伤害/防御(乘算无敌)
int GameObject::getHp() const
{
    return m_hp;
}

int GameObject::getMaxHp() const
{
    return m_maxHp;
}

int GameObject::getDamage() const
{
    return m_damage;
}

float GameObject::getDefense() const
{
    return m_defense;
}

void GameObject::setHp(int hp)
{
    m_hp = qBound(0, hp, m_maxHp); // 血量限制在0~maxHp之间，不会溢出/负数
}

void GameObject::setMaxHp(int maxHp)
{
    m_maxHp = maxHp;
    m_hp = qBound(0, m_hp, m_maxHp);
}

void GameObject::setDamage(int damage)
{
    m_damage = damage;
}

void GameObject::setDefense(float defense)
{
    m_defense = qBound(0.0f, defense, 1.0f); // 防御系数限制0~1，0无防御，1无敌
}

// 受击扣血核心：自动计算防御减伤，乘算逻辑，无敌判定
void GameObject::takeDamage(int damage)
{
    if (m_defense >= 1.0f || !m_isAlive) return;
    int realDamage = static_cast<int>(damage * (1 - m_defense));
    realDamage = qMax(1, realDamage);
    setHp(m_hp - realDamage);
    if (m_hp <= 0)
    {
        setAlive(false);
        onDead();
    }
}
void GameObject::onDead()
{
    setAlive(false);//二次修改使其兼容非受伤死亡方式
    this->deleteLater();
}
int GameObject::getCamp() const
{
    return m_camp;
}

void GameObject::setCamp(int camp)
{
    m_camp = camp;
}

bool GameObject::isSameCamp(GameObject *other)
{
    if(other->getCamp()==0||m_camp==0)return true;
    else if(other->getCamp()==1||m_camp==1)return false;
    else return m_camp == other->getCamp();
}

int GameObject::getSpeed() const
{
    return m_speed;
}

void GameObject::moveOffset(int dx, int dy)
{
    // 图像偏移dx dy
    m_imgRect.translate(dx, dy);
    // 碰撞盒圆心同步偏移dx dy → 保证永远一起移动
    m_collideCenter += QPoint(dx, dy);
}