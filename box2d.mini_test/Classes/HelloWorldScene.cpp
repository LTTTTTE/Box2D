#include "HelloWorldScene.h"

Scene* HelloWorld::createScene()
{
	Scene* scene = HelloWorld::create();

	return scene;
}

bool HelloWorld::init_background() {

	auto node = ParallaxNode::create();
	this->addChild(node);

	auto act_1 = MoveBy::create(120, Point(-2800, 0));
	auto act_2 = Place::create(Point(0, 0));
	auto act_3 = Sequence::create(act_1, act_2, NULL);
	auto act_4 = RepeatForever::create(act_3);
	node->runAction(act_4);

	auto spr_1 = Sprite::create("background_sky.png");
	spr_1->setAnchorPoint(Point(0, 0.95));
	spr_1->setScale(0.5);
	node->addChild(spr_1, -1, Point(1, 0), Point(0, winsize.height));

	auto spr_1_1 = Sprite::create("background_sky_1.png");
	spr_1_1->setAnchorPoint(Point(0, 0.95));
	spr_1_1->setScale(0.5);
	node->addChild(spr_1_1, -1, Point(1, 0), Point(1400, winsize.height));

	auto spr_1_2 = Sprite::create("background_sky.png");
	spr_1_2->setAnchorPoint(Point(0, 0.95));
	spr_1_2->setScale(0.5);
	node->addChild(spr_1_2, -1, Point(1, 0), Point(2800, winsize.height));


	return true;
}

bool HelloWorld::init_button() {

	auto spr_ui_button = Sprite::create("menu.png");
	spr_ui_button->setScale(0.2);
	spr_ui_button->setPosition(winsize.width - (spr_ui_button->getContentSize().width) / 10, winsize.height - (spr_ui_button->getContentSize().height) / 10);
	this->addChild(spr_ui_button, 2, "menu");
	
	auto spr_ui_button_1 = Sprite::create("grassBlock.png");
	spr_ui_button_1->setPosition(spr_ui_button->getPosition().x - (spr_ui_button->getContentSize().width) / 22, spr_ui_button->getPosition().y + (spr_ui_button->getContentSize().height) / 22);
	this->addChild(spr_ui_button_1, 3,"spr_ui_button_1");

	auto spr_ui_button_2 = Sprite::create("grassBlock_dead.png");
	spr_ui_button_2->setPosition(spr_ui_button->getPosition().x + (spr_ui_button->getContentSize().width) / 22, spr_ui_button->getPosition().y + (spr_ui_button->getContentSize().height) / 22);
	this->addChild(spr_ui_button_2, 3, "spr_ui_button_2");

	auto spr_ui_button_1_1 = Sprite::create("grassBlock_1.png");
	spr_ui_button_1_1->setPosition(spr_ui_button->getPosition().x - (spr_ui_button->getContentSize().width) / 22, spr_ui_button->getPosition().y + (spr_ui_button->getContentSize().height) / 22);
	this->addChild(spr_ui_button_1_1, 3, "spr_ui_button_1_1");
	spr_ui_button_1_1->setVisible(false);

	auto spr_ui_button_2_1 = Sprite::create("grassBlock_dead_1.png");
	spr_ui_button_2_1->setPosition(spr_ui_button->getPosition().x + (spr_ui_button->getContentSize().width) / 22, spr_ui_button->getPosition().y + (spr_ui_button->getContentSize().height) / 22);
	this->addChild(spr_ui_button_2_1, 3, "spr_ui_button_2_1");
	spr_ui_button_2_1->setVisible(false);

	auto spr_ui_trash = Sprite::create("Trash_Can_96px.png");
	spr_ui_trash->setPosition(spr_ui_button->getPosition().x + (spr_ui_button->getContentSize().width) / 22, spr_ui_button->getPosition().y - (spr_ui_button->getContentSize().height) / 22);
	this->addChild(spr_ui_trash,3,"trashcan");

	auto spr_ui_trash_1 = Sprite::create("Trash_Can_96px_1.png");
	spr_ui_trash_1->setPosition(spr_ui_button->getPosition().x + (spr_ui_button->getContentSize().width) / 22, spr_ui_button->getPosition().y - (spr_ui_button->getContentSize().height) / 22);
	this->addChild(spr_ui_trash_1, 3,"trashcan_1");
	spr_ui_trash_1->setVisible(false);

	auto spr_ui_man = Sprite::create("GingerMan_96px.png");
	spr_ui_man->setPosition(spr_ui_button->getPosition().x - (spr_ui_button->getContentSize().width) / 22, spr_ui_button->getPosition().y - (spr_ui_button->getContentSize().height) / 22);
	this->addChild(spr_ui_man, 3, "man");

	auto spr_ui_man_1 = Sprite::create("GingerMan_96px_1.png");
	spr_ui_man_1->setPosition(spr_ui_button->getPosition().x - (spr_ui_button->getContentSize().width) / 22, spr_ui_button->getPosition().y - (spr_ui_button->getContentSize().height) / 22);
	this->addChild(spr_ui_man_1, 3, "man_1");
	spr_ui_man_1->setVisible(false);

	return true;
}

