#include "3dScene.h"


cocos2d::CCScene* Canvas::scene(){
	CCScene* scene=CCScene::create();
	Canvas* layer=Canvas::create();

	scene->addChild(layer);
	return scene;
}


bool Canvas::init(){
	if (!CCLayer::init()){
		return false;
	}

	 
	CachesTable::init();

	this->mCurrentBtn=NULL;
	this->initButtons();
	this->initRenderer();
 
	
	
	scheduleUpdate();
	return true;
}
 
 
void Canvas::update(float delta){

	if (testcase!=nullptr   ){

		//CCLOG("%f",delta);

		testcase->update(delta);
	}

}

void Canvas::initRenderer(){
	enginePtr=new Engine(RENDER_VIEWPORT_WIDTH, RENDER_VIEWPORT_HEIGHT);
	
	CCSprite* sprite=enginePtr->getSprite();

	if (sprite){
		sprite->setAnchorPoint(ccp(0,1));
		sprite->setPosition(ccp(0,DEFAULT_SCREEN_HEIGHT));
		int i=this->getChildrenCount();
		this->addChild(sprite); 
		
		i=this->getChildrenCount();
		CCLOG("%i",i);
	}
}

void Canvas::initButtons(){
	char* texts[]={"1.2D","2.Wire3D","3.Camera","4.Light","5.Gouraud","6.Texture","7.Clip","8.Terrain","9.Alpha","10.ZBuffer","11.Correct","12.Bilerp","13.Multiple","14.debug"};

	int x=LABEL_LEFT,y=LABEL_TOP;
	int width=100,height=20;

	for (int i=0;i<sizeof(texts)/sizeof(char*);i++){
	
		CCControlButton* btn=Canvas::createButton(NULL, NULL,texts[i],x,y,width,height);
		mButtons.push_back(btn);
		if (i==13){
			btn->addTargetWithActionForControlEvents(this,cccontrol_selector(Canvas::buttonNextTouchEvent),CCControlEventTouchDown );
		}else{
			
			//btn->addTargetWithActionForControlEvent
			btn->addTargetWithActionForControlEvents(this,cccontrol_selector(Canvas::buttonTouchEvent),CCControlEventTouchDown);
		}

		this->addChild(btn);

		switch (i){
		case 0:
			testcase=new TestCase_2D();
			break;
		case 1:
			testcase=new TestCase_Wire3D();
			break;
		case 2:
			testcase=new TestCase_Camera();
			break;
		case 3:
			testcase=new TestCase_Light();
			break;
		case 4:
			testcase=new TestCase_Gouraud();
			break;
		case 5:
			testcase=new TestCase_Texture();
			break;
 
		default:
			testcase=nullptr;
		}
		btn->setUserData(testcase);
		y=y-40;
	}

}

void Canvas::buttonNextTouchEvent(CCObject* pSender, CCControlEvent event){
	if(testcase!=nullptr){
		//TestCase_Gouraud* t=(TestCase_Gouraud*)
		testcase->debug();
	}
}

void Canvas::buttonTouchEvent(CCObject* pSender, CCControlEvent event){
	//CCLOG("%s","ajsdhfja");
	CCControlButton* controlBtn=static_cast<CCControlButton*>(pSender);
	if (controlBtn!=nullptr){
		TestCase* tester=static_cast<TestCase*>(controlBtn->getUserData());
		if (tester==nullptr){
			return;
		}

		if (mCurrentBtn!=nullptr){
			mCurrentBtn->setTitleColorForState(ccc3(255,255,255),CCControlStateNormal);
			mCurrentBtn->setTitleColorForState(ccc3(255,255,0),CCControlStateHighlighted);
		}
		if (tester!=nullptr){
			tester->free();
		}

		controlBtn->setTitleColorForState(ccc3(255,255,0),CCControlStateNormal);
		controlBtn->setTitleColorForState(ccc3(255,255,255),CCControlStateHighlighted);
		mCurrentBtn=controlBtn;
		testcase=tester;
		testcase->init(enginePtr);
	}
}

 

CCControlButton* Canvas::createButton(const char* keyUpPng, const char* keyDownPng, const char* txt,
	int x,int y,float width,float height){
		CCControlButton* controlBtn=NULL;

		if (keyUpPng!=NULL&&keyDownPng!=NULL){
			CCScale9Sprite* btnNormal = CCScale9Sprite::create(keyUpPng);
			CCScale9Sprite* btnDown = CCScale9Sprite::create(keyDownPng); 
			CCLabelTTF *title = CCLabelTTF::create(txt, "Marker Felt", 30);  
			controlBtn = CCControlButton::create(title, btnNormal); 
			controlBtn->setBackgroundSpriteForState(btnDown, CCControlStateSelected); 
		}else{
			controlBtn = CCControlButton::create(txt, "Marker Felt", 26);  
			controlBtn->setTitleColorForState(ccc3(255,255,255),CCControlStateNormal);
			controlBtn->setTitleColorForState(ccc3(255,255,0),CCControlStateHighlighted);
		}

		controlBtn->setAnchorPoint(ccp(0,0));
		//controlBtn->set
		controlBtn->setPosition(ccp(x,y));
		controlBtn->setPreferredSize(CCSize(width,height));
		controlBtn->setTouchEnabled(true);
		return controlBtn;
}



