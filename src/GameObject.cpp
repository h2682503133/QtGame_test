#include "GameObject.h"
#include <cmath>

// 无参构造
GameObject::GameObject()
    : m_speed(0)
    , m_isAlive(true)
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
GameObject::GameObject(int x, int y, int imgWidth, int imgHeight, int collideRadius, int speed)
    : m_speed(speed)
    , m_isAlive(true)
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
    int dx = m_collideCenter.x() - other.m_collideCenter.x();
    int dy = m_collideCenter.y() - other.m_collideCenter.y();
    int distanceSqrt = dx*dx + dy*dy; // 平方和，避免开方，提升性能
    int radiusSum = m_collideRadius + other.m_collideRadius;
    return distanceSqrt <= radiusSum * radiusSum;
}

// ===================== 核心要求2 独立贴图+独立碰撞盒 共同移动 全部实现 =====================
void GameObject::setPixmap(const QPixmap &pix)
{
    m_img = pix;
}

QPixmap GameObject::getPixmap() const
{
    return m_img;
}

QRect GameObject::getImgRect() const
{
    return m_imgRect;
}
void GameObject::loadImgFromFile(const QString&  imgName)
{
    m_img.load( "./img/"+imgName+".png");
}

void GameObject::loadImgFromFile(const QString&  imgName, int showWidth, int showHeight)
{
    m_img = QPixmap("./img/"+imgName+".png").scaled(showWidth, showHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_imgRect.setSize(QSize(showWidth, showHeight));
}
void GameObject::setImgSize(int w, int h)
{
    m_imgRect.setSize(QSize(w, h));
    // 图像尺寸改变，碰撞圆心默认居中图像，可选联动，也可手动修改
    m_collideCenter = QPoint(m_imgRect.x() + w/2, m_imgRect.y() + h/2);
}

void GameObject::setImgPos(int x, int y)
{
    m_imgRect.moveTo(x, y);
    // 图像位置改变，碰撞圆心默认居中图像，可选联动，也可手动修改
    m_collideCenter = QPoint(x + m_imgRect.width()/2, y + m_imgRect.height()/2);
}

// ===================== 核心要求3 血量/伤害/防御(乘算无敌) 全部实现 =====================
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

// 受击扣血核心：自动计算防御减伤，乘算逻辑，无敌判定，原代码优先无多余逻辑
void GameObject::takeDamage(int damage)
{
    if (m_defense >= 1.0f || !m_isAlive) // 无敌状态/已死亡 直接免伤
        return;

    int realDamage = static_cast<int>(damage * (1 - m_defense)); // 乘算减伤
    realDamage = qMax(1, realDamage); // 保底1点伤害，避免0伤害
    setHp(m_hp - realDamage);

    if (m_hp <= 0) // 血量为0，标记死亡
    {
        m_isAlive = false;
    }
}

int GameObject::GameObject::getCamp() const
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

bool GameObject::isAlive() const
{
    return m_isAlive;
}

void GameObject::setSpeed(int speed)
{
    m_speed = speed;
}

void GameObject::setAlive(bool alive)
{
    m_isAlive = alive;
}

int GameObject::getX() const
{
    return m_imgRect.x(); // 对外X坐标默认取图像坐标，也可自定义为碰撞圆心X
}

int GameObject::getY() const
{
    return m_imgRect.y(); // 对外Y坐标默认取图像坐标，也可自定义为碰撞圆心Y
}

void GameObject::setX(int x)
{
    setImgPos(x, m_imgRect.y());
}

void GameObject::setY(int y)
{
    setImgPos(m_imgRect.x(), y);
}


void GameObject::moveOffset(int dx, int dy)
{
    // 图像偏移dx dy
    m_imgRect.translate(dx, dy);
    // 碰撞盒圆心同步偏移dx dy → 保证永远一起移动
    m_collideCenter += QPoint(dx, dy);
}