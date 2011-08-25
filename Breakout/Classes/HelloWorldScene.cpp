//
//  HelloWorldScene.cpp
//  BouncingBalls
//
//  Created by Augusto Souza on 8/21/11.
//  Copyright Menki Mobile 2011. All rights reserved.
//
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

enum 
{
    kBallTag = 0,
    kPaddleTag,
    kBlockTag
};

HelloWorld::~HelloWorld()
{
	if (world)
	{
		delete world;
		world = NULL;
	}

    if (ballBody)
	{
		ballBody = NULL;
	}

    if (groundBody)
	{
		groundBody = NULL;
	}

    if (paddleBody)
	{
		paddleBody = NULL;
	}
    
    if (bottomFixture)
	{
		bottomFixture = NULL;
	}

    if (ballFixture)
	{
		ballFixture = NULL;
	}

    if (paddleFixture)
	{
		paddleFixture = NULL;
	}
    
    if (mouseJoint) {
        mouseJoint = NULL;
    }
    
    if (ball)
	{
		ball = NULL;
	}

    if (contactListener)
	{
        delete contactListener;
		contactListener = NULL;
	}
    
    if (m_b2dDebugDraw)
	{
        delete m_b2dDebugDraw;
		m_b2dDebugDraw = NULL;
	}
    
	// cpp don't need to call super dealloc
	// virtual destructor will do this
}

HelloWorld::HelloWorld()
:world(NULL)
,ballBody(NULL)
,groundBody(NULL)
,paddleBody(NULL)
,bottomFixture(NULL)
,ballFixture(NULL)
,paddleFixture(NULL)
,mouseJoint(NULL)
,ball(NULL)
,contactListener(NULL)
,ptmRatio(0)
,m_b2dDebugDraw(NULL)
{
}

CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::node();
    
	// add layer as a child to scene
	scene->addChild(layer);
    
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	// super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
    
    //this->setIsAccelerometerEnabled(true);
    this->setIsTouchEnabled(true);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    ptmRatio = 32; // TODO: Find a PTM RATIO value different for Android and iOS, must be calculated, not defined.
    
    // create a world
    b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
    bool doSleep = false;
    world = new b2World(gravity, doSleep);
        
    // Debug Draw
    m_b2dDebugDraw = NULL;
    // Enable debug draw by un-commenting the next line. NOTE: **** this debug stuff currently isn't displaying correctly, should get fixed soon
    //m_b2dDebugDraw = new GLESDebugDraw(ptmRatio);
    if( m_b2dDebugDraw )
    {
        world->SetDebugDraw(m_b2dDebugDraw);
        
        uint32 flags = 0;
        flags += b2DebugDraw::e_shapeBit;
        // Only need above bit really, add the below flag bits for more detail
        flags += b2DebugDraw::e_jointBit;
        flags += b2DebugDraw::e_aabbBit;
        flags += b2DebugDraw::e_pairBit;
        flags += b2DebugDraw::e_centerOfMassBit;
        m_b2dDebugDraw->SetFlags(flags);
    }
    
    
    // create edges around the entire screen
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0);
    groundBody = world->CreateBody(&groundBodyDef);
    b2PolygonShape groundBox;
    b2FixtureDef boxShapeDef;
    boxShapeDef.shape = &groundBox;
    // bottom
    groundBox.SetAsEdge(b2Vec2(0, 0), b2Vec2(winSize.width/ptmRatio, 0));
    bottomFixture = groundBody->CreateFixture(&boxShapeDef);
    // left
    groundBox.SetAsEdge(b2Vec2(0, 0), b2Vec2(0, winSize.height/ptmRatio));
    groundBody->CreateFixture(&boxShapeDef);
    // top
    groundBox.SetAsEdge(b2Vec2(0, winSize.height/ptmRatio), b2Vec2(winSize.width/ptmRatio, winSize.height/ptmRatio));
    groundBody->CreateFixture(&boxShapeDef);    
    // right
    groundBox.SetAsEdge(b2Vec2(winSize.width/ptmRatio, winSize.height/ptmRatio), b2Vec2(winSize.width/ptmRatio, 0));
    groundBody->CreateFixture(&boxShapeDef);
    
    // create a sprite and add it to the layer
    ball = CCSprite::spriteWithFile("Ball.jpg");
    CCSize ballSize = ball->getContentSize();
    ball->setPosition(ccp(100, 100));
    ball->setTag(kBallTag);
    this->addChild(ball);
    
    // create ball body and shape
    // Density is mass per unit volume. So the more dense an object is, the more mass it has, and the harder it is to move.
    // Friction is a measure of how hard it is for objects to slide against each other. This should be in the range of 0 to 1. 0 means there is no friction, and 1 means there is a lot of friction.
    // Restitution is a measure of how “bouncy” an object is. This should usually be in the range of 0 to 1. 0 means the object will not bounce, and 1 means the bounce is perfectly elastic, meaning it will bounce away with the same velocity that it impacted an object.    
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    CCPoint ballPos = ball->getPosition();
    ballBodyDef.position.Set(ballPos.x/ptmRatio, ballPos.y/ptmRatio);
    ballBodyDef.userData = ball;
    ballBody = world->CreateBody(&ballBodyDef);
    
    b2CircleShape circle;
    circle.m_radius = ballSize.width*0.5f / ptmRatio;
        
    b2FixtureDef ballShapeDef;
    ballShapeDef.shape = &circle;
    ballShapeDef.density = 1.0f;
    ballShapeDef.friction = 0.f;
    ballShapeDef.restitution = 1.0f;
    ballFixture = ballBody->CreateFixture(&ballShapeDef);
    
    // apply a force in the ball to change the default angle 
    b2Vec2 force = b2Vec2(10, 10);
    ballBody->ApplyLinearImpulse(force, ballBodyDef.position);
    
    // create paddle and add it to the layer
    CCSprite *paddle = CCSprite::spriteWithFile("Paddle.jpg");
    CCSize paddleSize = paddle->getContentSize();
    CCPoint paddlePos = ccp(winSize.width*0.5f, 50);
    paddle->setPosition(paddlePos);
    paddle->setTag(kPaddleTag);
    this->addChild(paddle);
    
    // create paddle body
    b2BodyDef paddleBodyDef;
    paddleBodyDef.type = b2_dynamicBody;
    paddleBodyDef.position.Set(paddlePos.x/ptmRatio, 50/ptmRatio);
    paddleBodyDef.userData = paddle;
    paddleBody = world->CreateBody(&paddleBodyDef);
    
    // create paddle shape
    b2PolygonShape paddleShape;
    paddleShape.SetAsBox((paddleSize.width/ptmRatio)*0.5f, (paddleSize.height/ptmRatio)*0.5f);
    
    // create shape definition and add to body
    b2FixtureDef paddleShapeDef;
    paddleShapeDef.shape = &paddleShape;
    paddleShapeDef.density = 10.0f;
    paddleShapeDef.friction = 0.4f;
    paddleShapeDef.restitution = 0.1f;
    paddleFixture = paddleBody->CreateFixture(&paddleShapeDef);
    
    // restrict paddle along the x axis
    b2PrismaticJointDef jointDef;
    b2Vec2 worldAxis(1.0f, 0.0f);
    jointDef.collideConnected = true;
    jointDef.Initialize(paddleBody, groundBody, paddleBody->GetWorldCenter(), worldAxis);
    world->CreateJoint(&jointDef);
    
    // create contact listener
    contactListener = new MyContactListener();
    world->SetContactListener(contactListener);
    
    // create blocks
    static int padding = 20;
    for (int i = 0; i < 4; i++)
    {
        // create block and add it to the layer
        CCSprite *block = CCSprite::spriteWithFile("Block.jpg");
        CCSize blockSize = block->getContentSize();
        int xOffSet = padding + blockSize.width*0.5f + (blockSize.width + padding)*i;
        block->setPosition(ccp(xOffSet, 300));
        block->setTag(kBlockTag);
        this->addChild(block);
        
        // create block body
        b2BodyDef blockBodyDef;
        blockBodyDef.type = b2_dynamicBody;
        blockBodyDef.position.Set(xOffSet/ptmRatio, 300/ptmRatio);
        blockBodyDef.userData = block;
        b2Body *blockBody = world->CreateBody(&blockBodyDef);
        
        // create block shape
        b2PolygonShape blockShape;
        blockShape.SetAsBox((blockSize.width/ptmRatio)*0.5f, (blockSize.height/ptmRatio)*0.5f);
        
        // create shape definition and add to body
        b2FixtureDef blockShapeDef;
        blockShapeDef.shape = &blockShape;
        blockShapeDef.density = 10.0f;
        blockShapeDef.friction = 0.0f;
        blockShapeDef.restitution = 0.1f;
        blockBody->CreateFixture(&blockShapeDef);
    }
    
    
    this->schedule(schedule_selector(HelloWorld::tick));
    
	return true;
}

