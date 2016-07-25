//
//  OperationPanel.cpp
//  tiegao
//
//  Created by mac on 16/6/1.
//
//

#include "OperationPanel.h"
#include "DisplayManager.h"
#include "TransactionScene.h"
#include "PromptLayer.h"


OperationPanel::~OperationPanel() {
}

bool OperationPanel::init() {
    if (CCLayer::init()) {
        CCSprite* mask = CCSprite::create("res/pic/mask.png");
        mask->setPosition(DISPLAY->center());
        this->addChild(mask);
        
        _content = CCLayer::create();
//        _content->setScale(0.1);
//        _content->setVisible(false);
        this->addChild(_content);
        
        _bg = CCSprite::create("res/pic/panel/operation/operation_bg.png");
        _bg->setPosition(DISPLAY->center());
        _content->addChild(_bg);
        
        CCSprite* txt_close = CCSprite::create("res/pic/txt_close.png");
        txt_close->setPosition(ccp(DISPLAY->halfW(), DISPLAY->H() * 0.14));
        _content->addChild(txt_close);
        
        CCSprite* purchase1 = CCSprite::create("res/pic/panel/operation/operation_purchase.png");
        CCSprite* purchase2 = CCSprite::create("res/pic/panel/operation/operation_purchase.png");
        purchase2->setScale(DISPLAY->btn_scale());
        CCMenuItem* btn_purchase = CCMenuItemSprite::create(purchase1, purchase2, this, SEL_MenuHandler(&OperationPanel::on_purchase));
        btn_purchase->setPosition(ccp(0, DISPLAY->H() * 0.18));
        
        CCSprite* monthlycard1 = CCSprite::create("res/pic/panel/operation/operation_monthlycard.png");
        CCSprite* monthlycard2 = CCSprite::create("res/pic/panel/operation/operation_monthlycard.png");
        purchase2->setScale(DISPLAY->btn_scale());
        CCMenuItem* btn_monthlycard = CCMenuItemSprite::create(monthlycard1, monthlycard2, this, SEL_MenuHandler(&OperationPanel::on_monthlycard));
//        btn_monthlycard->setPosition(ccp(0, DISPLAY->H() * 0.02));
        
        CCMenu* menu = CCMenu::create(btn_purchase, btn_monthlycard, NULL);
        _content->addChild(menu);
        
        this->setTouchEnabled(true);
        this->setTouchMode(kCCTouchesOneByOne);
        this->setTouchSwallowEnabled(true);
        
        return true;
    }
    else {
        return false;
    }
}

void OperationPanel::onEnter() {
    CCLayer::onEnter();
    
//    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
//    nc->addObserver(this, SEL_CallFuncO(&OperationPanel::hanle_mail_oper), "HTTP_FINISHED_701", NULL);
    
//    this->do_enter();
    
    this->scheduleOnce(SEL_SCHEDULE(&OperationPanel::keyBackStatus), .8f);
}
void OperationPanel::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void OperationPanel::onExit() {
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    this->unscheduleAllSelectors();
    CCLayer::onExit();
}

bool OperationPanel::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent) {
    CCPoint location = pTouch->getLocation();
    if (! _bg->boundingBox().containsPoint(location)) {
//        this->do_exit();
        remove();
    }
    
    return true;
}

#pragma mark - export

void OperationPanel::show_from(CCPoint from) {
    _enter_pos = CCPointMake(from.x, from.y);
    CCDirector::sharedDirector()->getRunningScene()->addChild(this);
}

#pragma mark - inner

void OperationPanel::do_enter() {
    _content->setPosition(_enter_pos - DISPLAY->center());
    _content->setVisible(true);
    
    float duration = 0.5f;
    CCMoveTo* moveto = CCMoveTo::create(duration, CCPointZero);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 1.0);
    CCSpawn* spawn = CCSpawn::create(moveto, scaleto, NULL);
    //    _content->runAction(CCEaseBounceOut::create(spawn));
    _content->runAction(CCEaseElasticOut::create(spawn));
}

void OperationPanel::do_exit() {
    float duration = 0.4f;
    CCMoveTo* moveto = CCMoveTo::create(duration, _enter_pos - DISPLAY->center());
    CCSequence* seq = CCSequence::create(moveto, CCCallFunc::create(this, SEL_CallFunc(&OperationPanel::remove)), NULL);
    CCScaleTo* scaleto = CCScaleTo::create(duration, 0.1);
    CCSpawn* spawn = CCSpawn::create(seq, scaleto, NULL);
    _content->runAction(CCEaseElasticIn::create(spawn));
}

void OperationPanel::remove() {
    this->removeFromParentAndCleanup(true);
}

void OperationPanel::on_purchase(cocos2d::CCMenuItem *btn) {
    this->remove();
    CCNotificationCenter::sharedNotificationCenter()->postNotification("NEED_SHOW_PURCHASEPANEL");
}

void OperationPanel::on_monthlycard(cocos2d::CCMenuItem *btn) {
    PromptLayer* prompt = PromptLayer::create();
    prompt->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "敬请期待!~");
}


void OperationPanel::keyBackClicked(){
    int num_child = CCDirector::sharedDirector()->getRunningScene()->getChildren()->count();
    CCLog("===== children_num: %d", num_child);
    if(num_child > 1)
    {
        return;
    }
    
    this->remove();
}


