#include "bulletscene.h"

#include "commondef.h"
#include "soundregistry.h"
#include "attacktrail.h"

#include "debug.h"

BulletScene::BulletScene()
    :Scene()
    ,bulletList(nullptr)
    ,player(nullptr)
    ,boss(nullptr)
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
    SAFE_RELEASE(boss);
}

int BulletScene::pushBullet(Bullet&& bullet)
{
    return bulletList->pushEntity(std::forward<Bullet>(bullet));
}

int BulletScene::pushBullet(double x, double y, double speed, double angle, int type)
{
    return bulletList->newEntity<Bullet>(x, y, speed, angle, type);
}

int BulletScene::pushSpirit(double x, double y, double speed, double angle, double life, int hp)
{
    int id = bulletList->newEntity<SpiritWheel>(x, y, speed, angle, life, hp);
    spiritList.push_back(id);
    return id;
}

SpiritWheel* BulletScene::nearestSpiritToPlayer(double minDist)
{
    SpiritWheel* res = nullptr;
    for(auto iter = spiritList.begin(); iter!=spiritList.end();)
    {
        auto entity = bulletList->getSpirit(*iter);
        if(entity == nullptr || entity->destroyed())
        {
            iter = spiritList.erase(iter);
            continue;
        }
        double d = entity->distanceTo(*player);
        if(d<minDist)
        {
            res = entity;
            minDist = d;
        }
        ++iter;
    }
    return res;
}

void BulletScene::hitPlayerTargetSpirit()
{
    auto spirit = player->getTarget();
    if(spirit!=nullptr && spirit->distanceTo(*player)<200)
    {
        player->onHitTarget();
        bulletList->newEntity<AttackTrail>(player->getX(), player->getY(), spirit->getX(), spirit->getY());
        if(spirit->hit())
        {
            SOUND("pop")->play(true);
            int count = bulletList->destroyBulletInCircle(spirit->getX(), spirit->getY(), 100);
            bulletList->newEntity<AttackTrail>(player->getX(), player->getY(), spirit->getX(), spirit->getY());
            bulletList->newEntity<AttackTrail>(player->getX(), player->getY(), spirit->getX(), spirit->getY());
            score->addScore(1000+count*100);
        }
        else
        {
            score->addScore(300);
        }
    }
    else
    {
        score->addScore(100);
    }
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

BulletBase* BulletScene::getBullet(int id)
{
    if(id == 0)
        return nullptr;
    if(id == -1)
        return boss;
    return bulletList->getEntity(id);
}

void BulletScene::triggerBomb()
{
    bulletList->destroyBulletInCircle(player->getX(), player->getY()-40, 50);
}

void BulletScene::setJump(double t)
{
    jumpTime = t;
}

void BulletScene::setScoreObj(Score* score)
{
    this->score = score;
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
    player->setScoreObj(score);

    SAFE_RELEASE(boss);
    boss = new Enemy();

    spiritList.clear();
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

    boss->update(deltaTime.beat);

    player->checkTargetDist();
    if(player->getTarget() == nullptr)
    {
        player->setTarget(nearestSpiritToPlayer());
    }

    player->update(deltaTime.time);
}

void BulletScene::render()
{
    d2d.pushMatrix();
    d2d.translate(sceneCenterX, sceneCenterY);
    player->render();
    boss->render();
    bulletList->renderAll();
    player->renderOverlay();
    d2d.popMatrix();
}

