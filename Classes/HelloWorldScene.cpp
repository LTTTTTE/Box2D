#include "HelloWorldScene.h"

std::random_device rd;
std::mt19937_64 _MT19937(rd());

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

	if (this->createBox2dWorld(debug))
		this->setBox2dWorld();
		this->schedule(schedule_selector(HelloWorld::tick));

	return true;
}

bool HelloWorld::createBox2dWorld(bool debug)
{
	world = new b2World(b2Vec2(0, -30));
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);

	if (debug) {

		debugDraw = new GLESDebugDraw(PTM_RATIO);
		world->SetDebugDraw(debugDraw);

		uint32 flags = 0;

		flags += b2Draw::e_shapeBit;
		//flags += b2Draw::e_jointBit;
		//flags += b2Draw::e_aabbBit;
		//flags += b2Draw::e_pairBit;
		//flags += b2Draw::e_centerOfMassBit;
		debugDraw->SetFlags(flags);

	}


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

	return true;
}

void HelloWorld::setBox2dWorld()  ////����ƽ, Ű�׸�ƽ �ٵ�� ����( ����������������)
{

	//����Ʈ�� �ٵ�� �ٵ��� ����� 
	bDrag = false; //�Ʒ��� ���콺 ����Ʈ �ٵ�.
	gbody = this->addNewSprite(Vec2(0, 0), Size(0, 0), b2_staticBody, nullptr, 0);
	
	this->addNewSprite(Vec2(240, 160), Size(32, 32), b2_dynamicBody, "test", 0);


	texture = Director::getInstance()->getTextureCache()->addImage("Globe_48px.png");
	texture_block = Director::getInstance()->getTextureCache()->addImage("blocks.png");

	//static body start////////////////////////////////////////////////////////////////////////////////////////////////////

	b2Body* body_droid;
	b2BodyDef def_droid; //�ٵ����⺻�� .type = static

	auto spr_droid = Sprite::create("Android_48px.png");
	spr_droid->setPosition(200, 50);
	this->addChild(spr_droid, 0, "spr_droid");

	if (!debug) {

		def_droid.userData = spr_droid;
	}
	else {
		spr_droid->setVisible(false);
		def_droid.userData = nullptr;
	}

	def_droid.position.x = ((Sprite*)this->getChildByName("spr_droid"))->getPosition().x / PTM_RATIO;
	def_droid.position.y = ((Sprite*)this->getChildByName("spr_droid"))->getPosition().y / PTM_RATIO;

	body_droid = world->CreateBody(&def_droid);

	b2FixtureDef fixdef_droid;
	b2PolygonShape polygon_droid;
	polygon_droid.SetAsBox(20.0f / PTM_RATIO, 20.0f / PTM_RATIO);

	fixdef_droid.shape = &polygon_droid;
	fixdef_droid.density = 0.0f;
	fixdef_droid.restitution = 0.7f;

	body_droid->CreateFixture(&fixdef_droid);


	auto spr_block = Sprite::createWithTexture(texture_block);
	//auto spr_block = Sprite::create("blocks.png");
	spr_block->setPosition(winsize.width / 3, winsize.height / 3);
	this->addChild(spr_block);

	b2Body* body_block;
	b2BodyDef bodydef_block;

	bodydef_block.type = b2_staticBody;
	bodydef_block.position.Set(winsize.width / 3 / PTM_RATIO, winsize.height / 3 / PTM_RATIO); 
	bodydef_block.userData = spr_block;

	body_block = world->CreateBody(&bodydef_block);


	b2FixtureDef fixdef_block;
	b2PolygonShape shape_block;		// ��������Ʈ�� ũ�⸦ �̾Ƴ��� ���� �Ƚ���ũ�⸦ �����Ҽ��մ�. !�׸��� ��� ũ��� ������ũ���!
	shape_block.SetAsBox((spr_block->getContentSize().width / 2) / PTM_RATIO, (spr_block->getContentSize().height / 2) / PTM_RATIO);

	fixdef_block.density = 1.0f;
	fixdef_block.shape = &shape_block;

	body_block->CreateFixture(&fixdef_block);

	//static body end////////////////////////////////////////////////////////////////////////////////////////////////////

	//kinematic body  start////////////////////////////////////////////////////////////////////////////////////////////////////

	auto spr_minecraft = Sprite::create("Minecraft_48px.png");
	spr_minecraft->setPosition(0, 200);
	this->addChild(spr_minecraft, 5, "spr_minecraft");

	b2Body* body_minecraft;
	b2BodyDef bodydef_minecraft;

	bodydef_minecraft.type = b2_kinematicBody;
	bodydef_minecraft.position.Set(0, 100.0f / PTM_RATIO);
	bodydef_minecraft.linearVelocity.Set(10.0f, 0);
	bodydef_minecraft.userData = spr_minecraft;

	body_minecraft = world->CreateBody(&bodydef_minecraft);

	b2FixtureDef fixdef_minecraft;
	b2PolygonShape shape_minecraft;
	shape_minecraft.SetAsBox((spr_minecraft->getContentSize().width / 2) / PTM_RATIO, (spr_minecraft->getContentSize().height / 2) / PTM_RATIO);

	fixdef_minecraft.density = 1.0f;
	fixdef_minecraft.shape = &shape_minecraft;

	body_minecraft->CreateFixture(&fixdef_minecraft);

	//kinematic body end////////////////////////////////////////////////////////////////////////////////////////////////////
	
}

