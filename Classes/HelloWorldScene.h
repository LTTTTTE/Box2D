#include "cocos2d.h"
#include "Box2D\Box2D.h"
#pragma execution_character_set("utf-8")

USING_NS_CC;

#define PTM_RATIO 32
#define winsize Director::getInstance()->getWinSize()

class HelloWorld : public Scene {

public:


	static Scene* createScene();
	virtual bool init();

	Texture2D* texture;
	b2World* world;

	void onEnter();
	void tick(float dt);

	bool onTouchBegan(Touch* touch, Event* event);
	void addNewSpriteAtPosition(Vec2 location);


	CREATE_FUNC(HelloWorld);


};

