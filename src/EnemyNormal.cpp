#include "EnemyNormal.h"

EnemyNormal::EnemyNormal(int winWidth) : EnemyBase(winWidth)
{
    this->setWeight(7);
}
//贴图加载函数
void EnemyNormal::loadEnemyResource()
{
    
    this->loadImgFromFile("enemy_normal", this->m_imgWidth, this->m_imgHeight);
}