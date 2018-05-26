#include "HelloWorldScene.h"

Scene* HelloWorld::createScene()
{
	Scene* scene = HelloWorld::create();

	return scene;
}


bool HelloWorld::init()
{
	
	if (!Scene::init())
	{
		return false;
	}
	
	texture = Director::getInstance()->getTextureCache()->addImage("Globe_48px.png");
	
	world = new b2World(b2Vec2(0, -30));
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);

	// 윌드안에 바디가 있고 바디는 픽스쳐를 가진다.??
	// 바디도 def(속성)를 가지고??  픽스쳐도 def(속성)를 가진다??

	b2BodyDef worldBody_Def;
	b2Body* worldBody;
	worldBody_Def.position.Set(0, 0); //바디속성
	worldBody = world->CreateBody(&worldBody_Def);

	b2EdgeShape worldEdge;  //선 객체 ??
	b2FixtureDef boxShape_Def;  // 픽스쳐속성 설정 
	boxShape_Def.shape = &worldEdge;  //바디->픽스처넣기메소드(픽스쳐설정가진객체)  // 픽스쳐설정가진객체.shape 로 모양만들기

	//픽스쳐는 바디의 뼈대같은거니깐 선(EdgeShape) 으로 만든다?

	//아래는 [바디를구성할 픽스쳐]를 구성할 선을 설정후 바디의 픽스쳐로만드는코드

	worldEdge.Set(b2Vec2(0, 0), b2Vec2(winsize.width / PTM_RATIO, 0));  //(0,0)에서 (가로길이,0) 좌표를 가지는 b2EdgeShape클래스 객체(선??)
	worldBody->CreateFixture(&boxShape_Def);

	worldEdge.Set(b2Vec2(0, winsize.height / PTM_RATIO), b2Vec2(winsize.width / PTM_RATIO, winsize.height / PTM_RATIO));
	worldBody->CreateFixture(&boxShape_Def);

	worldEdge.Set(b2Vec2(0, 0), b2Vec2(0, winsize.height / PTM_RATIO));  
	worldBody->CreateFixture(&boxShape_Def);

	worldEdge.Set(b2Vec2(winsize.width / PTM_RATIO, 0), b2Vec2(winsize.width / PTM_RATIO, winsize.height / PTM_RATIO));
	worldBody->CreateFixture(&boxShape_Def);

////////
	auto spr_droid = Sprite::create("Android_48px.png");
	spr_droid->setPosition(50, 50);
	this->addChild(spr_droid, 0, "spr_droid");

	b2Body* body_droid;

	b2BodyDef def_droid; //바디설정기본값 .type = static
	def_droid.position.x = ((Sprite*)this->getChildByName("spr_droid"))->getPosition().x / PTM_RATIO;
	def_droid.position.y = ((Sprite*)this->getChildByName("spr_droid"))->getPosition().y / PTM_RATIO;
	def_droid.userData = spr_droid;

	body_droid = world->CreateBody(&def_droid);

	b2FixtureDef fixdef_droid;
	b2PolygonShape polygon_droid;
	polygon_droid.SetAsBox(31 / PTM_RATIO, 31 / PTM_RATIO);

	fixdef_droid.shape = &polygon_droid;
	fixdef_droid.density = 0.0f;
	fixdef_droid.restitution = 0.7f;

	body_droid->CreateFixture(&fixdef_droid);
////////

	this->schedule(schedule_selector(HelloWorld::tick));

	return true;
}

void HelloWorld::onEnter()
{
	Scene::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

}

void HelloWorld::tick(float dt)
{
	world->Step(dt, 8, 3); // (틱타임0.016 , 충돌계산반복타임, 바디위치계산반복타임)

	//아래는 바디에 붙은 스프라이트를 제어하는 업데이트문 , 월드안의 바디들을 getbodyList로 다 가져올수잇다(자동VECTOR저장)
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {

		if (b->GetUserData() != nullptr) {

			auto spr = (Sprite*)b->GetUserData();
			spr->setPosition(Vec2((b->GetPosition().x)*PTM_RATIO, (b->GetPosition().y)*PTM_RATIO)); // 바디좌표에 그 바디가가진 스프라이트를 좌표설정한다
			spr->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));//스프라이트의 각도도 바디각도에맞게 돌린다.
		}

	}
}

bool HelloWorld::onTouchBegan(Touch * touch, Event * event)
{
	addNewSpriteAtPosition(touch->getLocation());

	return false;
}

void HelloWorld::addNewSpriteAtPosition(Vec2 location)
{
	auto spr = Sprite::createWithTexture(texture);
	spr->setPosition(location.x, location.y);
	this->addChild(spr);
	
	b2Body* body_spr;
	b2BodyDef body_spr_Def;
	
	body_spr_Def.type = b2_dynamicBody;
	body_spr_Def.position.Set(location.x / PTM_RATIO, location.y / PTM_RATIO);
	body_spr_Def.userData = spr;

	body_spr = world->CreateBody(&body_spr_Def);  //바디만들고 바디def설정완료

	b2FixtureDef fix_spr_def;	 //픽스쳐설정객체 생성
	b2CircleShape cir;
	cir.m_radius = 0.65;

	//픽스쳐설정설정
	fix_spr_def.shape = &cir;  
	fix_spr_def.density = 1.0f;
	fix_spr_def.friction = 0.2f;
	fix_spr_def.restitution = 0.7f;

	body_spr->CreateFixture(&fix_spr_def);

}
