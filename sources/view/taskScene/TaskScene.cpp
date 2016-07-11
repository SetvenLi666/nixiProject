//
//  TaskScene.cpp
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#include "TaskScene.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "ConfigManager.h"
#include "NetManager.h"
#include "MainScene.h"
#include "Loading2.h"
#include "TaskStoryScene.h"

#include "ClothesScene.h"
#include "TaskTableView.h"
#include "MZResourceLoader.h"

#include "BuildingLayer.h"


TaskScene::TaskScene(){
    
}
TaskScene::~TaskScene(){
    
}

bool TaskScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    OpenToWhichOne = 0;
    taskPhase = 0;
    taskIndex = 0;
    
    CCDictionary* ratingDic =  DATA->getPlayer()->rating;
    CCString* str = CCString::createWithFormat("%d", DATA->getPlayer()->phase);
    OpenToWhichOne = ((CCInteger* )ratingDic->objectForKey(str->getCString()))->getValue();
    
    taskArr = CONFIG->mission();
    
    allClothesDic = CONFIG->clothes();// 所有衣服
    
    _ManSpr = CCSprite::create();
    this->addChild(_ManSpr, 10);
    
    this->creat_view();
    this->creat_Man();
    this->initClothes();
    
    return true;
}
CCScene* TaskScene::scene(){
    CCScene* scene = CCScene::create();
    TaskScene* layer = TaskScene::create();
    scene->addChild(layer);
    
    return scene;
}

void TaskScene::onEnter(){
    BaseScene::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::creat_Tishi), "Task_Creat_Tishi", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::EnterTheTishi), "Task_EnterTheTishi", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::ExitTishi), "Task_ExitTishi", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::exitView), "Task_ExitView", NULL);
    
    
    nc->addObserver(this, SEL_CallFuncO(&TaskScene::_startCallBack), "HTTP_FINISHED_400", NULL);
}
void TaskScene::onExit(){
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    BaseScene::onExit();
}

void TaskScene::keyBackClicked(){
    
}

