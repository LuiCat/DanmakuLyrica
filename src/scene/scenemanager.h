#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "scene.h"
#include "beattime.h"

#include <list>
using namespace std;

class SceneManager
{
private:

    typedef list<Scene*> SceneStack;
    SceneStack stackScene;

    Scene* coverScene;

    void pushStack(Scene* scene);
    void popStack();

public:

    explicit SceneManager();
    ~SceneManager();

    inline bool sceneEmpty() const
    {
        return stackScene.empty();
    }

    inline Scene* topScene()
    {
        if(stackScene.empty())
            return 0;
        return stackScene.back();
    }

    void setCoverScene(Scene* scene=0);

    void changeScene(Scene* scene);
    void pushScene(Scene* scene);
    void popScene();
    void popSceneAbove(Scene* scene);
    void removeScene(Scene* scene);
    void clearAllScene();

    void updateScene(const BeatTime& beatTime, bool updateCoveredScene=false);
    void renderScene();

};

#endif // SCENEMANAGER_H
