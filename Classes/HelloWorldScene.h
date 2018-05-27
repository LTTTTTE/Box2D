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

	void onEnter();
	void tick(float dt);

	bool createBox2dWorld(bool debug);
	void setBox2dWorld();
	bool onTouchBegan(Touch* touch, Event* event);
	void addNewSpriteAtPosition(Vec2 location);
	void addNewSpriteAtPosition2(Vec2 location);


	CREATE_FUNC(HelloWorld);


};