void TaskScene::creat_view(){
    
    taskPhase = DATA->getPlayer()->phase;
    CCArray* tempArr = CCArray::create();
    for (int i = 0; i < taskArr->count(); i++) {
        CCDictionary* dic = (CCDictionary* )taskArr->objectAtIndex(i);
        int phase = dic->valueForKey("phase")->intValue();
        if (phase == taskPhase) {
            tempArr->addObject(dic);
        }
    }
    DATA->setTaskSource(tempArr);
    
    BuildingLayer* layer = BuildingLayer::create(DATA->getPlayer()->phase);
    this->addChild(layer);
    
    CCSprite* backSpr1 = CCSprite::create("res/pic/taskScene/task_back.png");
    CCSprite* backSpr2 = CCSprite::create("res/pic/taskScene/task_back.png");
    backSpr2->setScale(1.02f);
    backItem = CCMenuItemSprite::create(backSpr1, backSpr2, this, menu_selector(TaskScene::backCallBack));
    backItem->setPosition(ccp(DISPLAY->ScreenWidth()* .08f, DISPLAY->ScreenHeight()* .04f));
    
    CCMenu* menu = CCMenu::create(backItem, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, 20);
    
    
    CCSprite* historySpr1_1 = CCSprite::create("res/pic/taskScene/task_paopao1.png");
    CCSprite* historySpr1_2 = CCSprite::create("res/pic/taskScene/task_paopao1.png");
    historySpr1_2->setScale(1.02f);
    historyItem1 = CCMenuItemSprite::create(historySpr1_1, historySpr1_2, this, menu_selector(TaskScene::historyCallBack));
    historyItem1->setVisible(false);
    
    CCSprite* historySpr2_1 = CCSprite::create("res/pic/taskScene/task_paopao2.png");
    CCSprite* historySpr2_2 = CCSprite::create("res/pic/taskScene/task_paopao2.png");
    historySpr2_2->setScale(1.02f);
    historyItem2 = CCMenuItemSprite::create(historySpr2_1, historySpr2_2, this, menu_selector(TaskScene::historyCallBack));
    historyItem2->setVisible(false);
    
    CCSprite* historySpr3_1 = CCSprite::create("res/pic/taskScene/task_paopao3.png");
    CCSprite* historySpr3_2 = CCSprite::create("res/pic/taskScene/task_paopao3.png");
    historySpr3_2->setScale(1.02f);
    historyItem3 = CCMenuItemSprite::create(historySpr3_1, historySpr3_2, this, menu_selector(TaskScene::historyCallBack));
    historyItem3->setVisible(false);
    
    int menuIndex = 0;
    CCMenu* historyMenu;
    if (DATA->getPlayer()->phase == 1) {
        menuIndex = 1;
        historyMenu = CCMenu::create(historyItem1, NULL);
    }else if (DATA->getPlayer()->phase == 2){
        menuIndex = 2;
        historyMenu = CCMenu::create(historyItem1, historyItem2, NULL);
    }else if (DATA->getPlayer()->phase == 3){
        menuIndex = 3;
        historyMenu = CCMenu::create(historyItem1, historyItem2, historyItem3, NULL);
    }
    historyMenu->alignItemsHorizontallyWithPadding(5);
    historyMenu->setAnchorPoint(ccp(1, 0));
    historyMenu->setPosition(ccp(DISPLAY->ScreenWidth() - ((historySpr1_1->getContentSize().width* menuIndex + 5* menuIndex)/ 2), DISPLAY->ScreenHeight()* .05f));
    this->addChild(historyMenu, 20);
    
    
    taskKuang = CCSprite::create("res/pic/taskScene/task_dikuang1.png");
    taskKuang->setAnchorPoint(ccp(1, .5f));
//    taskKuang->setPosition(ccp(DISPLAY->ScreenWidth()+7, DISPLAY->ScreenHeight()* .475f));
    taskKuang->setPosition(ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .3f));
    this->addChild(taskKuang, 20);
    
    TaskTableView* tabLayer = TaskTableView::create();
    tabLayer->setPosition(ccp(7, 10));
    tabLayer->setTag(0x77777);
    taskKuang->addChild(tabLayer, 5);
    taskKuang->setScale(.3f);
    
    this->scheduleOnce(SEL_SCHEDULE(&TaskScene::enterTheKuang), .1f);
}
void TaskScene::enterTheKuang(float dt){
    CCMoveTo* moveTo1 = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth() + 7, DISPLAY->ScreenHeight()* .54f));
    CCScaleTo* scaleTo1 = CCScaleTo::create(.5f, 1.f);
    CCSpawn* spawn1 = CCSpawn::create(moveTo1, scaleTo1, NULL);
    taskKuang->runAction(CCSequence::create(spawn1, NULL));
    
    CCFadeIn* fadeIn1 = CCFadeIn::create(.3f);
    historyItem3->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeIn1, NULL));
    
    CCFadeIn* fadeIn2 = CCFadeIn::create(.3f);
    historyItem2->runAction(CCSequence::create(CCDelayTime::create(.7f), CCShow::create(), fadeIn2, NULL));
    
    CCFadeIn* fadeIn3 = CCFadeIn::create(.3f);
    historyItem1->runAction(CCSequence::create(CCDelayTime::create(.9f), CCShow::create(), fadeIn3, NULL));
}

void TaskScene::backCallBack(CCObject* pSender){
    CCScene* scene = MainScene::scene();
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void TaskScene::historyCallBack(CCObject* pSender){
    
}

void TaskScene::creat_Tishi(){
    
    if (this->getChildByTag(0x88888) != NULL) {
        this->removeChildByTag(0x88888);
        kuangSpr = NULL;
    }
    
    int index = DATA->getTaskNumber();
    CCArray* arr = DATA->getTaskSource();
    CCDictionary* taskDic = (CCDictionary* )arr->objectAtIndex(index);
    int taskId = taskDic->valueForKey("id")->intValue();
    CCDictionary* dic;
    int id;
    for (int i = 0; i < taskArr->count(); i++) {
        dic = (CCDictionary* )taskArr->objectAtIndex(i);
        id = dic->valueForKey("id")->intValue();
        if (taskId == id) {
            break;
        }
    }
    int unlockCondition = dic->valueForKey("require")->intValue();
    int tiliIndex = 12;
    
    kuangSpr = CCSprite::create("res/pic/taskScene/task_dikuang2.png");
    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth() + 500, DISPLAY->ScreenHeight()* .12f));
    kuangSpr->setTag(0x88888);
    this->addChild(kuangSpr, 20);
    
    // 标题
    CCLabelTTF* nameLabel = CCLabelTTF::create(getTaskName(id)->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(370, 25), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    nameLabel->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .92f));
    nameLabel->setColor(ccWHITE);
