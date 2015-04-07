#include "bulletscene.h"

#include "commondef.h"

#include "debug.h"

BulletScene::BulletScene()
    :Entity()
    ,sceneCenterX(WIDTH/2)
    ,sceneCenterY(HEIGHT/2)
    ,sceneWidth(WIDTH)
    ,sceneHeight(HEIGHT)
{
    useDefaultTicking=false;
}

BulletScene::~BulletScene()
{
    bulletList.clearAll();
}

int BulletScene::pushBullet(const Bullet& bullet)
{
    return bulletList.pushEntity(bullet);
}

int BulletScene::pushBullet(double x, double y, double speed, double angle, int type)
{
    return bulletList.newEntity(x, y, speed, angle, type, this);
}

bool BulletScene::checkSceneBorder(Entity *entity, double offsetLength)
{
    double px=entity->getX(), py=entity->getY();
    double sx=entity->getSpeedX(), sy=entity->getSpeedY();
    return (sx*px*2>abs(sx)*(sceneWidth+offsetLength)||
            sy*py*2>abs(sy)*(sceneHeight+offsetLength));
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
    d3d.pushMatrix();
    d3d.translate2D(sceneCenterX, sceneCenterY);
    bulletList.renderAll();
    d3d.popMatrix();
}
