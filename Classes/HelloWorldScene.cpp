#include "HelloWorldScene.h"

std::random_device rd;
std::mt19937_64 _MT19937(rd());

Scene* HelloWorld::createScene()
{
	Scene* scene = HelloWorld::create();

	return scene;
}
//

bool HelloWorld::init()
{

	if (!Scene::init())
	{
		return false;
	}

	texture = Director::getInstance()->getTextureCache()->addImage("Globe_48px.png");
	texture_block = Director::getInstance()->getTextureCache()->addImage("blocks.png");

	if (this->createBox2dWorld(debug)) {

		srand((int)time(nullptr));
		this->setBox2dWorld();
		this->schedule(schedule_selector(HelloWorld::tick));

	}
	return true;
}
//

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

	return true;
}
//

void HelloWorld::setBox2dWorld()  ////스테틱, 키네마틱 바디들 생성( 월드지형생성느낌)
{

	//조인트는 바디와 바디의 연결고리 
	bDrag = false; //아래는 마우스 조인트 바디.
	gbody = this->addNewSprite(Vec2(0, 0), Size(0, 0), b2_staticBody, nullptr, 0);

	this->addNewSprite(Vec2(240, 160), Size(32, 32), b2_dynamicBody, "test", 0);

	//static body start////////////////////////////////////////////////////////////////////////////////////////////////////

	b2Body* body_droid;
	b2BodyDef def_droid; //바디설정기본값 .type = static

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
	b2PolygonShape shape_block;		// 스프라이트의 크기를 뽑아내어 쉽게 픽스쳐크기를 설정할수잇다. !그리고 모든 크기는 반지름크기로!
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
//


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

	bodydef.type = bodytype;  //<<이거하나안적어서 ㅅㅂ
	bodydef.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);

	if (spriteName) {   //뭔진모르겟는데 그냥 결론은 스프라이트생성후 바디정보에 넣기.
		if (strcmp(spriteName, "test") == 0) {

			int idx = (CCRANDOM_0_1() > 0.5 ? 0 : 1);
			int idy = (CCRANDOM_0_1() > 0.5 ? 0 : 1);
			auto spr = Sprite::createWithTexture(texture_block, Rect(32 * idx, 32 * idy, 32, 32));
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
//

b2Body * HelloWorld::getBodyAtTab(Vec2 point)//터치한곳이랑 모든바디랑 비교해서 체크
{
	b2Fixture* fix;

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) { //모든바디 돌기!~

		if (b->GetUserData() != nullptr) { //모든바디돌기

			if (b->GetType() == b2_staticBody)continue; //스테틱바디들은 재낌

			fix = b->GetFixtureList(); //그바디의 픽스쳐 뽑아냄
			CCLOG("getBodyAtTab");
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			/**//**//**//**//**//**//**//**//**////아래조건문을 돌지않음.
			if (fix->TestPoint(b2Vec2(point.x / PTM_RATIO, point.y / PTM_RATIO))) // TestPoint << 충돌체크인거같음..
				return b; //요고맞으면 반환
		}
	}
	return nullptr;
}
//

void HelloWorld::onEnter()
{
	Scene::onEnter();

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

}
//

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

		/*
		if (b->GetType() == b2_kinematicBody) {

		MT19937 num(1, 5); //난수생성
		MT19937 num2(-5, 5);

		if (b->GetPosition().x*PTM_RATIO > winsize.width ) //방항전환
		b->SetLinearVelocity(b2Vec2(num(_MT19937)*-5.0f, num2(_MT19937)*5.0f));

		if (b->GetPosition().y*PTM_RATIO > winsize.height)
		b->SetLinearVelocity(b2Vec2(num2(_MT19937)*5.0f, num(_MT19937)*-5.0f));

		if (b->GetPosition().x*PTM_RATIO < 0 )
		b->SetLinearVelocity(b2Vec2(num(_MT19937)*5.0f, num2(_MT19937)*5.0f));

		if (b->GetPosition().y*PTM_RATIO < 0)
		b->SetLinearVelocity(b2Vec2(num2(_MT19937)*5.0f, num(_MT19937)*5.0f));
		}*/
	}


}
//

bool HelloWorld::onTouchBegan(Touch * touch, Event * event)
{
	Vec2 touch_point = touch->getLocation();

	if (b2Body* b = this->getBodyAtTab(touch_point)) {

		//		//조인트는 바디와 바디의 연결고리 
		//		bDrag = false; //아래는 마우스 조인트 바디.
		//		gbody = this->addNewSprite(Vec2(0, 0), Size(0, 0), b2_staticBody, nullptr, 0);

		dragbody = b;
		bDrag = true;

		b2MouseJointDef mouseJdef;
		mouseJdef.bodyA = gbody;
		mouseJdef.bodyB = dragbody;
		mouseJdef.target.Set(dragbody->GetPosition().x, dragbody->GetPosition().y);
		mouseJdef.maxForce = 300.0*dragbody->GetMass();

		mouseJ = (b2MouseJoint*)world->CreateJoint(&mouseJdef);
		CCLOG("onTouchBagan");
	}

	return true;
}

void HelloWorld::onTouchMoved(Touch * touch, Event * event)
{
	if (bDrag) { //리동
		mouseJ->SetTarget(b2Vec2(touch->getLocation().x / PTM_RATIO, touch->getLocation().y / PTM_RATIO));
	}
}

void HelloWorld::onTouchEnded(Touch * touch, Event * event)
{
	if (bDrag) {
		//조인트제거
		world->DestroyJoint(mouseJ);
		mouseJ = nullptr;

		dragbody->SetAwake(true);

	}
	bDrag = false;
}

