#include "bulletscene.h"

#include "commondef.h"

#include "debug.h"

BulletScene::BulletScene()
    :Entity()
    ,sceneCenterX(WIDTH/2)
    ,sceneCenterY(HEIGHT/2)
    ,sceneWidth(WIDTH)
    ,sceneHeight(HEIGHT)
    ,offsetBorderWidth(50.0)
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
    return bulletList.newEntity<Bullet>(x, y, speed, angle, type);
}

bool BulletScene::checkSceneBorder(Entity *entity)
{
    double px=entity->getX(), py=entity->getY();
    double sx=entity->getSpeedX(), sy=entity->getSpeedY();
    return (sx*px*2>abs(sx)*(sceneWidth+offsetBorderWidth)||
            sy*py*2>abs(sy)*(sceneHeight+offsetBorderWidth));
}

int BulletScene::getBulletSize() const
{
    return bulletList.size();
}

void BulletScene::onUpdateMotion(double deltaSec, double)
{
    bulletList.updateAll(deltaSec);

    bulletList.checkOutsideScene(sceneCenterX-sceneWidth-offsetBorderWidth,
                                 sceneCenterY-sceneHeight-offsetBorderWidth,
                                 sceneCenterX+sceneWidth+offsetBorderWidth,
                                 sceneCenterY+sceneHeight+offsetBorderWidth);
}

void BulletScene::onRender()
{
    d3d.pushMatrix();
    d3d.translate2D(sceneCenterX, sceneCenterY);
    bulletList.renderAll();
    d3d.popMatrix();
}