//    nameLabel->setColor(ccc3(80, 63, 68));
    kuangSpr->addChild(nameLabel, 2);
    
    CCLabelTTF* nameLabel2 = CCLabelTTF::create(getTaskName(id)->getCString(), DISPLAY->fangzhengFont(), 25, CCSizeMake(370, 25), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    nameLabel2->setPosition(ccp(kuangSpr->getContentSize().width* .5f + 2, kuangSpr->getContentSize().height* .92f - 2));
    nameLabel2->setColor(ccGRAY);
    kuangSpr->addChild(nameLabel2);
    
    // 提示
    CCLabelTTF* tishiLabel = CCLabelTTF::create("提示: ", DISPLAY->fangzhengFont(), 22, CCSizeMake(370, 22), kCCTextAlignmentLeft,kCCVerticalTextAlignmentCenter);
    tishiLabel->setPosition(ccp(kuangSpr->getContentSize().width* .5f, kuangSpr->getContentSize().height* .78f));
    tishiLabel->setColor(ccc3(103, 81, 95));
//    tishiLabel->enableStroke(ccc3(103, 81, 95), .4f);
    kuangSpr->addChild(tishiLabel, 2);
    
    CCSprite* xianSpr = CCSprite::create("res/pic/taskScene/task_xian.png");
    xianSpr->setAnchorPoint(ccp(0, .5f));
    xianSpr->setPosition(ccp(2, kuangSpr->getContentSize().height* .74f));
    kuangSpr->addChild(xianSpr);
    
    // 开始故事
    CCSprite* startSpr1;
    CCSprite* startSpr2;
    CCMenuItem* startItem;
    if (unlockCondition <= OpenToWhichOne) {
        startSpr1 = CCSprite::create("res/pic/taskScene/task_start.png");
        startSpr2 = CCSprite::create("res/pic/taskScene/task_start.png");
        startSpr2->setScale(1.02f);
        startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, menu_selector(TaskScene::startCallBack));
        startItem->setPosition(ccp(kuangSpr->getContentSize().width* .845f, kuangSpr->getContentSize().height* .225f));
        startItem->setTag(index);
    }else{
        startSpr1 = CCSprite::create("res/pic/taskScene/task_start.png");
        startSpr1->setColor(ccGRAY);
        startSpr2 = CCSprite::create("res/pic/taskScene/task_start.png");
        startSpr2->setColor(ccGRAY);
        startItem = CCMenuItemSprite::create(startSpr1, startSpr2, this, NULL);
        startItem->setPosition(ccp(kuangSpr->getContentSize().width* .845f, kuangSpr->getContentSize().height* .225f));
        startItem->setColor(ccGRAY);
        startItem->setTag(index);
    }
    CCMenu* menu = CCMenu::create(startItem, NULL);
    menu->setPosition(CCPointZero);
    kuangSpr->addChild(menu);
    
    // 小人
//    CCSprite* xiaorenSpr = CCSprite::create("res/pic/taskScene/task_xiaoren.png");
//    xiaorenSpr->setPosition(ccp(kuangSpr->getContentSize().width* .84f, kuangSpr->getContentSize().height* .69f));
//    kuangSpr->addChild(xiaorenSpr, 100);
    
    // 消耗体力
    CCSprite* tiliSpr = CCSprite::create("res/pic/taskScene/task_tili_bar.png");
    tiliSpr->setPosition(ccp(kuangSpr->getContentSize().width* .845f, kuangSpr->getContentSize().height* .066f));
    kuangSpr->addChild(tiliSpr);
    // 体力数
    CCString* labStr4 = CCString::createWithFormat("%d", tiliIndex);
    CCLabelTTF* tiliLabel = CCLabelTTF::create(labStr4->getCString(), DISPLAY->fangzhengFont(), 15);
    tiliLabel->setPosition(ccp(tiliSpr->getContentSize().width* .685f, tiliSpr->getContentSize().height* .45f));
    tiliLabel->setColor(ccWHITE);
