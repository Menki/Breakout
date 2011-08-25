#include "SimpleAudioEngine.h"
#include "GameOverScene.h"
#include "HelloWorldScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

// GameOverScene

GameOver::~GameOver()
{
}

GameOver::GameOver()
{    
}

CCScene* GameOver::scene(const char *labelText)
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	GameOver *layer = GameOver::node();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    layer->setPosition(ccp(winSize.width*0.5f, winSize.height*0.5f));
    
    CCLabelTTF *label = CCLabelTTF::labelWithString(labelText, "Arial", 32.0f);
    layer->addChild(label);
    
	// add layer as a child to scene
	scene->addChild(layer);
    
	// return the scene
	return scene;
}

bool GameOver::init()
{
	// super init first
	if (!CCLayer::init())
	{
		return false;
	}
    
    this->setIsTouchEnabled(true);
    
    return true;
}

void GameOver::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
}

void GameOver::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
}

void GameOver::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

void GameOver::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
}