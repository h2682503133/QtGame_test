#ifndef ENEMYBASE_H
#define ENEMYBASE_H
#include "Player.h"
#include "GameObject.h"
#include <QString>
//敌机类型枚举 标记派生类型同时可用于随机池抽取
enum class EnemyType
{
    NormalEnemy,  //基础敌机
    StaticEnemy  //静止敌机
};
class EnemyBase;
typedef EnemyBase* (*EnemyCreatorFunc)(int winWidth);

// 敌机类型池的核心结构体：绑定敌机类型与其类型的创建函数
struct EnemyTypeItem
{
    EnemyType type;          // 敌机类型
    EnemyCreatorFunc creator;// 该类型的创建函数
};
// 所有敌机的公共父类
// 封装：所有敌机的通用数据、通用逻辑、统一接口、默认值设定
class EnemyBase : public GameObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(EnemyBase) // 禁止拷贝移动，防止内存错误
public:
    explicit EnemyBase(int winWidth, QObject *parent = nullptr);
    explicit EnemyBase(QObject *parent = nullptr);
    virtual ~EnemyBase() override = default;
    
    EnemyType getEnemyType()const;
    void move() override;
    
    virtual void loadEnemyResource()=0;  // 加载敌机贴图+碰撞体积(子类必须重写，无默认实现)
    virtual void onDead();        // 敌机死亡回调
    bool checkAllEnemyCollideWithPlayer(Player* player, bool& gameOver); //检测是否与玩家碰撞
    //出界相关
    void checkOutOfWindow(int winHeight); // 出界检测：飞出屏幕底部 → 标记死亡
    bool isEnemyOutOfWindow() const;       // 获取出界状态
    // 加分奖励
    int getScoreReward() const;
    void setScoreReward(int scoreReward);
    // 敌机基础移动速度
    void setEnemySpeed(int speed);
    int getEnemySpeed() const;
    // 敌机生成权重
    int getWeight() const;
    void setWeight(int weight);
protected:
    int         m_winWidth;        // 窗口宽度，用于随机生成X坐标、边界判定
    int         m_scoreReward=0;     // 击落敌机的加分奖励 有默认值
    int         m_enemySpeed;      // 敌机移动速度 有默认值
    bool        m_isOutOfWindow;   // 出界标记 默认值：false
    QString     m_imgFilePath;     // 敌机贴图路径 有默认空
    int         m_imgWidth;        // 敌机贴图宽度 有默认值
    int         m_imgHeight;       // 敌机贴图高度 有默认值
    int         m_collideRadius;   // 敌机碰撞半径 有默认值
    int         m_weight = 0;      // 敌机生成概率权重值 基础默认为0
    EnemyType   m_enemyType;
signals:  //敌机死亡信号，携带加分值
    void enemyDead(int scoreReward);
};


#endif // ENEMYBASE_H