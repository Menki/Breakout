//
//  HelloWorldScene.h
//  BouncingBalls
//
//  Created by Augusto Souza on 8/21/11.
//  Copyright Menki Mobile 2011. All rights reserved.
//
#ifndef __HELLO_WORLD_H__
#define __HELLO_WORLD_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Box2D.h"
#include "GLES-Render.h"
#include "MyContactListener.h"

class HelloWorld : public cocos2d::CCLayer 
{
public:
    HelloWorld();
	~HelloWorld();
    
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
    
	// there's no 'id' in cpp, so we recommand to return the exactly class pointer
	static cocos2d::CCScene* scene();
    
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(HelloWorld);

    virtual void draw(void);
private:
    virtual void didAccelerate(cocos2d::CCAcceleration* pAccelerationValue);
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);    
    void tick(cocos2d::ccTime delta);
    
    b2World *world;
    b2Body *ballBody;
    b2Body *groundBody;
    b2Body *paddleBody;
    b2Fixture *bottomFixture;
    b2Fixture *ballFixture;
    b2Fixture *paddleFixture;
    b2MouseJoint *mouseJoint;
    cocos2d::CCSprite *ball;
    
    MyContactListener *contactListener;
    
    int ptmRatio;
    
    GLESDebugDraw *m_b2dDebugDraw;
};

#endif // __HELLO_WORLD_H__
