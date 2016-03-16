#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "texturestrip.h"
#include "keybinding.h"
#include "imagepiece.h"
#include "spiritwheel.h"
#include "numbersprite.h"
#include "score.h"

#include <memory>
using namespace std;

enum PlayerDirection
{
    Dir_None = 0,
    Dir_Left = 0x1,
    Dir_Up = 0x2,
    Dir_Right = 0x4,
    Dir_Down = 0x8,
    Dir_LeftUp = Dir_Left|Dir_Up,
    Dir_LeftDown = Dir_Left|Dir_Down,
    Dir_RightUp = Dir_Right|Dir_Up,
    Dir_RightDown = Dir_Right|Dir_Down,
    Dir_All = 0xF
};

class BulletList;
class NoteScene;

class Player : public Entity
{
public:

    Player();

    void setJudgeList(BulletList* list);

    void setMoveSpeed(double speed);
    void setMoveField(double xmin, double xmax, double ymin, double ymax);

    void setDirection(PlayerDirection dir);

    inline double getJudgeSpan() const
    {
        return judgeSpan;
    }

    void setTarget(SpiritWheel* spirit);
    inline SpiritWheel* getTarget() const
    {
        return target;
    }

    void onHitTarget();

    void checkTargetDist();

    void renderOverlay();

    void setNoteScene(NoteScene* scene);

    void setScoreObj(Score* score);

protected:

    ImagePiece tex;
    vector<ImagePiece> img;

    ImagePiece plotInner;
    ImagePiece plotOuter;

    BulletList* judgeList;

    double judgeSpan;
    double moveSpeed;

    double lastMissTime;
    double invinTime;

    double minX, maxX;
    double minY, maxY;

    int moveFrame;
    int stayFrame;

    double lastHitTime;

    Position targetPos;
    SpiritWheel* target;

    NumberSprite number;

    NoteScene* noteScene;

    Score* score;

    void onUpdateMotion(double deltaSec, double deltaTick);
    void onTick();
    void onRender();
    void onDestroy();

    friend class BulletJudge;

};

#endif // PLAYER_H