//    tiliLabel->enableStroke(ccWHITE, .4f);
    tiliSpr->addChild(tiliLabel);
    // 心
    CCSprite* xinSpr = CCSprite::create("res/pic/qingjingScene/qj_tili.png");
    xinSpr->setScale(.9f);
    xinSpr->setPosition(ccp(tiliSpr->getContentSize().width* .86f, tiliSpr->getContentSize().height* .49f));
    tiliSpr->addChild(xinSpr);
    
    int tag1 = dic->valueForKey("tag1")->intValue();
    int tag2 = dic->valueForKey("tag2")->intValue();
    int tag3 = dic->valueForKey("tag3")->intValue();
    if (tag1 > 0) {
        CCString* tagStr1 = CCString::createWithFormat("res/pic/taskScene/biaoqian/task_biaoqian%d.png", tag1);
        CCSprite* tagSpr1 = CCSprite::create(tagStr1->getCString());
        tagSpr1->setPosition(ccp(kuangSpr->getContentSize().width* .1f, kuangSpr->getContentSize().height* .13f));
        kuangSpr->addChild(tagSpr1);
    }
    if (tag2 > 0) {
        CCString* tagStr2 = CCString::createWithFormat("res/pic/taskScene/biaoqian/task_biaoqian%d.png", tag2);
        CCSprite* tagSpr2 = CCSprite::create(tagStr2->getCString());
        tagSpr2->setPosition(ccp(kuangSpr->getContentSize().width* .26f, kuangSpr->getContentSize().height* .13f));
        kuangSpr->addChild(tagSpr2);
    }
    if (tag3 > 0) {
        CCString* tagStr3 = CCString::createWithFormat("res/pic/taskScene/biaoqian/task_biaoqian%d.png", tag3);
        CCSprite* tagSpr3 = CCSprite::create(tagStr3->getCString());
        tagSpr3->setPosition(ccp(kuangSpr->getContentSize().width* .42f, kuangSpr->getContentSize().height* .13f));
        kuangSpr->addChild(tagSpr3);
    }
}

void TaskScene::startCallBack(CCObject* pSender){
    CCMenuItem* item = (CCMenuItem* )pSender;
    taskIndex = item->getTag();
    
    if (DATA->getClothes()->has_init_clothes == true) {
        this->_startCallBack(NULL);
    }
    else {
        LOADING->show_loading();
        NET->owned_clothes_400();
    }
}

void TaskScene::_startCallBack(CCObject* pSender){
    LOADING->remove();
    CCLayer* layer = ClothesScene::create_with_type(1, taskIndex, taskPhase);
    CCScene* scene = CCScene::create();
    scene->addChild(layer);
    CCTransitionScene* trans = CCTransitionSplitRows::create(0.3f, scene);
    CCDirector::sharedDirector()->replaceScene(trans);
}

void TaskScene::EnterTheTishi(){
    CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth() - kuangSpr->getContentSize().width* .5f, DISPLAY->ScreenHeight()* .12f));
    kuangSpr->runAction(moveTo);
}
void TaskScene::ExitTishi(){
    if (this->getChildByTag(0x88888) != NULL) {
        kuangSpr->removeAllChildren();
        this->removeChildByTag(0x88888);
        kuangSpr = NULL;
    }
}

CCString* TaskScene::getTaskName(int index){
    CCDictionary* dic = (CCDictionary* )taskArr->objectAtIndex(index - 1);
    
    return (CCString*)dic->valueForKey("name");
}
CCString* TaskScene::getTaskDescription(int index){
    CCDictionary* dic = (CCDictionary* )taskArr->objectAtIndex(index - 1);
    
    return (CCString*)dic->valueForKey("desc");
}


