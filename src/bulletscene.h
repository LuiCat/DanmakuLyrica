#ifndef BULLETSCENE_H
#define BULLETSCENE_H

#include "bulletdef.h"
#include "bullet.h"
#include "bulletlist.h"
#include "scene.h"
#include "player.h"
#include "spiritwheel.h"
#include "enemy.h"
#include "score.h"

#include <list>
#include <set>
#include <random>

using namespace std;

class BulletScene : public Scene
{
private:

    BulletList* bulletList;
    //list<LuaTask*> taskList;

    list<int> spiritList;

    Player* player;
    Enemy* boss;

    double jumpTime;

    Score* score;

public:

    double sceneCenterX, sceneCenterY;
    double sceneWidth, sceneHeight;
    double offsetBorderWidth;

    mt19937 random;

    BulletScene();
    virtual ~BulletScene();

    int pushBullet(Bullet&& bullet);
    int pushBullet(double x, double y, double speed, double angle, int type);

    int pushSpirit(double x, double y, double speed, double angle, double life, int hp=1);

    SpiritWheel* nearestSpiritToPlayer(double minDist = M_DINF);

    void hitPlayerTargetSpirit();

    bool checkSceneBorder(Entity* entity);

    void setPlayerMotion(PlayerDirection dir);

    int getBulletSize() const;
    BulletList* getBulletList() const;

    BulletBase* getBullet(int id);

    void triggerBomb();

    inline Player* getPlayer()
    {
        return player;
    }

    void setJump(double t);
    inline double getJump() const
    {
        return jumpTime;
    }

    void setScoreObj(Score* score);

    void load();
    void unload();

    void update();
    void render();

    friend class BulletScript;

};


#endif // BULLETSCENE_H
