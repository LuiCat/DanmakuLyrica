#ifndef SCENE_H
#define SCENE_H

class SceneManager;

class Scene
{
protected:

    SceneManager* manager;

    void setManager(SceneManager* newManager);

public:

    explicit Scene();

    virtual void load()=0;
    virtual void unload()=0;

    virtual void update(double deltaSec)=0;
    virtual void render()=0;

    friend class SceneManager;

};

#endif // SCENE_H
