#include "HelloWorldScene.h"



HelloWorld::HelloWorld(){}
HelloWorld::~HelloWorld(){}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    
    scene->getPhysicsWorld()->setGravity(Vect(0.0f, -60*9.8f));
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_NONE);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _player = Sprite::create("star.png");
    _player->setPosition(visibleSize.width/2, visibleSize.height/2);
    _player->setScale(0.5f);
    this->addChild(_player);
    
    auto player_body = PhysicsBody::createCircle(_player->getContentSize().width/4);
    player_body->setMass(0.0f);
    _player->setPhysicsBody(player_body);
    
    
    auto edge_body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT,6);
    
    auto edgeNode = Node::create();
    edgeNode->setPosition(Point(visibleSize.width/2, visibleSize.height/2));
    edgeNode->setPhysicsBody(edge_body);
    this->addChild(edgeNode);

    
    /*
    auto land = LayerColor::create(Color4B(220,220,220,220),visibleSize.width,40.0f);
    land->setPosition(Point::ZERO);
    
    auto land_body = PhysicsBody::createBox(Size(land->getContentSize().width, land->getContentSize().height));
    land_body->setDynamic(false);
    land_body->setPositionOffset(Point(visibleSize.width/2, land->getContentSize().height/2));
    //land_body->setRotationOffset(30.0f);
    land->setPhysicsBody(land_body);
    this->addChild(land);
    */
    
    
    listener->onTouchBegan = [&](Touch *touch, Event *event)
    {
        if (_player->getPhysicsBody()->getVelocity().y >= 0)
        {
            _player->getPhysicsBody()->setVelocity(Vect(0, 600.0f));
            return true;
        }
        
        return false;
    };
    
    listener->onTouchMoved = [](Touch *touch, Event *event)
    {
        
    };
    
    listener->onTouchEnded = [&](Touch *touch, Event *event)
    {
        
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

