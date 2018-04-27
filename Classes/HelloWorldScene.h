#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ParticleSystemExtended.h"

class HelloWorld : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
	cocos2d::ParticleSystemQuad* _emitter;

	cocos2d::Sprite* makeBox(cocos2d::Vec2 point, cocos2d::Size size, int color = 0, cocos2d::PhysicsMaterial material = cocos2d::PHYSICSBODY_MATERIAL_DEFAULT);

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