void TaskScene::exitView(){
    BaseScene::hideBaseScene();
    CCCallFunc* callFunc = CCCallFunc::create(this, SEL_CallFunc(&TaskScene::openTaskStoryScene));
    
    
    CCFadeOut* fadeOut1 = CCFadeOut::create(.3f);
    historyItem1->runAction(CCSequence::create(CCDelayTime::create(.5f), CCShow::create(), fadeOut1, NULL));
    
    CCFadeOut* fadeOut2 = CCFadeOut::create(.3f);
    historyItem2->runAction(CCSequence::create(CCDelayTime::create(.7f), CCShow::create(), fadeOut2, NULL));
    
    CCFadeOut* fadeOut3 = CCFadeOut::create(.3f);
    historyItem3->runAction(CCSequence::create(CCDelayTime::create(.9f), CCShow::create(), fadeOut3,callFunc, NULL));
    
    CCMoveTo* moveTo = CCMoveTo::create(.5f, ccp(DISPLAY->ScreenWidth()+ 300, DISPLAY->ScreenHeight()* .54f));
    CCScaleBy* scaleBy = CCScaleBy::create(.5f, .5f);
    CCSpawn* spawn = CCSpawn::create(moveTo, scaleBy, NULL);
    taskKuang->runAction(CCSequence::create(CCDelayTime::create(.2f), spawn, NULL));
}
void TaskScene::openTaskStoryScene(){
    CCScene* scene = TaskStoryScene::scene();
    CCDirector::sharedDirector()->replaceScene(scene);
}


