#include "scene.h"

#include "scenemanager.h"

Scene::Scene()
    :manager(0)
{

}

void Scene::updateTime(const BeatTime & beatTime)
{
    sceneTime = beatTime;
}

void Scene::proceedTime()
{
    sceneTime.proceed();
}

void Scene::setManager(SceneManager* newManager)
{
    if(newManager!=0)
        manager=newManager;
}

