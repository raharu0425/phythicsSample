#include "HelloWorldScene.h"


#define PI 3.1415926f
#define PTM_RATIO 32

#define MAX_NUM 8
#define MAP_LENGTH 6000.0f


HelloWorld::HelloWorld():
_last_point(Point(0,0)),
inMap1(true),
passLength(0.0f)
{
    this->initPhysicWorld();
}

HelloWorld::~HelloWorld()
{
    delete _world;
    _world = NULL;
}

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

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
    
    srand(time(NULL));
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    _texture = Director::getInstance()->getTextureCache()->addImage("star.png");

    
    //b2EdgeShape groundBox;

    // ポイントを格納配列を作成
    /*
    std::vector<Point>randPoints;
    
    float max_length = 24000.0f;
    float max_height = 800.0f;
    
    float current_length = 0.0f;
    for (int i = 0; i < MAX_NUM; i++) {
        float the_rate = static_cast<float>(1+static_cast<float>(getRand(2, 6)/10));
        float the_rate2 = max_length*static_cast<float>(getRand(3, 10)/10);
        float the_length = the_rate*max_length/MAX_NUM;
        randPoints.push_back(Point(_last_point.x + current_length, the_rate2*max_height));
        
        current_length+=the_length;
    }
    _last_point = randPoints.back();
    
    Point the_points[MAX_NUM];
    //{Point(0.0f, 500.0f), Point(580.0f, 100.0f), Point(1136.0f, 50.0f),  Point(2136.0f, 200.0f),Point(3136.0f, 300.0f),Point(4136.0f, 500.0f),Point(5636.0f, 100.0f)};
    std::copy(randPoints.begin(), randPoints.end(), the_points);
    randPoints.clear();
    
    
    Point points[MAX_NUM*16];
    genWavePoints(points, the_points, MAX_NUM, sizeof(points)/sizeof(Point));
    // ポイントを作成し、points[]に入れる。
    // getWavePoints(格納配列, スタートポイント, 長さ, 波の数, 波の幅, ポイントの数);
    
    b2Vec2 vectors[MAX_NUM*16];
    
    
    b2BodyDef ground_body_def;
    ground_body_def.type = b2_staticBody;
    //ground_body_def.position.Set(0, 0);
    b2Body *groundBody = _world->CreateBody(&ground_body_def);
    b2ChainShape wave;
    for (int i=0; i<sizeof(points)/sizeof(Point); i++) {
        CCLOG("Point => (%f, %f)",points[i].x, points[i].y);
        vectors[i].Set(points[i].x/PTM_RATIO, points[i].y/PTM_RATIO);
    }
    wave.CreateChain(vectors, sizeof(vectors)/sizeof(b2Vec2));
    groundBody->CreateFixture(&wave,0);
    */
    
    _waves[0] = this->generateWaves(MAP_LENGTH, 300.0f);
    _waves[1] = this->generateWaves(MAP_LENGTH, 440.0f);
    
    
    _player = Sprite::create("star.png");
    
    b2BodyDef player_body_def;
    player_body_def.type = b2_dynamicBody;
    player_body_def.position.Set(visibleSize.width/4/PTM_RATIO, visibleSize.width/2/PTM_RATIO);
    b2Body *body = _world->CreateBody(&player_body_def);
    
    b2CircleShape circle;
    circle.m_radius = 36.0/PTM_RATIO;
    
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 2;
    fixtureDef.restitution = 0;
    fixtureDef.friction = 0.7;
    
    body->CreateFixture(&fixtureDef);
    
    _player->setScale(0.5f);
    
    CCLOG("Player L:(%f, %f)",body->GetPosition().x,body->GetPosition().y);
    
    _player->setPosition(body->GetPosition().x*PTM_RATIO, body->GetPosition().y*PTM_RATIO);
    body->SetUserData(_player);
    
    bodies[0] = body;
    
    
    auto parallax = ParallaxNode::create();
    
    this->addChild(_player);
    this->runAction(Follow::create(_player));
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
        bodies[0]->ApplyForceToCenter(b2Vec2(50*PTM_RATIO, 0*PTM_RATIO),true);
        return true;
    };
    
    listener->onTouchMoved = [](Touch *touch, Event *event)
    {
        
    };
    
    listener->onTouchEnded = [&](Touch *touch, Event *event)
    {
        
    };
    
    this->scheduleUpdate();
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void HelloWorld::getWavePoints(Point container[], const Point start , const float max_length, const int num_of_waves, const float height, const int smoother)
{
    std::vector<Point>points;
    if (smoother < 2) {
        return;
    }
    
    float length = static_cast<float>(max_length/(smoother-1));
    float radius = PI*num_of_waves/(smoother-1);
    
    CCLOG("theLength: %f", length);
    
    float r = 0;
    float the_length = 0;
    int s = 0;
    
    while(s < smoother)
    {
        points.push_back(Point(the_length, static_cast<int>(sin(r)*height)+start.y));
        the_length += length;
        r+=radius;
        s++;
    }
    
    std::copy(points.begin(), points.end(), container);
    points.clear();
}

