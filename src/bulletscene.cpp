#include "bulletscene.h"

#include "debug.h"

BulletScene::BulletScene()
    :Entity()
    ,sceneCenterX(320.0)
    ,sceneCenterY(240.0)
    ,sceneHeight(480.0)
    ,sceneWidth(640.0)
{
    useDefaultTicking=false;
}

BulletScene::~BulletScene()
{
    bulletList.clearAll();
}

Bullet* BulletScene::newBullet(double x, double y, double speed, double angle, int type)
{
    return new Bullet(x, y, speed, angle, type);
}

Bullet* BulletScene::pushBullet(Bullet *bullet)
{
    bulletList.pushEntity(bullet);
    return bullet;
}

bool BulletScene::checkSceneBorder(Entity *entity, double offsetLength)
{
    double px=entity->getX(), py=entity->getY();
    double sx=entity->getSpeedX(), sy=entity->getSpeedY();
    return (sx*(px-sceneCenterX)*2>abs(sx)*(sceneWidth+offsetLength)||
            sy*(py-sceneCenterY)*2>abs(sy)*(sceneHeight+offsetLength));
}

int BulletScene::getBulletSize() const
{
    return bulletList.size();
}

void BulletScene::onUpdateMotion(double deltaSec, double)
{
    //double d=deltaSec*1.1+(sin(timeSec+deltaSec)-sin(timeSec));


    bulletList.updateAll(deltaSec);

}

void BulletScene::onRender()
{
    bulletList.renderAll();
}
