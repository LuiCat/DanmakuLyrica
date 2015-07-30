#include "scenemanager.h"

SceneManager::SceneManager()
    :coverScene(0)
{

}

SceneManager::~SceneManager()
{
    clearAllScene();
}

void SceneManager::pushStack(Scene* scene)
{
    stackScene.push_back(scene);
    scene->load();
}

void SceneManager::popStack()
{
    if(!stackScene.empty())
    {
        topScene()->unload();
        stackScene.pop_back();
    }
}

void SceneManager::setCoverScene(Scene* scene)
{
    coverScene=scene;
}

void SceneManager::changeScene(Scene* scene)
{
    if(!scene)
        return;
    popStack();
    pushStack(scene);
}

void SceneManager::pushScene(Scene* scene)
{
    if(!scene)
        return;
    pushStack(scene);
}

void SceneManager::popScene()
{
    popStack();
}

void SceneManager::popSceneAbove(Scene* scene)
{
    while(!sceneEmpty()&&topScene()!=scene)
        popStack();
}

void SceneManager::removeScene(Scene* scene)
{
    if(!scene)
        return;
    for(SceneStack::iterator iter=stackScene.begin();iter!=stackScene.end();++iter)
    {
        if(scene==*iter)
        {
            iter=stackScene.erase(iter);
        }
    }
}

void SceneManager::clearAllScene()
{
    stackScene.clear();
    coverScene=0;
}

void SceneManager::updateScene(double deltaSec, bool updateCoveredScene)
{
    if(coverScene)
    {
        coverScene->update(deltaSec);
        if(!updateCoveredScene)
            return;
    }

    for(Scene* p : stackScene)
        p->update(deltaSec);

}

void SceneManager::renderScene()
{
    for(Scene* p : stackScene)
        p->render();
    if(coverScene)
        coverScene->render();
}