void TaskScene::creat_Man(){
    float widthFolt = .65f;
    float heightFloat = .3f;
    float scaleFloat = 1.5f;
    
    CCSprite* manSpr = CCSprite::create("res/pic/clothesScene/man/gj_man.png");
    manSpr->setScale(scaleFloat);
    manSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(manSpr, 200);
    _touSpr = CCSprite::create("res/pic/clothesScene/man/gj_lian.png");
    _touSpr->setScale(scaleFloat);
    _touSpr->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
    _ManSpr->addChild(_touSpr, 210);
}
void TaskScene::initClothes(){//穿衣服
    CCDictionary* myClothesTemp = DATA->getClothes()->MyClothesTemp(); // 男宠衣着
    float widthFolt = .65f;
    float heightFloat = .3f;
    float scaleFloat = 1.5f;
    bool flipxBool = false;
    
    for (int i = Tag_GJ_TouFa; i <= Tag_GJ_ZhuangRong; i++) {
        if (i == Tag_GJ_TouFa) {
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 10000) {
                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10000);
                _tfSpr1 = CCSprite::create(str1->getCString());
                _tfSpr1->setScale(scaleFloat);
                _tfSpr1->setFlipX(flipxBool);
                _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr1->setTag(Tag_GJ_TouFa1);
                _ManSpr->addChild(_tfSpr1, 430);
                
                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", 10009);
                _tfSpr2 = CCSprite::create(str2->getCString());
                _tfSpr2->setScale(scaleFloat);
                _tfSpr2->setFlipX(flipxBool);
                _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _tfSpr2->setTag(Tag_GJ_TouFa2);
                _ManSpr->addChild(_tfSpr2, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _tfSpr1 = CCSprite::create(str1->getCString());
                            _tfSpr1->setScale(scaleFloat);
                            _tfSpr1->setFlipX(flipxBool);
                            _tfSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr1->setTag(Tag_GJ_TouFa1);
                            _ManSpr->addChild(_tfSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _tfSpr2 = CCSprite::create(str2->getCString());
                            _tfSpr2->setScale(scaleFloat);
                            _tfSpr2->setFlipX(flipxBool);
                            _tfSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr2->setTag(Tag_GJ_TouFa2);
                            _ManSpr->addChild(_tfSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/1toufa/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _tfSpr3 = CCSprite::create(str3->getCString());
                            _tfSpr3->setScale(scaleFloat);
                            _tfSpr3->setFlipX(flipxBool);
                            _tfSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _tfSpr3->setTag(Tag_GJ_TouFa3);
                            _ManSpr->addChild(_tfSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_WaiTao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 20000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", 20000);
                _wtSpr1 = CCSprite::create(str->getCString());
                _wtSpr1->setScale(scaleFloat);
                _wtSpr1->setFlipX(flipxBool);
                _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wtSpr1->setTag(Tag_GJ_WaiTao1);
                _ManSpr->addChild(_wtSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wtSpr1 = CCSprite::create(str1->getCString());
                            _wtSpr1->setScale(scaleFloat);
                            _wtSpr1->setFlipX(flipxBool);
                            _wtSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr1->setTag(Tag_GJ_WaiTao1);
                            _ManSpr->addChild(_wtSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wtSpr2 = CCSprite::create(str2->getCString());
                            _wtSpr2->setScale(scaleFloat);
                            _wtSpr2->setFlipX(flipxBool);
                            _wtSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr2->setTag(Tag_GJ_WaiTao2);
                            _ManSpr->addChild(_wtSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/2waitao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wtSpr3 = CCSprite::create(str3->getCString());
                            _wtSpr3->setScale(scaleFloat);
                            _wtSpr3->setFlipX(flipxBool);
                            _wtSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wtSpr3->setTag(Tag_GJ_WaiTao3);
                            _ManSpr->addChild(_wtSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_ShangYi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 30000) {
//                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", 30000);
//                _sySpr1 = CCSprite::create(str->getCString());
//                _sySpr1->setScale(scaleFloat);
//                _sySpr1->setFlipX(flipxBool);
//                _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
//                _sySpr1->setTag(Tag_GJ_ShangYi1);
//                _ManSpr->addChild(_sySpr1, 350);
                
                CCSprite* xingSpr1 = CCSprite::create("res/pic/taskScene/task_xing2.png");
                xingSpr1->setFlipX(true);
                xingSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .22f, DISPLAY->ScreenHeight()* .488f));
                _ManSpr->addChild(xingSpr1, 420);
                
                CCSprite* xingSpr2 = CCSprite::create("res/pic/taskScene/task_xing2.png");
                xingSpr2->setFlipX(true);
                xingSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* .385f, DISPLAY->ScreenHeight()* .473f));
                _ManSpr->addChild(xingSpr2, 420);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _sySpr1 = CCSprite::create(str1->getCString());
                            _sySpr1->setScale(scaleFloat);
                            _sySpr1->setFlipX(flipxBool);
                            _sySpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr1->setTag(Tag_GJ_ShangYi1);
                            _ManSpr->addChild(_sySpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _sySpr2 = CCSprite::create(str2->getCString());
                            _sySpr2->setScale(scaleFloat);
                            _sySpr2->setFlipX(flipxBool);
                            _sySpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr2->setTag(Tag_GJ_ShangYi2);
                            _ManSpr->addChild(_sySpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/3shangyi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _sySpr3 = CCSprite::create(str3->getCString());
                            _sySpr3->setScale(scaleFloat);
                            _sySpr3->setFlipX(flipxBool);
                            _sySpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _sySpr3->setTag(Tag_GJ_ShangYi3);
                            _ManSpr->addChild(_sySpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_KuZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 40000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", 40000);
                _kzSpr1 = CCSprite::create(str->getCString());
                _kzSpr1->setScale(scaleFloat);
                _kzSpr1->setFlipX(flipxBool);
                _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _kzSpr1->setTag(Tag_GJ_KuZi1);
                _ManSpr->addChild(_kzSpr1, 290);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _kzSpr1 = CCSprite::create(str1->getCString());
                            _kzSpr1->setScale(scaleFloat);
                            _kzSpr1->setFlipX(flipxBool);
                            _kzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr1->setTag(Tag_GJ_KuZi1);
                            _ManSpr->addChild(_kzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _kzSpr2 = CCSprite::create(str2->getCString());
                            _kzSpr2->setScale(scaleFloat);
                            _kzSpr2->setFlipX(flipxBool);
                            _kzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr2->setTag(Tag_GJ_KuZi2);
                            _ManSpr->addChild(_kzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/4kuzi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _kzSpr3 = CCSprite::create(str3->getCString());
                            _kzSpr3->setScale(scaleFloat);
                            _kzSpr3->setFlipX(flipxBool);
                            _kzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _kzSpr3->setTag(Tag_GJ_KuZi3);
                            _ManSpr->addChild(_kzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_WaZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 50000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", 50000);
                _wzSpr1 = CCSprite::create(str->getCString());
                _wzSpr1->setScale(scaleFloat);
                _wzSpr1->setFlipX(flipxBool);
                _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _wzSpr1->setTag(Tag_GJ_WaZi1);
                _ManSpr->addChild(_wzSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _wzSpr1 = CCSprite::create(str1->getCString());
                            _wzSpr1->setScale(scaleFloat);
                            _wzSpr1->setFlipX(flipxBool);
                            _wzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr1->setTag(Tag_GJ_WaZi1);
                            _ManSpr->addChild(_wzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _wzSpr2 = CCSprite::create(str2->getCString());
                            _wzSpr2->setScale(scaleFloat);
                            _wzSpr2->setFlipX(flipxBool);
                            _wzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr2->setTag(Tag_GJ_WaZi2);
                            _ManSpr->addChild(_wzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/5wazi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _wzSpr3 = CCSprite::create(str3->getCString());
                            _wzSpr3->setScale(scaleFloat);
                            _wzSpr3->setFlipX(flipxBool);
                            _wzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _wzSpr3->setTag(Tag_GJ_WaZi3);
                            _ManSpr->addChild(_wzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_XieZi){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 60000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", 60000);
                _xzSpr1 = CCSprite::create(str->getCString());
                _xzSpr1->setScale(scaleFloat);
                _xzSpr1->setFlipX(flipxBool);
                _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _xzSpr1->setTag(Tag_GJ_XieZi1);
                _ManSpr->addChild(_xzSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _xzSpr1 = CCSprite::create(str1->getCString());
                            _xzSpr1->setScale(scaleFloat);
                            _xzSpr1->setFlipX(flipxBool);
                            _xzSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr1->setTag(Tag_GJ_XieZi1);
                            _ManSpr->addChild(_xzSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _xzSpr2 = CCSprite::create(str2->getCString());
                            _xzSpr2->setScale(scaleFloat);
                            _xzSpr2->setFlipX(flipxBool);
                            _xzSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr2->setTag(Tag_GJ_XieZi2);
                            _ManSpr->addChild(_xzSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/6xiezi/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _xzSpr3 = CCSprite::create(str3->getCString());
                            _xzSpr3->setScale(scaleFloat);
                            _xzSpr3->setFlipX(flipxBool);
                            _xzSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _xzSpr3->setTag(Tag_GJ_XieZi3);
                            _ManSpr->addChild(_xzSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }
        else if (i == Tag_GJ_ShiPin){
            CCDictionary* shipinDic = (CCDictionary* )myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString());// 获取所穿视频的字典
            
            CCInteger* cloth_id;
            for (int j = 11; j <= 20; j++) {
                cloth_id = (CCInteger* )shipinDic->objectForKey(CCString::createWithFormat("%d", j)->getCString());
                if (cloth_id->getValue() == 70000) {
                    CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", 70000);
                    CCSprite* _spSpr1 = CCSprite::create(str->getCString());
                    _spSpr1->setScale(scaleFloat);
                    _spSpr1->setFlipX(flipxBool);
                    _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* .5f));
                    _spSpr1->setTag(j + 1000);
                    _ManSpr->addChild(_spSpr1, 50);
                }else{
                    CCDictionary* dic = CONFIG->clothes();// 所有衣服
                    CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                    for (int k = 0; k < clothesArr->count(); k++) {
                        CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(k);
                        int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                        if (now_clothes_Id == cloth_id->getValue()) {
                            const CCString* layer1 =  clothDic->valueForKey("layer1");
                            const CCString* layer2 =  clothDic->valueForKey("layer2");
                            const CCString* layer3 =  clothDic->valueForKey("layer3");
                            if (layer1->compare("") != 0) {
                                CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer1")->intValue());
                                CCSprite* _spSpr1 = CCSprite::create(str1->getCString());
                                _spSpr1->setScale(scaleFloat);
                                _spSpr1->setFlipX(flipxBool);
                                _spSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr1->setTag(j + 1000);
                                _ManSpr->addChild(_spSpr1, clothDic->valueForKey("z_order1")->intValue());
                            }
                            
                            if (layer2->compare("") != 0) {
                                CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer2")->intValue());
                                CCSprite* _spSpr2 = CCSprite::create(str2->getCString());
                                _spSpr2->setScale(scaleFloat);
                                _spSpr2->setFlipX(flipxBool);
                                _spSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr2->setTag(j + 2000);
                                _ManSpr->addChild(_spSpr2, clothDic->valueForKey("z_order2")->intValue());
                            }
                            
                            if (layer3->compare("") != 0) {
                                CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/7shipin/%d.png", clothDic->valueForKey("layer3")->intValue());
                                CCSprite* _spSpr3 = CCSprite::create(str3->getCString());
                                _spSpr3->setScale(scaleFloat);
                                _spSpr3->setFlipX(flipxBool);
                                _spSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                                _spSpr3->setTag(j + 3000);
                                _ManSpr->addChild(_spSpr3, clothDic->valueForKey("z_order3")->intValue());
                            }
                            break;
                        }
                    }
                }
            }
        }
        else if (i == Tag_GJ_Bao){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 80000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", 80000);
                _bSpr1 = CCSprite::create(str->getCString());
                _bSpr1->setScale(scaleFloat);
                _bSpr1->setFlipX(flipxBool);
                _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _bSpr1->setTag(Tag_GJ_Bao1);
                _ManSpr->addChild(_bSpr1, 50);
            }else{
                CCArray* clothesArr = (CCArray* )allClothesDic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        const CCString* layer2 =  clothDic->valueForKey("layer2");
                        const CCString* layer3 =  clothDic->valueForKey("layer3");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _bSpr1 = CCSprite::create(str1->getCString());
                            _bSpr1->setScale(scaleFloat);
                            _bSpr1->setFlipX(flipxBool);
                            _bSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr1->setTag(Tag_GJ_Bao1);
                            _ManSpr->addChild(_bSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        
                        if (layer2->compare("") != 0) {
                            CCString* str2 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer2")->intValue());
                            _bSpr2 = CCSprite::create(str2->getCString());
                            _bSpr2->setScale(scaleFloat);
                            _bSpr2->setFlipX(flipxBool);
                            _bSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr2->setTag(Tag_GJ_Bao2);
                            _ManSpr->addChild(_bSpr2, clothDic->valueForKey("z_order2")->intValue());
                        }
                        
                        if (layer3->compare("") != 0) {
                            CCString* str3 = CCString::createWithFormat("res/pic/clothesScene/clothes/8bao/%d.png", clothDic->valueForKey("layer3")->intValue());
                            _bSpr3 = CCSprite::create(str3->getCString());
                            _bSpr3->setScale(scaleFloat);
                            _bSpr3->setFlipX(flipxBool);
                            _bSpr3->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _bSpr3->setTag(Tag_GJ_Bao3);
                            _ManSpr->addChild(_bSpr3, clothDic->valueForKey("z_order3")->intValue());
                        }
                        break;
                    }
                }
            }
        }else if (i == Tag_GJ_ZhuangRong){
            CCInteger* cloth_id = (CCInteger*)myClothesTemp->objectForKey(CCString::createWithFormat("%d", i)->getCString()); // 男宠当前所穿上衣
            
            if (cloth_id->getValue() == 90000) {
                CCString* str = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/90000.png");
                _zrSpr1 = CCSprite::create(str->getCString());
                _zrSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                _zrSpr1->setTag(Tag_GJ_ZhuangRong1);
                _zrSpr1->setScale(scaleFloat);
                _zrSpr1->setFlipX(flipxBool);
                _ManSpr->addChild(_zrSpr1, 220);
            }else{
                CCDictionary* dic = CONFIG->clothes();// 所有衣服
                CCArray* clothesArr = (CCArray* )dic->objectForKey(i);// 获得当前类型所有衣服
                for (int j = 0; j < clothesArr->count(); j++) {
                    CCDictionary* clothDic = (CCDictionary* )clothesArr->objectAtIndex(j);
                    int now_clothes_Id = clothDic->valueForKey("id")->intValue();
                    if (now_clothes_Id == cloth_id->getValue()) {
                        const CCString* layer1 =  clothDic->valueForKey("layer1");
                        if (layer1->compare("") != 0) {
                            CCString* str1 = CCString::createWithFormat("res/pic/clothesScene/clothes/9zhuangrong/%d.png", clothDic->valueForKey("layer1")->intValue());
                            _zrSpr1 = CCSprite::create(str1->getCString());
                            _zrSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* widthFolt, DISPLAY->ScreenHeight()* heightFloat));
                            _zrSpr1->setTag(Tag_GJ_ZhuangRong1);
                            _zrSpr1->setScale(scaleFloat);
                            _zrSpr1->setFlipX(flipxBool);
                            _ManSpr->addChild(_zrSpr1, clothDic->valueForKey("z_order1")->intValue());
                        }
                        break;
                    }
                }
            }
        }
    }
}
