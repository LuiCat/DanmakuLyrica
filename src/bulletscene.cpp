#include "bulletscene.h"

#include "commondef.h"

#include "debug.h"

BulletScene::BulletScene()
    :Scene()
    ,bulletList(nullptr)
    ,player(nullptr)
    ,sceneCenterX(WIDTH/2)
    ,sceneCenterY(HEIGHT/2)
    ,sceneWidth(WIDTH)
    ,sceneHeight(HEIGHT)
    ,offsetBorderWidth(50.0)
{

}

BulletScene::~BulletScene()
{
    SAFE_RELEASE(bulletList);
    SAFE_RELEASE(player);
}

int BulletScene::pushBullet(Bullet&& bullet)
{
    return bulletList->pushEntity(std::forward<Bullet>(bullet));
}

int BulletScene::pushBullet(double x, double y, double speed, double angle, int type)
{
    return bulletList->newEntity<Bullet>(x, y, speed, angle, type);
}

bool BulletScene::checkSceneBorder(Entity *entity)
{
    double px=entity->getX(), py=entity->getY();
    double sx=entity->getSpeedX(), sy=entity->getSpeedY();
    return (sx*px*2>abs(sx)*(sceneWidth+offsetBorderWidth)||
            sy*py*2>abs(sy)*(sceneHeight+offsetBorderWidth));
}

void BulletScene::setPlayerMotion(PlayerDirection dir)
{
    player->setDirection(dir);
}

BulletList* BulletScene::getBulletList() const
{
    return bulletList;
}

void BulletScene::triggerBomb()
{
    bulletList->destroyBulletInCircle(player->getX(), player->getY()+40, 50);
}

int BulletScene::getBulletSize() const
{
    return bulletList->size();
}

void BulletScene::load()
{
    random.seed();

    SAFE_RELEASE(bulletList);
    bulletList = new BulletList();

    SAFE_RELEASE(player);
    player = new Player();
    player->setJudgeList(bulletList);
    player->setMoveField(-380, 380, -270, 200);
}

void BulletScene::unload()
{
    bulletList->clearAll();
    SAFE_RELEASE(bulletList);
    SAFE_RELEASE(player);
}

void BulletScene::update(rtime_t deltaTime)
{
    bulletList->updateAll(deltaTime.beat);
    bulletList->checkOutsideScene(-sceneWidth*0.5-offsetBorderWidth,
                                  -sceneHeight*0.5-offsetBorderWidth,
                                  sceneWidth*0.5+offsetBorderWidth,
                                  sceneHeight*0.5+offsetBorderWidth);

    player->update(deltaTime.sec);
}

void BulletScene::render()
{
    d3d.pushMatrix();
    d3d.translate2D(sceneCenterX, sceneCenterY);
    player->render();
    bulletList->renderAll();
    d3d.popMatrix();
}

