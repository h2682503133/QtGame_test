#include "EnemyNormal.h"
QPixmap EnemyNormal::s_img;
EnemyNormal::EnemyNormal(int winWidth) : EnemyBase(winWidth)
{
    this->setWeight(7);
    this->setReady(true);
}
//贴图加载函数
void EnemyNormal::loadEnemyResource()
{
    s_img=this->loadImgFromFile("enemy_normal", this->m_imgWidth, this->m_imgHeight);
    this->m_img=&s_img;
}