void HelloWorld::draw(Renderer * renderer, const Mat4 & transform, uint32_t flags)
{
	Scene::draw(renderer, transform, flags);
	custom_cmd.init(_globalZOrder, transform, flags);
	custom_cmd.func = CC_CALLBACK_0(HelloWorld::onDraw, this, transform, flags);
	renderer->addCommand(&custom_cmd);

}

void HelloWorld::onDraw(const Mat4 & transform, uint32_t flags)
{
	Director* director = Director::getInstance();

	CCASSERT(nullptr != director, "Director is null when seting matrix stack");
	director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
	director->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, transform);


	GL::enableVertexAttribs(cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION);
	world->DrawDebugData();
	CHECK_GL_ERROR_DEBUG();

	director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);

}

b2Body * HelloWorld::addNewSprite(Vec2 point, Size size, b2BodyType bodytype, const char * spriteName, int type)
{

	b2Body* body;
	b2BodyDef bodydef;
	bodydef.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);

	if (spriteName) {   //�����𸣰ٴµ� �׳� ����� ��������Ʈ������ �ٵ������� �ֱ�.
		if (strcmp(spriteName, "test") == 0) {

			int idx = (CCRANDOM_0_1() > 0.5 ? 0 : 1);
			int idy = (CCRANDOM_0_1() > 0.5 ? 0 : 1);
			auto spr = Sprite::create("blocks.png");
			spr->setPosition(point);

			this->addChild(spr);
			bodydef.userData = spr;
		}
		else {
			auto spr = Sprite::create(spriteName);
			spr->setPosition(point);
			this->addChild(spr);

			bodydef.userData = spr;
		}
	}
		body = world->CreateBody(&bodydef);

		b2FixtureDef fixdef;
		b2PolygonShape shape;
		b2CircleShape shape_cir;

		if (type == 0) {
			shape.SetAsBox(size.width / 2 / PTM_RATIO, size.height / 2 / PTM_RATIO);
			fixdef.shape = &shape;
		}
		else {
			shape_cir.m_radius = (size.width / 2) / PTM_RATIO;
			fixdef.shape = &shape_cir;
		}

		fixdef.density = 1.0f;
		fixdef.restitution = 0.7f;

		body->CreateFixture(&fixdef);

		CCLOG("addNewSprite");

		return body;
}

b2Body * HelloWorld::getBodyAtTab(Vec2 point)//��ġ�Ѱ��̶� ���ٵ�� ���ؼ� üũ
{
	b2Fixture* fix; 

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) { //���ٵ� ����!~
		
		if (b->GetUserData() != nullptr) { //���ٵ𵹱�

			if (b->GetType() == b2_staticBody)continue; //����ƽ�ٵ���� �糦
		
			fix = b->GetFixtureList(); //�׹ٵ��� �Ƚ��� �̾Ƴ�
			CCLOG("getBodyAtTab");
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/**//**//**//**//**//**//**//**//**////�Ʒ����ǹ��� ��������.
			if (fix->TestPoint(b2Vec2(point.x / PTM_RATIO, point.y / PTM_RATIO))) // TestPoint << �浹üũ�ΰŰ���..
				return b; //�������� ��ȯ
		}
	}
	return nullptr;
}

void HelloWorld::onEnter()
{
	Scene::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
//	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan2, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

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

		//
		if (b->GetType() == b2_kinematicBody) {

			MT19937 num(1, 5); //��������
			MT19937 num2(-5, 5);

			if (b->GetPosition().x*PTM_RATIO > winsize.width ) //������ȯ
				b->SetLinearVelocity(b2Vec2(num(_MT19937)*-5.0f, num2(_MT19937)*5.0f));

			if (b->GetPosition().y*PTM_RATIO > winsize.height)
				b->SetLinearVelocity(b2Vec2(num2(_MT19937)*5.0f, num(_MT19937)*-5.0f));

			if (b->GetPosition().x*PTM_RATIO < 0 )
				b->SetLinearVelocity(b2Vec2(num(_MT19937)*5.0f, num2(_MT19937)*5.0f));

			if (b->GetPosition().y*PTM_RATIO < 0)
				b->SetLinearVelocity(b2Vec2(num2(_MT19937)*5.0f, num(_MT19937)*5.0f));
		}
	}


}

