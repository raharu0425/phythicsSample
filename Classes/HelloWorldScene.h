#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include "GLES-Render.h"

#include "extensions/cocos-ext.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCCustomCommand.h"

USING_NS_CC;
USING_NS_CC_EXT;

class HelloWorld : public cocos2d::Layer
{
public:
    HelloWorld();
    ~HelloWorld();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void update(float delta);
    b2Body* generateWaves(float const maxLength, float const maxHeight);
    
    void initPhysicWorld();
    //virtual void onDraw();
    
    void getWavePoints(Point container[], const Point start, const float max_length, const int num_of_waves, const float height, const int smoother);
    
    void genWavePoints(Point container[], Point points[], const int point_num, const int smoother);
    
    Point lerp(Point start, Point end, float time);
    Point bezier(Point start, Point end, Point c_start, Point c_end, float time);
    
    
    virtual void draw(Renderer *renderer, const kmMat4 &transform, bool transformUpdated) override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
protected:
    kmMat4 _modelViewMV;
    void onDraw();
    CustomCommand _customCommand;
    
    
    
private:
    bool inMap1;
    float passLength;
    
    Point _last_point;
    b2Body *_waves[2];
    
    Sprite * _player;
    float _player_vspeed;
    float _player_aspeed;
    
    Node* _ground_node;
    PhysicsBody *_ground;
    Point _ground_next_pos;
    
    b2Body* bodies[2];
    
    b2Draw *_debugDraw;
    
    b2World *_world;
    
    
    Texture2D *_texture;
    
    
    
    
    int getRand(const int min, const int max);
};

#endif // __HELLOWORLD_SCENE_H__