bool HelloWorld::init()
{

	if (!Scene::init())
	{
		return false;
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto listener_keyboard = EventListenerKeyboard::create();
	listener_keyboard->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	listener_keyboard->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_keyboard, this);
	


	this->init_background();
	this->init_button();
	this->createBox2dWorld();
	this->setBox2dWorld();
	this->schedule(schedule_selector(HelloWorld::tick));
	this->schedule(schedule_selector(HelloWorld::ui_update));

	return true;
}

void HelloWorld::tick(float dt) {

	world->Step(dt, 8, 3);

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {

		if (b->GetUserData() != nullptr) {

			auto spr = (Sprite*)b->GetUserData();
			auto man = (Sprite*)this->getChildByName("man_spawned");

			if (spr == man) {

				if (toggle_keyboard == 2) {
					CCLOG("keyboard_1");
					b->SetTransform(b2Vec2(b->GetPosition().x + vec/PTM_RATIO, b->GetPosition().y), 0);
				}
				if (toggle_keyboard == 1) {
					CCLOG("keyboard_2");
					b->SetTransform(b2Vec2(b->GetPosition().x + vec/PTM_RATIO, b->GetPosition().y), 0);
				}
			}

			spr->setPosition(Vec2((b->GetPosition().x)*(PTM_RATIO), (b->GetPosition().y)*(PTM_RATIO)));
			spr->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}

}

void HelloWorld::ui_update(float dt) {
	
	if (toggle == 0) { //아무것도 클릭 안한상태

		((Sprite*)this->getChildByName("spr_ui_button_1"))->setVisible(true);
		((Sprite*)this->getChildByName("spr_ui_button_1_1"))->setVisible(false);
		((Sprite*)this->getChildByName("spr_ui_button_2"))->setVisible(true);
		((Sprite*)this->getChildByName("spr_ui_button_2_1"))->setVisible(false);
		((Sprite*)this->getChildByName("trashcan"))->setVisible(true);
		((Sprite*)this->getChildByName("trashcan_1"))->setVisible(false);
		((Sprite*)this->getChildByName("man"))->setVisible(true);
		((Sprite*)this->getChildByName("man_1"))->setVisible(false);
	}

	if (toggle == 1) { //첫번째꺼 클릭상태

		((Sprite*)this->getChildByName("spr_ui_button_1"))->setVisible(false);
		((Sprite*)this->getChildByName("spr_ui_button_1_1"))->setVisible(true);
		((Sprite*)this->getChildByName("spr_ui_button_2"))->setVisible(true);
		((Sprite*)this->getChildByName("spr_ui_button_2_1"))->setVisible(false);
		((Sprite*)this->getChildByName("trashcan"))->setVisible(true);
		((Sprite*)this->getChildByName("trashcan_1"))->setVisible(false);
		((Sprite*)this->getChildByName("man"))->setVisible(true);
		((Sprite*)this->getChildByName("man_1"))->setVisible(false);
	}

	if (toggle == 2) { //두번째꺼 클릭상태

		((Sprite*)this->getChildByName("spr_ui_button_1"))->setVisible(true);
		((Sprite*)this->getChildByName("spr_ui_button_1_1"))->setVisible(false);
		((Sprite*)this->getChildByName("spr_ui_button_2"))->setVisible(false);
		((Sprite*)this->getChildByName("spr_ui_button_2_1"))->setVisible(true);
		((Sprite*)this->getChildByName("trashcan"))->setVisible(true);
		((Sprite*)this->getChildByName("trashcan_1"))->setVisible(false);
		((Sprite*)this->getChildByName("man"))->setVisible(true);
		((Sprite*)this->getChildByName("man_1"))->setVisible(false);
	}

	if (toggle == 3) { //인간 클릭상태

		((Sprite*)this->getChildByName("spr_ui_button_1"))->setVisible(true);
		((Sprite*)this->getChildByName("spr_ui_button_1_1"))->setVisible(false);
		((Sprite*)this->getChildByName("spr_ui_button_2"))->setVisible(true);
		((Sprite*)this->getChildByName("spr_ui_button_2_1"))->setVisible(false);
		((Sprite*)this->getChildByName("trashcan"))->setVisible(true);
		((Sprite*)this->getChildByName("trashcan_1"))->setVisible(false);
		((Sprite*)this->getChildByName("man"))->setVisible(false);
		((Sprite*)this->getChildByName("man_1"))->setVisible(true);
	}

	if (toggle == 4) { //쓰래기통 클릭상태

		((Sprite*)this->getChildByName("spr_ui_button_1"))->setVisible(true);
		((Sprite*)this->getChildByName("spr_ui_button_1_1"))->setVisible(false);
		((Sprite*)this->getChildByName("spr_ui_button_2"))->setVisible(true);
		((Sprite*)this->getChildByName("spr_ui_button_2_1"))->setVisible(false);
		((Sprite*)this->getChildByName("trashcan"))->setVisible(false);
		((Sprite*)this->getChildByName("trashcan_1"))->setVisible(true);
		((Sprite*)this->getChildByName("man"))->setVisible(true);
		((Sprite*)this->getChildByName("man_1"))->setVisible(false);
	}

}

bool HelloWorld::createBox2dWorld() {

	world = new b2World(b2Vec2(0, -30));
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);


	return true;
}

