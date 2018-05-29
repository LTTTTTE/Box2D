#include "cocos2d.h"
#include "Box2D\Box2D.h"

#pragma execution_character_set("utf-8")

USING_NS_CC;

#define winsize Director::getInstance()->getWinSize()
#define MT19937 std::uniform_int_distribution<__int64>
#define PTM_RATIO 76.8

class HelloWorld : public Scene {

public:

	int toggle = 0;
	int toggle_keyboard = 0;
	int vec = 0;
	int point = 0;
	bool spawned_man = false;

	static Scene* createScene();
	bool init_background();
	bool init_button();
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

	CREATE_FUNC(HelloWorld);


};