int HelloWorld::getRand(const int min, const int max)
{
    if (min > max) return 0;
    return rand() % (max-min) + min;
}

void HelloWorld::genWavePoints(cocos2d::Point container[], cocos2d::Point points[], int point_num, int smoother)
{
    std::vector<Point>return_points;
    
    int smoother_num = (smoother % (point_num-1)) != 0 ? smoother / (point_num-1)+1 : smoother / (point_num-1);
    
    CCLOG("smoother number:%i", smoother_num);
    
    int p_n = 0;
    
    while (p_n < (point_num-1))
    {
        float average = abs(points[p_n].x - points[p_n+1].x);
        float curve_1_length = static_cast<float>(getRand(static_cast<int>(average*.3), static_cast<int>(average*.5)));
        float height1 = static_cast<float>(getRand(800, 1500)/1000)*points[p_n].y;
        float height2 = static_cast<float>(getRand(800, 1500)/1000)*points[p_n+1].y;
        
        for (int t = 0; t < smoother_num; t++)
        {
            return_points.push_back(bezier(points[p_n], points[p_n+1], points[p_n]+Point(curve_1_length/2, height1), points[p_n+1]-Point((average-curve_1_length)/1.5, height2), static_cast<float>(t)/smoother_num));
        }
        p_n++;
    }
    
    std::copy(return_points.begin(), return_points.end(), container);
    return_points.clear();
}

Point HelloWorld::lerp(cocos2d::Point start, cocos2d::Point end, float time)
{
    return start + (end - start)*time;
}

Point HelloWorld::bezier(cocos2d::Point start, cocos2d::Point end, cocos2d::Point c_start, cocos2d::Point c_end, float time)
{
    Point ab = lerp(start, c_start, time);
    Point bc = lerp(c_start, c_end, time);
    Point cd = lerp(c_end, end, time);
    Point abbc = lerp(ab, bc, time);
    Point bccd = lerp(bc, cd, time);
    
    return lerp(abbc, bccd, time);
}

