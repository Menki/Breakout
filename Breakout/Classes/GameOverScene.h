#include "cocos2d.h"

class GameOver : public cocos2d::CCLayer
{
public:
    GameOver();
	~GameOver();
    
    static cocos2d::CCScene* scene(const char *labelText);
    virtual bool init();
    LAYER_NODE_FUNC(GameOver);
    
    virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	virtual void ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
};