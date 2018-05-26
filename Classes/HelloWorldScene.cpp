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

	// ����ȿ� �ٵ� �ְ� �ٵ�� �Ƚ��ĸ� ������.??
	// �ٵ� def(�Ӽ�)�� ������??  �Ƚ��ĵ� def(�Ӽ�)�� ������??

	b2BodyDef worldBody_Def;
	b2Body* worldBody;
	worldBody_Def.position.Set(0, 0); //�ٵ�Ӽ�
	worldBody = world->CreateBody(&worldBody_Def);

	b2EdgeShape worldEdge;  //�� ��ü ??
	b2FixtureDef boxShape_Def;  // �Ƚ��ļӼ� ���� 
	boxShape_Def.shape = &worldEdge;  //�ٵ�->�Ƚ�ó�ֱ�޼ҵ�(�Ƚ��ļ���������ü)  // �Ƚ��ļ���������ü.shape �� ��縸���

	//�Ƚ��Ĵ� �ٵ��� ���배���Ŵϱ� ��(EdgeShape) ���� �����?

	//�Ʒ��� [�ٵ𸦱����� �Ƚ���]�� ������ ���� ������ �ٵ��� �Ƚ��ķθ�����ڵ�

	worldEdge.Set(b2Vec2(0, 0), b2Vec2(winsize.width / PTM_RATIO, 0));  //(0,0)���� (���α���,0) ��ǥ�� ������ b2EdgeShapeŬ���� ��ü(��??)
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

	b2BodyDef def_droid; //�ٵ����⺻�� .type = static
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
	world->Step(dt, 8, 3); // (ƽŸ��0.016 , �浹���ݺ�Ÿ��, �ٵ���ġ���ݺ�Ÿ��)

	//�Ʒ��� �ٵ� ���� ��������Ʈ�� �����ϴ� ������Ʈ�� , ������� �ٵ���� getbodyList�� �� �����ü��մ�(�ڵ�VECTOR����)
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {

		if (b->GetUserData() != nullptr) {

			auto spr = (Sprite*)b->GetUserData();
			spr->setPosition(Vec2((b->GetPosition().x)*PTM_RATIO, (b->GetPosition().y)*PTM_RATIO)); // �ٵ���ǥ�� �� �ٵ𰡰��� ��������Ʈ�� ��ǥ�����Ѵ�
			spr->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));//��������Ʈ�� ������ �ٵ𰢵����°� ������.
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

	body_spr = world->CreateBody(&body_spr_Def);  //�ٵ𸸵�� �ٵ�def�����Ϸ�

	b2FixtureDef fix_spr_def;	 //�Ƚ��ļ�����ü ����
	b2CircleShape cir;
	cir.m_radius = 0.65;

	//�Ƚ��ļ�������
	fix_spr_def.shape = &cir;  
	fix_spr_def.density = 1.0f;
	fix_spr_def.friction = 0.2f;
	fix_spr_def.restitution = 0.7f;

	body_spr->CreateFixture(&fix_spr_def);

}