void HelloWorld::tick(cocos2d::ccTime delta) 
{
    bool blockFound = false;
    
    // velocity iterations and position iterations should usually be setted in the range of 8-10
    int velocityIterations = 10;
    int positionIterations = 10;
    world->Step(delta, velocityIterations, positionIterations);
        
    for (b2Body *b = world->GetBodyList(); b != NULL; b = b->GetNext()) 
    {
        CCSprite *sprite = (CCSprite *)b->GetUserData();
        if (sprite != NULL) 
        {
            if (sprite->getTag() == kBlockTag) 
            {
                blockFound = true;
            }
            
            sprite->setPosition(ccp(b->GetPosition().x * ptmRatio, b->GetPosition().y * ptmRatio));
            sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
            
            
            if (sprite->getTag() == kBallTag)
            {
                static int maxSpeed = 10;
                b2Vec2 velocity = b->GetLinearVelocity();
                float32 speed = velocity.Length();

                if (speed > maxSpeed)
                {
                    b->SetLinearDamping(0.5f);
                }
                else if (speed < maxSpeed)
                {
                    b->SetLinearDamping(0.0);
                }

            }
        }
    }
    
    if (!blockFound)
    {
        CCScene *gameOverScene = GameOver::scene("You win!");
        CCDirector::sharedDirector()->replaceScene(gameOverScene);
    }    
    
    std::vector<b2Body *> toDestroy;
    std::vector<MyContact>::iterator pos;
    for (pos = contactListener->contacts.begin(); pos != contactListener->contacts.end(); ++pos) 
    {
        MyContact contact = *pos;
        
        if ((contact.fixtureA == bottomFixture && contact.fixtureB == ballFixture) ||
            (contact.fixtureA == ballFixture && contact.fixtureB == bottomFixture))      
        {
            CCScene *gameOverScene = GameOver::scene("You loose!");
            CCDirector::sharedDirector()->replaceScene(gameOverScene);         
        }
        
        b2Body *bodyA = contact.fixtureA->GetBody();
        b2Body *bodyB = contact.fixtureB->GetBody();
        CCSprite *spriteA = (CCSprite *) bodyA->GetUserData();
        CCSprite *spriteB = (CCSprite *) bodyB->GetUserData();
        if (spriteA != NULL && spriteB != NULL) 
        {
            // sprite A = ball and sprite B = block
            if (spriteA->getTag() == kBallTag && spriteB->getTag() == kBlockTag) 
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyB) == toDestroy.end())
                    toDestroy.push_back(bodyB);
            }
            // sprite A = block and sprite B = ball
            else if (spriteA->getTag() == kBlockTag && spriteB->getTag() == kBallTag) 
            {
                if (std::find(toDestroy.begin(), toDestroy.end(), bodyA) == toDestroy.end())
                    toDestroy.push_back(bodyA);
            }
        }
    }
    
    std::vector<b2Body *>::iterator pos2;
    for (pos2 = toDestroy.begin(); pos2 != toDestroy.end(); ++pos2) 
    {
        b2Body *body = *pos2;
        if (body->GetUserData() != NULL) 
        {
            CCSprite *sprite = (CCSprite *) body->GetUserData();
            this->removeChild(sprite, true);
        }
        world->DestroyBody(body);
    }
}

void HelloWorld::didAccelerate(CCAcceleration* pAccelerationValue)
{
    b2Vec2 gravity(-pAccelerationValue->y * 15, pAccelerationValue->x * 15);
    world->SetGravity(gravity);
}

void HelloWorld::draw(void)
{
    CCLayer::draw();
    
    if( m_b2dDebugDraw )
    {
        glDisable(GL_TEXTURE_2D);                               
        glDisableClientState(GL_COLOR_ARRAY);               // so polys draw properly and plain colour
        //        glDisableClientState(GL_TEXTURE_COORD_ARRAY);       // doesnt seem to matter
        glEnableClientState(GL_VERTEX_ARRAY);             // vertices are drawn with this flag set
        
        world->DrawDebugData();
        
        glDisableClientState(GL_VERTEX_ARRAY);   
        //        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glEnable(GL_TEXTURE_2D);
    }
    
}

void HelloWorld::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    if (mouseJoint != NULL) return;
    
    CCTouch *myTouch = (CCTouch *) pTouches->anyObject();
    CCPoint location = myTouch->locationInView(myTouch->view());
    location = CCDirector::sharedDirector()->convertToGL(location);
    b2Vec2 locationWorld = b2Vec2(location.x/ptmRatio, location.y/ptmRatio);
    
    if (paddleFixture->TestPoint(locationWorld)) {
        b2MouseJointDef md; 
        md.bodyA = groundBody;
        md.bodyB = paddleBody;
        md.target = locationWorld;
        md.collideConnected = true;
        md.maxForce = 1000.0f * paddleBody->GetMass();
        
        mouseJoint = (b2MouseJoint *) world->CreateJoint(&md);
        paddleBody->SetAwake(true);
    }
}

void HelloWorld::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    if (mouseJoint == NULL) return;
    
    CCTouch *myTouch = (CCTouch *) pTouches->anyObject();
    CCPoint location = myTouch->locationInView(myTouch->view());
    location = CCDirector::sharedDirector()->convertToGL(location);
    b2Vec2 locationWorld = b2Vec2(location.x/ptmRatio, location.y/ptmRatio);

    mouseJoint->SetTarget(locationWorld);
}

void HelloWorld::ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    if (mouseJoint) {
        world->DestroyJoint(mouseJoint);
        mouseJoint = NULL;
    }
}

void HelloWorld::ccTouchesCancelled(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
    if (mouseJoint) {
        world->DestroyJoint(mouseJoint);
        mouseJoint = NULL;
    }    
}

