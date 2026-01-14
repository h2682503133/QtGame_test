#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QRect>
#include <QPoint>
#include <QRect>
#include <QPoint>
#include <QPixmap>
#include <QtGui>
#include <cmath> 

// 游戏所有实体的通用基类 - 所有自机/子弹/敌机都继承该类
// 完全满足需求：圆形碰撞+独立碰撞盒与贴图+血量/伤害/防御/阵营+移动+存活+尺寸+坐标 全封装
class GameObject
{
public:

    // 无参构造+有参全构造
    GameObject();
    GameObject(int x, int y, int imgWidth, int imgHeight, int collideRadius, int speed = 0);
    virtual ~GameObject() = default; // 虚析构

    //圆形碰撞体积
    QPoint getCollideCenter() const;  // 获取圆形碰撞盒圆心
    int getCollideRadius() const;     // 获取圆形碰撞盒半径
    void setCollideCenter(int x, int y); // 设置圆形碰撞盒圆心
    void setCollideRadius(int radius);  // 设置圆形碰撞盒半径
    bool isCircleCollide(const GameObject& other) const; // 圆形与圆形碰撞检测核心函数

    // 本地图片文件加载接口
    void loadImgFromFile(const QString&  imgName);
    void loadImgFromFile(const QString&  imgName, int showWidth, int showHeight);

    // 贴图属性(显示用) - 尺寸/坐标 独立于碰撞盒，但是联动移动
    void setPixmap(const QPixmap& pix);   // 设置绑定的图像
    QPixmap getPixmap() const;            // 获取绑定的图像
    QRect getImgRect() const;             // 获取图像的显示矩形(贴图大小/位置)
    void setImgSize(int w, int h);        // 设置图像尺寸(贴图大小)
    void setImgPos(int x, int y);         // 设置图像坐标(贴图位置)

    //血量+伤害+防御(乘算无敌)
    int getHp() const;        // 获取当前血量
    int getMaxHp() const;     // 获取最大血量
    int getDamage() const;    // 获取攻击伤害
    float getDefense() const; // 获取防御系数(乘算，核心：0=无防御，1=无敌，0.5=减半伤害)
    void setHp(int hp);       // 设置当前血量
    void setMaxHp(int maxHp); // 设置最大血量
    void setDamage(int damage); // 设置攻击伤害
    void setDefense(float defense); // 设置防御系数(乘算)
    void takeDamage(int damage);    // 受击扣血核心函数(自动计算防御减伤，无敌判定)

    //阵营旗帜 全套接口
    int getCamp() const;     // 获取阵营标记
    void setCamp(int camp);  // 设置阵营标记
    bool isSameCamp(GameObject *other); // 判断是否同阵营

    // 移动速度、存活状态、坐标基础操作
    int getSpeed() const;
    bool isAlive() const;
    void setSpeed(int speed);
    void setAlive(bool alive);
    int getX() const;
    int getY() const;
    void setX(int x);
    void setY(int y);

    //移动行为
    virtual void move() = 0;

protected:
    int m_speed;          // 移动速度
    bool m_isAlive;       // 存活状态 true=存活 false=死亡(待清理)

    //圆形碰撞体积
    QPoint m_collideCenter; // 圆形碰撞盒 圆心坐标
    int m_collideRadius;    // 圆形碰撞盒 半径

    // --- 核心要求2：独立贴图+独立碰撞盒 (双独立属性，联动移动核心) ---
    QPixmap m_img;          // 绑定的显示图像
    QRect m_imgRect;        // 图像显示矩形(贴图坐标+尺寸)

    //战斗核心属性 (血量/伤害/防御乘算/无敌) ---
    int m_hp;               // 当前血量
    int m_maxHp;            // 最大血量
    int m_damage;           // 攻击伤害
    float m_defense;        // 防御系数【乘算】：最终伤害 = 原伤害 * (1 - m_defense)
                            // 无敌物体设置 m_defense = 1.0 即可，伤害*0 → 无伤
                            // 无防御设置 m_defense = 0.0，伤害全额生效

    //阵营旗帜---
    int m_camp;            // 阵营标记，区分敌我 0不与任何单位为敌，1为其他所有单位为敌，其他相同则为友
    
    //贴图与碰撞盒 共同移动
    // 统一移动偏移：调用该函数，图像和碰撞盒会同步偏移，保证永远一起移动，无需手动同步
    void moveOffset(int dx, int dy);
};

#endif // GAMEOBJECT_H