void HelloWorld::setBox2dWorld() {

	auto spr_block_1 = Sprite::create("grassBlock_dead.png");
	spr_block_1->setPosition(100, 650);
	this->addChild(spr_block_1, 2);

	b2Body* body_block_static_1;
	b2BodyDef bodydef_block_static_1;

	bodydef_block_static_1.type = b2_staticBody;
	bodydef_block_static_1.position.Set(spr_block_1->getPosition().x / PTM_RATIO, spr_block_1->getPosition().y / PTM_RATIO);
	bodydef_block_static_1.userData = spr_block_1;

	body_block_static_1 = world->CreateBody(&bodydef_block_static_1);

	b2FixtureDef fixdef_block_static_1; 
	b2PolygonShape shape_block_static_1;
	shape_block_static_1.SetAsBox(spr_block_1->getContentSize().width / 2.0 / PTM_RATIO, spr_block_1->getContentSize().height / 2.0 / PTM_RATIO);

	fixdef_block_static_1.shape = &shape_block_static_1;
	fixdef_block_static_1.density = 1.0f;
	
	body_block_static_1->CreateFixture(&fixdef_block_static_1);

}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event) {

	Vec2 point = Vec2(touch->getLocation().x, touch->getLocation().y);

	CCLOG("TOUCH %d", toggle);

	{ // toggle ALGO
		if (toggle == 0) {

			if ((Sprite*)this->getChildByName("spr_ui_button_1")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 1;
				return true;
			}
			if ((Sprite*)this->getChildByName("spr_ui_button_2")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 2;
				return true;
			}
			if ((Sprite*)this->getChildByName("man")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 3;
				return true;
			}
			if ((Sprite*)this->getChildByName("trashcan")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 4;
				return true;
			}
			
		}

		if (toggle == 1) {

			if ((Sprite*)this->getChildByName("spr_ui_button_1_1")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 0;
				return true;
			}
			if ((Sprite*)this->getChildByName("spr_ui_button_2")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 2;
				return true;
			}
			if ((Sprite*)this->getChildByName("man")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 3;
				return true;
			}
			if ((Sprite*)this->getChildByName("trashcan")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 4;
				return true;
			}
		}

		if (toggle == 2) {

			if ((Sprite*)this->getChildByName("spr_ui_button_1")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 1;
				return true;
			}
			if ((Sprite*)this->getChildByName("spr_ui_button_2_1")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 0;
				return true;
			}
			if ((Sprite*)this->getChildByName("man")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 3;
				return true;
			}
			if ((Sprite*)this->getChildByName("trashcan")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 4;
				return true;
			}
			
		}

		if (toggle == 3) {

			if ((Sprite*)this->getChildByName("spr_ui_button_1")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 1;
				return true;
			}
			if ((Sprite*)this->getChildByName("spr_ui_button_2_1")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 2;
				return true;
			}
			if ((Sprite*)this->getChildByName("man")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 0;
				return true;
			}
			if ((Sprite*)this->getChildByName("trashcan")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 4;
				return true;
			}

		}

		if (toggle == 4) {

			if ((Sprite*)this->getChildByName("trashcan_1")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 0;
				return true;
			}
			if ((Sprite*)this->getChildByName("spr_ui_button_1")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 1;
				return true;
			}
			if ((Sprite*)this->getChildByName("spr_ui_button_2")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 2;
				return true;
			}
			if ((Sprite*)this->getChildByName("man")->getBoundingBox().containsPoint(touch->getLocation())) {
				toggle = 3;
				return true;
			}
		}

	}

	if ((Sprite*)this->getChildByName("menu")->getBoundingBox().containsPoint(touch->getLocation()))return true; //박스ui안 스폰금지
	this->addNewBody(point);
	this->delete_body(point);
	return true;
}

