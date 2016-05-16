#ifndef _3DSCENE_H_
#define _3DSCENE_H_
#include <cocos2d.h>
#include "GUI/CCControlExtension/CCControlButton.h"
#include "config.h"
#include "testcase.h"
#include "caches.h"
#include "3dengine.h"


USING_NS_CC;
using namespace cocos2d::extension;

class Canvas:public cocos2d::CCLayer{
public:
	virtual bool init();
	static cocos2d::CCScene* scene();
	virtual void update(float dt) override;

	
	// a selector callback
	void menuCloseCallback(CCObject* pSender);

	

	CREATE_FUNC(Canvas);
private:
	void initButtons();
	void initRenderer();

	static CCControlButton* Canvas::createButton(const char* keyUpPng,const char* keyDownPng, 
														const char* txt,int x,int y,float width,float height);
public:
	std::vector<CCControlButton*> mButtons;
	void buttonTouchEvent(CCObject* pSender, CCControlEvent event); 
	void buttonNextTouchEvent(CCObject* pSender, CCControlEvent event);

private:
	CCControlButton* mCurrentBtn;
	TestCase* testcase;
	Engine* enginePtr;

};



#endif