void HelloWorld::update(float delta)
{
    _world->Step(delta, 8 , 1);

    for (b2Body* b = _world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            Sprite* myActor = (Sprite*)b->GetUserData();
            myActor->setPosition( Point( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
        }
    }
    
    
    if (_player->getPosition().x - passLength >= MAP_LENGTH*1.2) {
        b2Body* the_map;
        if (inMap1) {
            the_map = _waves[0];
            _waves[0] = this->generateWaves(MAP_LENGTH, 360.0f);
        } else {
            the_map = _waves[1];
            _waves[1] = this->generateWaves(MAP_LENGTH, 400.0f);
        }
        the_map->GetWorld()->DestroyBody(the_map);
        inMap1 = !inMap1;
        passLength+=MAP_LENGTH;
        
        CCLOG("Map:%f", passLength);
    }
    
    
    
    CCLOG("Position:(%f, %f)", _player->getPosition().x, _player->getPosition().y);
    
    
    
    
}

void HelloWorld::initPhysicWorld()
{
    b2Vec2 gravity;
    gravity.Set(0.0f, -20.0f);
    
    _world = new b2World(gravity);
    
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    
    _debugDraw = new GLESDebugDraw( PTM_RATIO );
    _world->SetDebugDraw(_debugDraw);
    
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    _debugDraw->SetFlags(flags);
}



b2Body* HelloWorld::generateWaves(const float maxLength, const float maxHeight)
{
    float current_length = 0.0f;
    std::vector<Point>randPoints;

    for (int i = 0; i < MAX_NUM; i++) {
        CCLOG("Looping");
        float the_rate = static_cast<float>(this->getRand(70, 130))/100.0f;
        float the_rate2 = static_cast<float>(this->getRand(30, 100))/100.0f;
        float the_length = the_rate*static_cast<float>(maxLength/MAX_NUM);
        
        if (i == 0 && _last_point.x > 0) {
            CCLOG("connect!!");
            randPoints.push_back(Point(_last_point.x, _last_point.y));
        }
        else if (i == MAX_NUM-1)
        {
            randPoints.push_back(Point(_last_point.x + maxLength-100.0f, the_rate2*maxHeight+400.0f));
        }
        else
        {
            randPoints.push_back(Point(_last_point.x + current_length, the_rate2*maxHeight));
        }
        
        CCLOG("Point ======> (%f, %f)",randPoints.back().x, randPoints.back().y);
        current_length+=the_length;
    }
    _last_point = Point(randPoints.back().x+100.0f, randPoints.back().y-400.0f);
    
    Point the_points[MAX_NUM];
    //{Point(0.0f, 500.0f), Point(580.0f, 100.0f), Point(1136.0f, 50.0f),  Point(2136.0f, 200.0f),Point(3136.0f, 300.0f),Point(4136.0f, 500.0f),Point(5636.0f, 100.0f)};
    std::copy(randPoints.begin(), randPoints.end(), the_points);
    //randPoints.clear();
    
    
    Point points[MAX_NUM*16];
    genWavePoints(points, the_points, MAX_NUM, sizeof(points)/sizeof(Point));
    // ポイントを作成し、points[]に入れる。
    // getWavePoints(格納配列, スタートポイント, 長さ, 波の数, 波の幅, ポイントの数);
    
    b2Vec2 vectors[MAX_NUM*16];
    
    
    b2BodyDef ground_body_def;
    ground_body_def.type = b2_staticBody;
    //ground_body_def.position.Set(0, 0);
    b2Body *groundBody = _world->CreateBody(&ground_body_def);
    b2ChainShape wave;
    for (int i=0; i<sizeof(points)/sizeof(Point); i++) {
        //CCLOG("Point => (%f, %f)",points[i].x, points[i].y);
        vectors[i].Set(points[i].x/PTM_RATIO, points[i].y/PTM_RATIO);
    }
    wave.CreateChain(vectors, sizeof(vectors)/sizeof(b2Vec2));
    groundBody->CreateFixture(&wave,0);
    
    return groundBody;
}















void HelloWorld::draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated)
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    Layer::draw(renderer, transform, transformUpdated);
    
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    
    kmGLPushMatrix();
    kmGLGetMatrix(KM_GL_MODELVIEW, &_modelViewMV);
    
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(HelloWorld::onDraw, this);
    renderer->addCommand(&_customCommand);
    
    kmGLPopMatrix();
}

void HelloWorld::onDraw()
{
    kmMat4 oldMV;
    kmGLGetMatrix(KM_GL_MODELVIEW, &oldMV);
    kmGLLoadMatrix(&_modelViewMV);
    _world->DrawDebugData();
    kmGLLoadMatrix(&oldMV);
}
/*
void HelloWorld::onDraw()
{
    
    glEnableVertexAttribArray(kCCVertexAttribFlag_Position);
    kmGLPushMatrix();
    _world->DrawDebugData();
    kmGLPopMatrix();
}
 */