bool HelloWorld::onTouchBegan(Touch * touch, Event * event)
{
	addNewSpriteAtPosition2(touch->getLocation());

	return true;
}

bool HelloWorld::onTouchBegan2(Touch * touch, Event * event)
{
	Vec2 touch_point = touch->getLocation();

	if (b2Body* b = this->getBodyAtTab(touch_point)) {
		
//		//����Ʈ�� �ٵ�� �ٵ��� ����� 
//		bDrag = false; //�Ʒ��� ���콺 ����Ʈ �ٵ�.
//		gbody = this->addNewSprite(Vec2(0, 0), Size(0, 0), b2_staticBody, nullptr, 0);

		dragbody = b;
		bDrag = true;

		b2MouseJointDef mouseJdef;
		mouseJdef.bodyA = gbody;
		mouseJdef.bodyB = dragbody;
		mouseJdef.target.Set(dragbody->GetPosition().x, dragbody->GetPosition().y);
		mouseJdef.maxForce = 300.0*dragbody->GetMass();

		mouseJ = (b2MouseJoint*)world->CreateJoint(&mouseJdef);
		CCLOG("onTouchBagan2");
	}

	return true;
}

void HelloWorld::onTouchMoved(Touch * touch, Event * event)
{
	if (bDrag) { //����
		mouseJ->SetTarget(b2Vec2(touch->getLocation().x / PTM_RATIO, touch->getLocation().y / PTM_RATIO));
	}
}

void HelloWorld::onTouchEnded(Touch * touch, Event * event)
{
	if (bDrag) {
		//����Ʈ����
		world->DestroyJoint(mouseJ);
		mouseJ = nullptr;

		dragbody->SetAwake(true);

	}
	bDrag = false;
}

void HelloWorld::addNewSpriteAtPosition(Vec2 location)
{
	b2Body* body_spr;
	b2BodyDef body_spr_Def;

	//debug//
	if (!debug) {
		auto spr = Sprite::createWithTexture(texture);
		spr->setPosition(location.x, location.y);
		this->addChild(spr);
		body_spr_Def.userData = spr;
	}
	else body_spr_Def.userData = nullptr;
	////

	body_spr_Def.type = b2_dynamicBody;
	body_spr_Def.position.Set(location.x / PTM_RATIO, location.y / PTM_RATIO);
	

	body_spr = world->CreateBody(&body_spr_Def);  //�ٵ𸸵�� �ٵ�def�����Ϸ�

	b2FixtureDef fix_spr_def;	 //�Ƚ��ļ�����ü ����
	b2CircleShape cir;
	cir.m_radius = 0.65f;

	//�Ƚ��ļ�������
	fix_spr_def.shape = &cir;  
	fix_spr_def.density = 1.0f;
	fix_spr_def.friction = 0.2f;
	fix_spr_def.restitution = 0.7f;

	body_spr->CreateFixture(&fix_spr_def);

}

void HelloWorld::addNewSpriteAtPosition2(Vec2 location)
{
	MT19937 num(1, 3);

	auto spr_google = Sprite::create("Google_Play_48px.png");
	spr_google->setPosition(location.x, location.y);
	spr_google->setAnchorPoint(Point(0.5, 0.1));
	this->addChild(spr_google,1,"spr_google");

	b2Body* body_google;
	b2BodyDef bodydef_google;

	bodydef_google.type = b2_dynamicBody;
	bodydef_google.position.Set(location.x / PTM_RATIO, location.y / PTM_RATIO);
	bodydef_google.userData = spr_google;

	body_google = world->CreateBody(&bodydef_google);

	b2FixtureDef fixdef_google;
	b2PolygonShape shape_google;

	b2Vec2 tri[3];
	tri[0] = b2Vec2((spr_google->getContentSize().width / 2)*0.9 / PTM_RATIO * -1, 0.0);
	tri[1] = b2Vec2((spr_google->getContentSize().width / 2)*0.9 / PTM_RATIO, 0.0);
	tri[2] = b2Vec2(0.0, (spr_google->getContentSize().height)*0.9 / PTM_RATIO);
	shape_google.Set(tri, 3);

	fixdef_google.shape = &shape_google;
	fixdef_google.density = 1.0f;
	fixdef_google.restitution = 0.7f;

	body_google->CreateFixture(&fixdef_google);

}