void HelloWorld::addNewBody(Vec2 point) {

	if (toggle == 1) { // dynamic 조건

		auto spr_block_dymic = Sprite::create("grassBlock.png");
		spr_block_dymic->setPosition(point);
		this->addChild(spr_block_dymic, 2);


		b2Body* body_dymic_block;
		b2BodyDef bodydef_dymic_block;

		bodydef_dymic_block.userData = spr_block_dymic;
		bodydef_dymic_block.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);
		bodydef_dymic_block.type = b2_dynamicBody;

		body_dymic_block = world->CreateBody(&bodydef_dymic_block);

		b2FixtureDef fixdef_dymic_block;
		b2PolygonShape shape_dymic_block;
		shape_dymic_block.SetAsBox(((spr_block_dymic->getContentSize().width) / 2.0) / PTM_RATIO, ((spr_block_dymic->getContentSize().height) / 2.0) / PTM_RATIO);

		fixdef_dymic_block.shape = &shape_dymic_block;
		fixdef_dymic_block.density = 1.0f;

		body_dymic_block->CreateFixture(&fixdef_dymic_block);
		CCLOG("dynamic body : DONE");

	}

	if (toggle == 2) {// static조건


		auto spr_block_static = Sprite::create("grassBlock_dead.png");
		spr_block_static->setPosition(point);
		this->addChild(spr_block_static, 2);


		b2Body* body_static_block;
		b2BodyDef bodydef_static_block;

		bodydef_static_block.userData = spr_block_static;
		bodydef_static_block.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);
		bodydef_static_block.type = b2_staticBody;

		body_static_block = world->CreateBody(&bodydef_static_block);

		b2FixtureDef fixdef_static_block;
		b2PolygonShape shape_static_block;
		shape_static_block.SetAsBox(((spr_block_static->getContentSize().width) / 2.0) / PTM_RATIO, ((spr_block_static->getContentSize().height) / 2.0) / PTM_RATIO);

		fixdef_static_block.shape = &shape_static_block;
		fixdef_static_block.density = 1.0f;

		body_static_block->CreateFixture(&fixdef_static_block);
		CCLOG("static body : DONE");
	}

	if (toggle == 3) {// 인간조건


		auto spr_dymic_man = Sprite::create("GingerMan_96px.png");
		spr_dymic_man->setPosition(point);
		this->addChild(spr_dymic_man, 2, "man_spawned");


		b2Body* body_dymic_man;
		b2BodyDef bodydef_dymic_man;

		bodydef_dymic_man.userData = spr_dymic_man;
		bodydef_dymic_man.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);
		bodydef_dymic_man.type = b2_dynamicBody;

		body_dymic_man = world->CreateBody(&bodydef_dymic_man);

		b2FixtureDef fixdef_dymic_man;
		b2PolygonShape shape_dymic_man;
		shape_dymic_man.SetAsBox(((spr_dymic_man->getContentSize().width) / 3) / PTM_RATIO, ((spr_dymic_man->getContentSize().height) / 3) / PTM_RATIO);

		fixdef_dymic_man.shape = &shape_dymic_man;
		fixdef_dymic_man.density = 1.0f;

		body_dymic_man->CreateFixture(&fixdef_dymic_man);
		CCLOG("dynamic man : DONE");
	}

}

void HelloWorld::delete_body(Vec2 point) {


	if(toggle==4)
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {

		if (b->GetUserData() != nullptr) {

			auto spr = (Sprite*)b->GetUserData();

			if (spr->getBoundingBox().containsPoint(point)) {

				this->removeChild(spr, true);
				world->DestroyBody(b);

				break;
			}

			
		}
	}

}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode e, Event *) {

	CCLOG("keyPressed");

	Sprite* spr = (Sprite*)this->getChildByName("man");

	if (e == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {

		++toggle_keyboard; vec = -3;
	}
	if (e == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {

		++toggle_keyboard; vec = 3;
	}
	/*if (e == EventKeyboard::KeyCode::KEY_SPACE) {
		Action* act_bounce = JumpBy::create(0.4, Point(0, 0), 50, 1);
		spr->runAction(act_bounce);
	}*/

}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode e, Event *) {

	Sprite* spr = (Sprite*)this->getChildByName("man");

	if (e == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
		--toggle_keyboard; vec = 3;
	}
	if (e == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
		--toggle_keyboard; vec = -3;
	}

}