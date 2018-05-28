#include "cocos2d.h"
#include "Box2D\Box2D.h"
#include <GLES-Render.h>

#pragma execution_character_set("utf-8")

USING_NS_CC;

#define PTM_RATIO 32
#define winsize Director::getInstance()->getWinSize()
#define MT19937 std::uniform_int_distribution<__int64>

class HelloWorld : public Scene {

public:

	bool debug = false;
	bool bDrag;

	static Scene* createScene();
	virtual bool init();

	Texture2D* texture; 
	Texture2D* texture_block;

	b2World* world;

	//debugMode//
	GLESDebugDraw* debugDraw;
	CustomCommand custom_cmd;
	
	virtual void draw(Renderer* renderer, const Mat4& transform, uint32_t flags) override;
	void onDraw(const Mat4& transform, uint32_t flags);
	////

	b2Body* dragbody;
	b2Body* gbody;
	b2MouseJoint* mouseJ;
	
	b2Body* addNewSprite(Vec2 point, Size size, b2BodyType bodytype, const char* spriteName, int type);
	b2Body* getBodyAtTab(Vec2 point);

	void onEnter();
	void tick(float dt);

	bool createBox2dWorld(bool debug);
	void setBox2dWorld();


	bool onTouchBegan(Touch* touch, Event* event);
	void onTouchMoved(Touch* touch, Event* event);
	void onTouchEnded(Touch* touch, Event* event);


	CREATE_FUNC(HelloWorld);


};

