#include "cocos2d.h"
#include "Box2D\Box2D.h"

#pragma execution_character_set("utf-8")

USING_NS_CC;

#define winsize Director::getInstance()->getWinSize()
#define MT19937 std::uniform_int_distribution<__int64>
#define PTM_RATIO 76.8

class HelloWorld : public Scene, public b2RayCastCallback {

public:

	int toggle = 0;
	int toggle_keyboard = 0;
	int vec = 0;
	int point = 0;
	int isjumped = 0;
	bool spawned_man = false;
	bool triggered = false;

	static Scene* createScene();
	bool init_background();
	bool init_button();
	bool init_other();
	virtual bool init();

	b2World* world;

	void tick(float dt);
	void ui_update(float dt);

	bool createBox2dWorld();
	void setBox2dWorld();

	bool onTouchBegan(Touch* touch, Event* event);
	void addNewBody(Vec2 point);
	void delete_body(Vec2 point);

	virtual void onKeyPressed(EventKeyboard::KeyCode, Event*);
	virtual void onKeyReleased(EventKeyboard::KeyCode, Event*);

	//rayCast

	virtual float32 ReportFixture(b2Fixture* fix, const b2Vec2& point, const b2Vec2& normal, float32 action);

	b2Vec2 drawP1;
	b2Vec2 drawP2;

	CREATE_FUNC(HelloWorld);


};

