//
//  VipStoryScene.cpp
//  mm3c
//
//  Created by mac on 14-11-27.
//
//

#include "VipStoryScene.h"
#include "MainScene.h"
#include "DataManager.h"
#include "VipQingjingScene.h"
#include "DisplayManager.h"
#include "VipLabelColorLayer.h"
#include "Loading2.h"
#include "NetManager.h"
#include "VipStorySettlementOfTheAnimationLayer.h"
#include "PromptLayer.h"
#include "AudioManager.h"


VipStoryScene::VipStoryScene(){
    
}
VipStoryScene::~VipStoryScene(){
    
}

bool VipStoryScene::init(){
    if (!BaseScene::init()) {
        return false;
    }
    
    num_child = 0;
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    m_current_story_index_id = 0;
    storyIndex = 0;
    openStory = false;
    m_bIsKJSelect = false;
    m_bIsZDSelect = false;
    _dikuangBool = false;
    passersbyBool = false;
    manBool = false;
    passersbyBool1 = false;
    passersbyBool2 = false;
    dikuangBool = false;
    quanBool = false;
    manString1 = "";
    manString2 = "";
    
    oneBool = false;
    twoBool = false;
    
    _dkSpr = CCSprite::create();
    this->addChild(_dkSpr, 10);
    
    DATA->setStoryLabelArr(CCArray::create());
    
    DATA->setFastForward(true);
    
    CCArray* animations = CCArray::createWithCapacity(5);
    char strPei[100] = {};
    for (int i = 2; i >= 1; i--) {
    
        sprintf(strPei, "res/pic/qingjingScene/storyscene/qj_act%d.png", i);
        CCSpriteFrame *frame = CCSpriteFrame::create(strPei,CCRectMake(0, 0, 107, 107));
        animations->addObject(frame);
    }
    CCAnimation* animation = CCAnimation::createWithSpriteFrames(animations, .3f);
    CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "quanStr");
    
    
//    MMDataManager::get_instance()->show_paper();
    
    return true;
}

CCScene* VipStoryScene::scene(){
    CCScene* scene = CCScene::create();
    
    VipStoryScene* layer = VipStoryScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

VipStoryScene* VipStoryScene::create_with_story_id(int _index){
    VipStoryScene* rtn = VipStoryScene::create();
    rtn->init_with_story_id(_index);
    
    return rtn;
}

void VipStoryScene::init_with_story_id(int _index){
    
    index = 0;
    tagIndex = 0;
    nowIndex = 0;
    wordCount = 0;
    contentLength = 0;
    
    buttonBool = false;
    buttonBool1 = false;
    buttonBool2 = false;
    buttonBool3 = false;
    recordBool1 = false;
    recordBool2 = false;
    recordBool3 = false;
    openButtonBool = false;
    
    m_current_story_index_id = _index;
    
    CCSprite* shangkuangSpr = CCSprite::create("res/pic/qingjingScene/storyscene/qj_shangkuang.png");
    shangkuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .97f));
    this->addChild(shangkuangSpr, 50);
    CCLabelTTF* shangLabel = CCLabelTTF::create(DISPLAY->GetOffTheName2(m_current_story_index_id)->getCString(), DISPLAY->fangzhengFont(), 30);
    shangLabel->setPosition(ccp(shangkuangSpr->getContentSize().width* .5f, shangkuangSpr->getContentSize().height* .5f));
    shangLabel->setColor(ccWHITE);
    shangkuangSpr->addChild(shangLabel);
    
    vipDialog = VipDialogs::create();
    vipDialog->retain();
    CCString* fileStr = CCString::createWithFormat("res/vipStory/80100/%s", DISPLAY->GetOffTheNumber2(m_current_story_index_id)->getCString());
//    CCString* fileStr = CCString::createWithFormat("res/vipStory/80100/story_80100_101_%d", 5);
//    MZLog("fileStr === %s", fileStr->getCString());
    vipDialog->config_with_file((char* )fileStr->getCString());
    vipDialogItem = (VipDialogItem* )vipDialog->getVipDialogs()->objectAtIndex(index);
    
    this->init(vipDialogItem);
    
    kuangSpr = CCSprite::create("res/pic/qingjingScene/storyscene/qj_di.png");
    kuangSpr->setAnchorPoint(ccp(.5f, 0));
    kuangSpr->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5, 80 - 500));
    kuangSpr->setTag(Tag_GJ_kuang);
    _dkSpr->addChild(kuangSpr, 10);
    
    DATA->setDiKuangSize(kuangSpr->boundingBox().size);
    
    quanSpr = CCSprite::create("res/pic/qingjingScene/storyscene/qj_act1.png");
    quanSpr->setPosition(ccp(kuangSpr->getContentSize().width* .96f, -12));
    quanSpr->setTag(0x88999);
    kuangSpr->addChild(quanSpr, 100);
    CCAnimation* manAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("quanStr");
    CCAnimate* animate = CCAnimate::create(manAnimation);
    CCRepeatForever* rep = CCRepeatForever::create(animate);
    CCDirector::sharedDirector()->getActionManager()->addAction(rep, quanSpr, false);
    quanSpr->setVisible(false);
    
    this->addButton();
    
    float widSize = kuangSpr->getContentSize().width;
    float heiSize = kuangSpr->getContentSize().height;
    
    nameKuang = CCSprite::create("res/pic/qingjingScene/storyscene/qj_namekuang.png");
    nameKuang->setPosition(ccp(kuangSpr->getContentSize().width* .15f, kuangSpr->getContentSize().height - 10));
    kuangSpr->addChild(nameKuang);
    nameKuang->setVisible(false);
    
    saidLabel = CCLabelTTF::create("", DISPLAY->fangzhengFont(), 25, CCSizeMake(widSize* .8f, heiSize* .6f), kCCTextAlignmentLeft, kCCVerticalTextAlignmentTop);
    saidLabel->setAnchorPoint(CCPointZero);
    saidLabel->setPosition(ccp(widSize* .11, 25));
    saidLabel->setTag(0x222);
    saidLabel->setColor(ccc3(80, 63, 68));
    kuangSpr->addChild(saidLabel, 8);
    
    nameLab = CCLabelTTF::create("", DISPLAY->fangzhengFont(), 27);
    nameLab->setPosition(ccp(nameKuang->getContentSize().width* .5f, nameKuang->getContentSize().height* .5f));
    nameLab->setTag(0x111);
    nameLab->setColor(ccWHITE);
    nameLab->enableStroke(ccWHITE, .4f);
    nameKuang->addChild(nameLab, 8);
    
    this->dialogueControl(vipDialogItem);
    
    this->schedule(schedule_selector(VipStoryScene::gengxin), .1f);
}

void VipStoryScene::dialogueControl(VipDialogItem* dialItem){
//    CCLog("index ==== %d", dialItem->getVipIndex());
//    CCLog("states ==== %d", dialItem->getVipStates());
//    CCLog("passersby ==== %d", dialItem->getVipPassersby());
//    CCLog("name ==== %s", dialItem->getVipName().c_str());
//    CCLog("said ==== %s", dialItem->getVipSaid().c_str());
//    CCLog("bg ==== %s", dialItem->getVipBg().c_str());
//    CCLog("bust ==== %d", dialItem->getVipBust());
//
//    CCLog("head_1 ==== %s", dialItem->getVipHead_1().c_str());
//    CCLog("figure_1 ==== %d", dialItem->getVipFigure_1());
//    CCLog("zishi_1 ==== %s", dialItem->getVipZishi_1().c_str());
//
//    CCLog("head_2 ==== %s", dialItem->getVipHead_2().c_str());
//    CCLog("figure_2 ==== %d", dialItem->getVipFigure_2());
//    CCLog("zishi_2 ==== %s", dialItem->getVipZishi_2().c_str());
//
//    CCLog("animation ==== %d", dialItem->getVipAnimation());
////    CCLog("achievenment ==== %d", dialItem->getVipAchievenment());
//    CCLog("next ==== %d", dialItem->getVipNext());
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    this->setTouchEnabled(false);
    logIndex = 0;
    
    if (dialItem->getVipNext() == -2) {
        chengjiuStr = dialItem->getVipSaid().c_str();
    }
    
    if (dialItem->getVipStates() == 0) {// ==0 没有人
        
        this->recordLabel(dialItem);
        
        this->removeMan();
        
        openStory = true;
        manString1 = "";
        manString2 = "";
        manBool = false;
        passersbyBool1 = false;
        passersbyBool2 = false;
        
        oneBool = false;
        twoBool = false;
        
        if (dialItem->getVipBg() != "keep") {
//            if (this->getChildByTag(Tag_GJ_kuang) != NULL) {
//                this->removeChildByTag(Tag_GJ_kuang);
//            }
            if (this->getChildByTag(0x88888) != NULL) {
                this->removeChildByTag(0x88888);
            }
            
            this->unschedule(schedule_selector(VipStoryScene::logic));
            this->outKuang();
            this->emptyLabel();
            this->creatBg();
            
            this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::addKuang), 1.2f);
            
        }else{
            
            if (_dikuangBool) {
                this->schedule(schedule_selector(VipStoryScene::logic), .1f);
            }else{
                this->unschedule(schedule_selector(VipStoryScene::logic));

                this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::addKuang), 1.2f);
            }
            
            
        }
        
    }else if (dialItem->getVipStates() == 1){// ==1 一个人
        
        this->recordLabel(dialItem);
        
        this->unschedule(schedule_selector(VipStoryScene::logic));
        openStory = true;
        passersbyBool1 = false;
        passersbyBool2 = false;
        
        oneBool = true;
        twoBool = false;
        
        if (dialItem->getVipBg() != "keep") {
            
//            if (this->getChildByTag(Tag_GJ_kuang) != NULL) {
//                this->removeChildByTag(Tag_GJ_kuang);
//            }
            if (this->getChildByTag(0x88888) != NULL) {
                this->removeChildByTag(0x88888);
            }
            
            this->removeMan();
            
            this->outKuang();
            this->emptyLabel();
            this->creatBg();
            
            this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::creatMan), 1.2f);
            this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::addKuang), 1.6f);
            
            this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::closeEyesAnimation), 1.2f);
            this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::creatManEyesAnimation), 3.1f);
        }else{
            
            this->removeMan();
            
            this->emptyLabel();
            
            this->creatMan(0);
            
            this->schedule(schedule_selector(VipStoryScene::logic), .1f);
        }
        
    }else if (dialItem->getVipStates() == 2){// ==2 两个人
        
        this->removeMan();
        this->recordLabel(dialItem);
        
        this->unschedule(schedule_selector(VipStoryScene::logic));
        
        openStory = true;
        manBool = false;
        
        oneBool = false;
        twoBool = true;
        
        if (dialItem->getVipBg() != "keep") {
            
//            if (this->getChildByTag(Tag_GJ_kuang) != NULL) {
//                this->removeChildByTag(Tag_GJ_kuang);
//            }
            if (this->getChildByTag(0x88888) != NULL) {
                this->removeChildByTag(0x88888);
            }
            
            this->outKuang();
            this->emptyLabel();
            this->creatBg();
            
            this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::creatPassersbyMan), 1.2f);
            this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::addKuang), 1.6f);
            
            this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::closeEyesAnimation), 1.2f);
            this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::toPassersbyEyesAnimation), 3.1f);
            
        }else{
            
            this->emptyLabel();
            this->creatPassersbyMan(0);
            
            this->schedule(schedule_selector(VipStoryScene::logic), .1f);
        }
        
    }else if (dialItem->getVipStates() == 3){// ==3 保持之前状态
        
        this->recordLabel(dialItem);
        
        openStory = true;
        
        if (dialItem->getVipBg() != "keep") {
            this->emptyLabel();
            
            this->creatBg();
            
            if (_dikuangBool) {
                this->schedule(schedule_selector(VipStoryScene::logic), .1f);
                
                if (oneBool) {
                    this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::creatManEyesAnimation), .1f);
                }else if (twoBool){
                    this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::toPassersbyEyesAnimation), .1f);
                }
            }else{
                this->unschedule(schedule_selector(VipStoryScene::logic));

                this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::addKuang), 1.2f);
                
                if (oneBool) {
                    this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::creatManEyesAnimation), 1.5f);
                }else if (twoBool){
                    this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::toPassersbyEyesAnimation), 1.5f);
                }
            }
        }else{
            this->schedule(schedule_selector(VipStoryScene::logic), .1f);
            
            if (oneBool) {
                this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::creatManEyesAnimation), .1f);
            }else if (twoBool){
                this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::toPassersbyEyesAnimation), .1f);
            }
        }
    }else if (dialItem->getVipStates() == 4){// ==4 只有背景图片
//        if (this->getChildByTag(Tag_GJ_kuang) != NULL) {
//            this->removeChildByTag(Tag_GJ_kuang);
//        }
        if (this->getChildByTag(0x88888) != NULL) {
            this->removeChildByTag(0x88888);
        }
        
        manString1 = "";
        manString2 = "";
        manBool = false;
        passersbyBool1 = false;
        passersbyBool2 = false;
        
        oneBool = false;
        twoBool = false;
        
        this->removeMan();
        this->outKuang();
        saidLabel->setString("");
        nameLab->setString("");
        nameKuang->setVisible(false);
        
        this->creatBg();
    }
}

void VipStoryScene::recordLabel(VipDialogItem* dialItem){
    if (dialItem->getVipName().length() != 0) {
        labStr.append(dialItem->getVipName().c_str());
        labStr.append("\n\t");
    }
    if (dialItem->getVipSaid().length() != 0) {
        labStr.append(dialItem->getVipSaid().c_str());
        labStr.append("\n");
    }
    
    nowIndex++;
    if (nowIndex == 4) {
        nowIndex = 0;
//        char* labChar = strdup(labStr.c_str());
        
        CCString* str = CCString::createWithFormat("%s", labStr.c_str());
        DATA->getStoryLabelArr()->addObject(str);
        labStr = "";
    }
}

void VipStoryScene::creatMan(float dt){
    
    if (this->getChildByTag(Tag_GJ_man1) != NULL) {
        this->removeChildByTag(Tag_GJ_man1);
    }
    if (this->getChildByTag(Tag_GJ_head1) != NULL) {
        this->removeChildByTag(Tag_GJ_head1);
    }
    
    std::string str1 = vipDialogItem->getVipZishi_1();
    std::string str2 = vipDialogItem->getVipHead_1();
    
    if (!str1.empty() && !str2.empty()) {
        //缩小状态坐标
        CCString* zsStr1 = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", vipDialogItem->getVipZishi_1().c_str());
        manSpr1 = CCSprite::create(zsStr1->getCString());
        manSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        manSpr1->setTag(Tag_GJ_man1);
//        manSpr1->setScale(.75f);
        manSpr1->setVisible(false);
        this->addChild(manSpr1, 5);
        
        CCString* tStr1 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", vipDialogItem->getVipHead_1().c_str());
        headSpr1 = CCSprite::create(tStr1->getCString());
        headSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        headSpr1->setTag(Tag_GJ_head1);
//        headSpr1->setScale(.75f);
        headSpr1->setVisible(false);
        this->addChild(headSpr1, 6);
        
        if (manString2.empty()) {
            if (manString1.empty()) {
                manString1 = vipDialogItem->getVipZishi_1();
                
                CCFadeIn* fadeIn1 = CCFadeIn::create(inTime);
                CCFadeIn* fadeIn2 = CCFadeIn::create(inTime);
                manSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn1, NULL));
                headSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn2, NULL));
                
                this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::creatManEyesAnimation), inTime);
            }else{
                if (manString1 == vipDialogItem->getVipZishi_1()) {
                    manSpr1->setVisible(true);
                    headSpr1->setVisible(true);
                    
                    this->creatManEyesAnimation();
                }else{
                    manString1 = vipDialogItem->getVipZishi_1();
                    
                    CCFadeIn* fadeIn1 = CCFadeIn::create(inTime);
                    CCFadeIn* fadeIn2 = CCFadeIn::create(inTime);
                    manSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn1, NULL));
                    headSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn2, NULL));
                    
                    this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::creatManEyesAnimation), inTime);
                }
            }
        }else{
            manString1 = "";
            manString2 = "";
            manString1 = vipDialogItem->getVipZishi_1();
            
            CCFadeIn* fadeIn1 = CCFadeIn::create(inTime);
            CCFadeIn* fadeIn2 = CCFadeIn::create(inTime);
            manSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn1, NULL));
            headSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn2, NULL));
            
            this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::creatManEyesAnimation), inTime);
        }
    }
}

void VipStoryScene::creatPassersbyMan(float dt){
    
    this->removeMan();
    
    if (vipDialogItem->getVipPassersby() == 1) {
        
        std::string str1 = vipDialogItem->getVipZishi_1();
        std::string str2 = vipDialogItem->getVipHead_1();
        std::string str3 = vipDialogItem->getVipZishi_2();
        std::string str4 = vipDialogItem->getVipHead_2();
        if (!str1.empty() && !str2.empty() && !str3.empty() && !str4.empty()) {
            //1号 一般大的
            CCString* zsStr1 = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", vipDialogItem->getVipZishi_1().c_str());
            manSpr1 = CCSprite::create(zsStr1->getCString());
            manSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 - 150, DISPLAY->ScreenHeight()* .5f));
//            manSpr1->setScale(.7f);
            manSpr1->setTag(Tag_GJ_man1);
            manSpr1->setVisible(false);
            this->addChild(manSpr1, 6);
            
            CCString* tStr1 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", vipDialogItem->getVipHead_1().c_str());
            headSpr1 = CCSprite::create(tStr1->getCString());
            headSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 - 150, DISPLAY->ScreenHeight()* .5f));
//            headSpr1->setScale(.7f);
            headSpr1->setTag(Tag_GJ_head1);
            headSpr1->setVisible(false);
            this->addChild(headSpr1, 7);
            
            //2号 一般大的
            CCString* zsStr2 = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", vipDialogItem->getVipZishi_2().c_str());
            manSpr2 = CCSprite::create(zsStr2->getCString());
            manSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 + 200, DISPLAY->ScreenHeight()* .5f));
//            manSpr2->setScale(.7f);
            manSpr2->setTag(Tag_GJ_man2);
            manSpr2->setVisible(false);
            this->addChild(manSpr2, 6);
            
            CCString* tStr2 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", vipDialogItem->getVipHead_2().c_str());
            headSpr2 = CCSprite::create(tStr2->getCString());
            headSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 + 200, DISPLAY->ScreenHeight()* .5f));
//            headSpr2->setScale(.7f);
            headSpr2->setTag(Tag_GJ_head2);
            headSpr2->setVisible(false);
            this->addChild(headSpr2, 7);
            
            if (manString2.empty()) {
                manString1 = vipDialogItem->getVipZishi_1();
                manString2 = vipDialogItem->getVipZishi_2();
                
                CCFadeIn* fadeIn1 = CCFadeIn::create(inTime);
                CCFadeIn* fadeIn2 = CCFadeIn::create(inTime);
                manSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn1, NULL));
                headSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn2, NULL));
                
                CCFadeIn* fadeIn3 = CCFadeIn::create(inTime);
                CCFadeIn* fadeIn4 = CCFadeIn::create(inTime);
                manSpr2->runAction(CCSpawn::create(CCShow::create(), fadeIn3, NULL));
                headSpr2->runAction(CCSpawn::create(CCShow::create(), fadeIn4, NULL));
                
                this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::toPassersbyEyesAnimation), inTime);
                
            }else{
                
                if (manString1 == vipDialogItem->getVipZishi_1()) {
                    manSpr1->setVisible(true);
                    headSpr1->setVisible(true);
                    
                    this->creatPassersbyEyesAnimation(1);
                }else{
                    manString1 = vipDialogItem->getVipZishi_1();
                    
                    CCFadeIn* fadeIn1 = CCFadeIn::create(inTime);
                    CCFadeIn* fadeIn2 = CCFadeIn::create(inTime);
                    manSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn1, NULL));
                    headSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn2, NULL));
                    
                    this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::toPassersbyEyesAnimation1), inTime);
                }
                
                if (manString2 == vipDialogItem->getVipZishi_2()) {
                    manSpr2->setVisible(true);
                    headSpr2->setVisible(true);
                    
                    this->creatPassersbyEyesAnimation(2);
                }else{
                    manString2 = vipDialogItem->getVipZishi_2();
                    
                    CCFadeIn* fadeIn3 = CCFadeIn::create(inTime);
                    CCFadeIn* fadeIn4 = CCFadeIn::create(inTime);
                    manSpr2->runAction(CCSpawn::create(CCShow::create(), fadeIn3, NULL));
                    headSpr2->runAction(CCSpawn::create(CCShow::create(), fadeIn4, NULL));
                    
                    this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::toPassersbyEyesAnimation2), inTime);
                }
            }
        }
    }else if (vipDialogItem->getVipPassersby() == 0){
        
        if (vipDialogItem->getVipHead_1().size() > 1) {
            std::string str1 = vipDialogItem->getVipZishi_1();
            std::string str2 = vipDialogItem->getVipHead_1();
            if (!str1.empty() && !str2.empty()) {
                CCString* zsStr1 = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", vipDialogItem->getVipZishi_1().c_str());
                manSpr1 = CCSprite::create(zsStr1->getCString());
                manSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 - 150, DISPLAY->ScreenHeight()* .5f));
//                manSpr1->setScale(.7f);
                manSpr1->setTag(Tag_GJ_man1);
                manSpr1->setVisible(false);
                this->addChild(manSpr1, 5);
                
                CCString* tStr1 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", vipDialogItem->getVipHead_1().c_str());
                headSpr1 = CCSprite::create(tStr1->getCString());
                headSpr1->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 - 150, DISPLAY->ScreenHeight()* .5f));
//                headSpr1->setScale(.7f);
                headSpr1->setTag(Tag_GJ_head1);
                headSpr1->setVisible(false);
                this->addChild(headSpr1, 6);
                
                if (manString1 == vipDialogItem->getVipZishi_1()) {
                    manSpr1->setVisible(true);
                    headSpr1->setVisible(true);
                    
                    this->creatPassersbyEyesAnimation(1);
                }else{
                    manString1 = vipDialogItem->getVipZishi_1();
                    
                    CCFadeIn* fadeIn1 = CCFadeIn::create(inTime);
                    CCFadeIn* fadeIn2 = CCFadeIn::create(inTime);
                    manSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn1, NULL));
                    headSpr1->runAction(CCSpawn::create(CCShow::create(), fadeIn2, NULL));
                    
                    this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::toPassersbyEyesAnimation1), inTime);
                }
            }
        }else{
            manString1 = "";
        }
        
        if (vipDialogItem->getVipHead_2().size() > 1) {
            std::string str3 = vipDialogItem->getVipZishi_2();
            std::string str4 = vipDialogItem->getVipHead_2();
            if (!str3.empty() && !str4.empty()) {
                CCString* zsStr2 = CCString::createWithFormat("res/pic/qingjingScene/zishi/%s.png", vipDialogItem->getVipZishi_2().c_str());
                manSpr2 = CCSprite::create(zsStr2->getCString());
                manSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 + 200, DISPLAY->ScreenHeight()* .5f));
//                manSpr2->setScale(.7f);
                manSpr2->setTag(Tag_GJ_man2);
                manSpr2->setVisible(false);
                this->addChild(manSpr2, 5);
                
                CCString* tStr2 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", vipDialogItem->getVipHead_2().c_str());
                headSpr2 = CCSprite::create(tStr2->getCString());
                headSpr2->setPosition(ccp(DISPLAY->ScreenWidth()* 0.5 + 200, DISPLAY->ScreenHeight()* .5f));
//                headSpr2->setScale(.7f);
                headSpr2->setTag(Tag_GJ_head2);
                headSpr2->setVisible(false);
                this->addChild(headSpr2, 6);
                
                if (manString2 == vipDialogItem->getVipZishi_2()) {
                    manSpr2->setVisible(true);
                    headSpr2->setVisible(true);
                    
                    this->creatPassersbyEyesAnimation(2);
                }else{
                    manString2 = vipDialogItem->getVipZishi_2();
                    
                    CCFadeIn* fadeIn3 = CCFadeIn::create(inTime);
                    CCFadeIn* fadeIn4 = CCFadeIn::create(inTime);
                    manSpr2->runAction(CCSpawn::create(CCShow::create(), fadeIn3, NULL));
                    headSpr2->runAction(CCSpawn::create(CCShow::create(), fadeIn4, NULL));
                    
                    this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::toPassersbyEyesAnimation2), inTime);
                }
            }
        }else{
            manString2 = "";
        }
    }
}
void VipStoryScene::creatManEyesAnimation(){
    
    if (ccs(vipDialogItem->getVipNameId())->intValue() != 0) {
        std::string::size_type idx = vipDialogItem->getVipZishi_1().find(vipDialogItem->getVipNameId());
        if (idx != string::npos) {
//            CCLog("字符串含有");
            std::string::size_type idx1 = vipDialogItem->getVipHead_1().find("_1");
            if (idx1 != string::npos) {
                manBool = true;
                
                CCArray* animations = CCArray::createWithCapacity(4);
                char strPei[100] = {};
                for (int i = 1; i <= 2; i++) {
                    
                    sprintf(strPei, "res/pic/qingjingScene/eyes/%d_%d.png", ccs(vipDialogItem->getVipNameId())->intValue(), i);
                    CCSpriteFrame *frame = CCSpriteFrame::create(strPei,CCRectMake(0, 0, 900, 1136));
                    animations->addObject(frame);
                }
                CCAnimation* animation = CCAnimation::createWithSpriteFrames(animations, .2f);
                CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "manStr");
                
                this->creatManBlinkEyes();
            }else{
                manBool = false;
            }
        }
    }
}
void VipStoryScene::creatManBlinkEyes(){
    CCAnimation* manAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("manStr");
    CCAnimate* animate = CCAnimate::create(manAnimation);
    CCRepeatForever* rep = CCRepeatForever::create(animate);
    
    CCDirector::sharedDirector()->getActionManager()->addAction(rep, headSpr1, false);
}
void VipStoryScene::toPassersbyEyesAnimation(){
    this->creatPassersbyEyesAnimation(1);
    this->creatPassersbyEyesAnimation(2);
}
void VipStoryScene::toPassersbyEyesAnimation1(){
    this->creatPassersbyEyesAnimation(1);
}
void VipStoryScene::toPassersbyEyesAnimation2(){
    this->creatPassersbyEyesAnimation(2);
}
void VipStoryScene::creatPassersbyEyesAnimation(int nameIndex){
    
    if (ccs(vipDialogItem->getVipNameId())->intValue() != 0) {
        std::string::size_type idx;
        if (nameIndex == 1) {
            idx = vipDialogItem->getVipZishi_1().find(vipDialogItem->getVipNameId());
            if (idx != string::npos) {
                std::string::size_type idx1 = vipDialogItem->getVipHead_1().find("_1");
                if (idx1 != string::npos) {
                    passersbyBool1 = true;
                }else{
                    passersbyBool1 = false;
                }
            }
        }else if (nameIndex == 2){
            idx = vipDialogItem->getVipZishi_2().find(vipDialogItem->getVipNameId());
            if (idx != string::npos) {
                std::string::size_type idx1 = vipDialogItem->getVipHead_2().find("_1");
                if (idx1 != string::npos) {
                    passersbyBool2 = true;
                }else{
                    passersbyBool2 = false;
                }
            }
        }
        CCArray* animations = CCArray::createWithCapacity(4);
        char strPei[100] = {};
        for (int i = 1; i <= 2; i++) {
            
            sprintf(strPei, "res/pic/qingjingScene/eyes/%d_%d.png", ccs(vipDialogItem->getVipNameId())->intValue(), i);
            CCSpriteFrame *frame = CCSpriteFrame::create(strPei,CCRectMake(0, 0, 900, 1136));
            animations->addObject(frame);
        }
        CCAnimation* animation = CCAnimation::createWithSpriteFrames(animations, .2f);
        if (passersbyBool1) {
            passersbyBool2 = false;
            CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "passersbyStr1");
            this->creatPassersbyBlinkEyes();
        }else if (passersbyBool2){
            passersbyBool1 = false;
            CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "passersbyStr2");
            this->creatPassersbyBlinkEyes();
        }
    }
}
void VipStoryScene::creatPassersbyBlinkEyes(){
    if (passersbyBool1) {
        CCAnimation* manAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("passersbyStr1");
        CCAnimate* animate = CCAnimate::create(manAnimation);
        CCRepeatForever* rep = CCRepeatForever::create(animate);
        
        if (headSpr1 != NULL) {
            std::string::size_type idx = vipDialogItem->getVipZishi_1().find(vipDialogItem->getVipNameId());
            if (idx != string::npos) {
                std::string::size_type idx1 = vipDialogItem->getVipHead_1().find("_1");
                if (idx1 != string::npos) {
                    CCDirector::sharedDirector()->getActionManager()->addAction(rep, headSpr1, false);
                }
            }
            
        }
        
    }else if (passersbyBool2){
        CCAnimation* manAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("passersbyStr2");
        CCAnimate* animate = CCAnimate::create(manAnimation);
        CCRepeatForever* rep = CCRepeatForever::create(animate);
        
        if (headSpr2 != NULL) {
            std::string::size_type idx = vipDialogItem->getVipZishi_2().find(vipDialogItem->getVipNameId());
            if (idx != string::npos) {
                std::string::size_type idx1 = vipDialogItem->getVipHead_2().find("_1");
                if (idx1 != string::npos) {
                    CCDirector::sharedDirector()->getActionManager()->addAction(rep, headSpr2, false);
                }
            }
        }
        
    }
}
void VipStoryScene::closeEyesAnimation(){
    
    if (manBool) {
        manBool = false;
        
        if (headSpr1 != NULL) {
            CCDirector::sharedDirector()->getActionManager()->removeAllActionsFromTarget(headSpr1);
            
            CCString* tStr1 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", vipDialogItem->getVipHead_1().c_str());
            CCTexture2D* aTexture = CCTextureCache::sharedTextureCache()->addImage(tStr1->getCString());
            CCRect rect = CCRectZero;
            rect.size = aTexture->getContentSize();
            headSpr1->setTexture(aTexture);
            headSpr1->setTextureRect(rect);
            headSpr1->setVisible(true);
        }
        
    }else if (passersbyBool1){
        passersbyBool1 = false;
        
        if (headSpr1 != NULL) {
            CCDirector::sharedDirector()->getActionManager()->removeAllActionsFromTarget(headSpr1);
            
            CCString* tStr1 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", vipDialogItem->getVipHead_1().c_str());
            CCTexture2D* aTexture = CCTextureCache::sharedTextureCache()->addImage(tStr1->getCString());
            CCRect rect = CCRectZero;
            rect.size = aTexture->getContentSize();
            headSpr1->setTexture(aTexture);
            headSpr1->setTextureRect(rect);
            headSpr1->setVisible(true);
        }
        
    }else if (passersbyBool2){
        passersbyBool2 = false;
        
        if (headSpr2 != NULL) {
            CCDirector::sharedDirector()->getActionManager()->removeAllActionsFromTarget(headSpr2);
            
            CCString* tStr1 = CCString::createWithFormat("res/pic/qingjingScene/head/%s.png", vipDialogItem->getVipHead_2().c_str());
            CCTexture2D* aTexture = CCTextureCache::sharedTextureCache()->addImage(tStr1->getCString());
            CCRect rect = CCRectZero;
            rect.size = aTexture->getContentSize();
            headSpr2->setTexture(aTexture);
            headSpr2->setTextureRect(rect);
            headSpr2->setVisible(true);
        }
        
    }
}

void VipStoryScene::addKuang(float dt){
//    if (this->getChildByTag(Tag_GJ_kuang) != NULL) {
//        this->removeChildByTag(Tag_GJ_kuang);
//    }
    _dikuangBool = true;
    _dkSpr->setVisible(true);
    CCFiniteTimeAction* _actMove1 = CCMoveBy::create(.3f, ccp(0, 535));
    CCFiniteTimeAction* _actMove2 = CCMoveBy::create(.1f, ccp(0, -35));
    CCCallFuncN* complete = CCCallFuncN::create(this, callfuncN_selector(VipStoryScene::addComplete));
    _dkSpr->runAction(CCSequence::create(_actMove1, _actMove2, CCDelayTime::create(.3f), complete, NULL));
    
    this->schedule(schedule_selector(VipStoryScene::logic), 1.5f);
}
void VipStoryScene::addComplete(){
    dikuangBool = true;
}
void VipStoryScene::outKuang(){
    if (_dikuangBool) {
        _dikuangBool = false;
        _dkSpr->setVisible(false);
        CCFiniteTimeAction* _actMove = CCMoveBy::create(.3f, ccp(0, -500));
        _dkSpr->runAction(CCSequence::create(_actMove, NULL));
    }
}

void VipStoryScene::logic(float dt){
    
    openStory = false;
    
    this->unschedule(schedule_selector(VipStoryScene::logic));
    
    if (!buttonBool1 && !buttonBool2) {
        this->openTouch(0);
    }
    
    if (vipDialogItem->getVipName().length() <= 0) {
        nameKuang->setVisible(false);
    }else{
        nameKuang->setVisible(true);
    }
    nameLab->setString(vipDialogItem->getVipName().c_str());
    
    if (wordCount > contentLength) {
        wordCount++;
        
        saidLabel->setString(getContentByLength(wordCount).c_str());
        
        this->closeEyesAnimation();
        
        if (!quanBool) {
            quanBool = true;
            
            quanSpr->setVisible(true);
        }
        
        if (buttonBool3) {
            this->setTouchEnabled(false);
            
            this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::getIndex), 1.f);
        }
        
        return;
    }else{
        wordCount++;
        
        saidLabel->setString(getContentByLength(wordCount).c_str());
        
        if (buttonBool1) {
            wordCount = getContentLength();
            saidLabel->setString(getContentByLength(wordCount).c_str());
            
            this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::getIndex), .1f);
        }
        
        this->schedule(schedule_selector(VipStoryScene::logic), .1f);
    }
}

void VipStoryScene::creatBg(){
    this->setTouchEnabled(false);
    
    if (this->getChildByTag(Tag_GJ_bg) != NULL) {
        this->removeChildByTag(Tag_GJ_bg);
    }
    
    std::string str = vipDialogItem->getVipBg();
    if (!str.empty()) {
        CCString * CBg = CCString::createWithFormat("res/pic/qingjingScene/bgimage/%s", vipDialogItem->getVipBg().c_str());
        bgSpr = CCSprite::create(CBg->getCString());
        bgSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        bgSpr->setTag(Tag_GJ_bg);
        bgSpr->setOpacity(0);
        this->addChild(bgSpr, 3);
        
        if (vipDialogItem->getVipStates() != 4) {
            //淡入
            CCFiniteTimeAction* _bgAction = CCFadeIn::create(1.5f);
            bgSpr->runAction(CCSequence::create(_bgAction, NULL));
        }else{
            
            //淡入
            CCFiniteTimeAction* _bgAction = CCFadeIn::create(.3f);
            bgSpr->runAction(CCSequence::create(_bgAction, NULL));
            
            this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::openTouch), 1.f);
        }
    }
}

void VipStoryScene::emptyLabel(){
    
    nameLab->setString("");
    saidLabel->setString(getContentByLength(wordCount).c_str());
}

void VipStoryScene::addButton(){
    // 退出
    CCSprite* fhSpr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_tuichu.png");
    CCSprite* fhSpr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_tuichu.png");
    fhSpr2->setColor(ccGRAY);
    CCMenuItem* fhItem;
    fhItem = CCMenuItemSprite::create(fhSpr1, fhSpr2, this, menu_selector(VipStoryScene::fhCallBack));
    fhItem->setPosition(ccp(kuangSpr->boundingBox().size.width* .1f, 43 - 500));
    
    // 回顾
    CCSprite* hgSpr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_huikan.png");
    CCSprite* hgSpr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_huikan.png");
    hgSpr2->setColor(ccGRAY);
    CCMenuItem* hkButton = CCMenuItemSprite::create(hgSpr1, hgSpr2, this, menu_selector(VipStoryScene::button2CallBack));
    hkButton->setPosition(ccp(kuangSpr->getContentSize().width* .61, kuangSpr->getContentSize().height + 80 - 500));
    
    
    // 自动
    CCSprite* zdSpr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_zidong1.png");
    CCSprite* zdSpr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_zidong1.png");
    zdSpr2->setColor(ccGRAY);
    CCSprite* ztSpr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_zidong2.png");
    CCSprite* ztSpr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_zidong2.png");
    ztSpr2->setColor(ccGRAY);
    CCMenuItem* zidongItemOn = CCMenuItemSprite::create(zdSpr1, zdSpr1);
    CCMenuItem* zidongItemOff = CCMenuItemSprite::create(ztSpr1, ztSpr2);
    zidongToggleItem = CCMenuItemToggle::createWithTarget(this, menu_selector(VipStoryScene::button3CallBack), zidongItemOn,zidongItemOff,NULL);
    zidongToggleItem->setPosition(ccp(kuangSpr->getContentSize().width* .76, kuangSpr->getContentSize().height + 80 - 500));
    zidongToggleItem->setTag(Tag_zidong);
    m_bIsZDSelect = recordBool3;
    if (m_bIsZDSelect) {
        zidongToggleItem->setSelectedIndex(1);
    }else{
        zidongToggleItem->setSelectedIndex(0);
    }
    
    // 快进
    CCSprite* kjSpr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_kuaijin1.png");
    CCSprite* kjSpr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_kuaijin1.png");
    kjSpr2->setColor(ccGRAY);
    CCSprite* ztSpr3 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_kuaijin2.png");
    CCSprite* ztSpr4 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_kuaijin2.png");
    ztSpr4->setColor(ccGRAY);
    CCMenuItem* kuaijinItemOn = CCMenuItemSprite::create(kjSpr1, kjSpr2);
    CCMenuItem* kuaijinItemOff = CCMenuItemSprite::create(ztSpr3, ztSpr4);
    kuaijinToggleItem = CCMenuItemToggle::createWithTarget(this, menu_selector(VipStoryScene::button1CallBack), kuaijinItemOn,kuaijinItemOff,NULL);
    kuaijinToggleItem->setPosition(ccp(kuangSpr->getContentSize().width* .91, kuangSpr->getContentSize().height + 80 - 500));
    CCPoint pos = ccp(kuaijinToggleItem->getPosition().x, kuaijinToggleItem->getPosition().y);
    
    m_bIsKJSelect = recordBool1;
    if (DATA->getFastForward()) {
        if (m_bIsKJSelect) {
            kuaijinToggleItem->setSelectedIndex(1);
        }else{
            kuaijinToggleItem->setSelectedIndex(0);
        }
    }else{
        kuaijinToggleItem->setSelectedIndex(0);
    }
    
    
    CCMenu* menu = CCMenu::create(fhItem, kuaijinToggleItem, hkButton, zidongToggleItem, NULL);
    menu->setPosition(CCPointZero);
    menu->setTag(0x88888);
    _dkSpr->addChild(menu, 15);
}

void VipStoryScene::button1CallBack(CCObject* pSender){
    AUDIO->common_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "vip情景界面", "点击快进");
    
    if (DATA->getFastForward()) {
        
        if (!buttonBool1) {
            buttonBool1 = true;
            buttonBool2 = false;
            buttonBool3 = false;
            recordBool1 = buttonBool1;
            recordBool2 = buttonBool2;
            recordBool3 = buttonBool3;
            
            this->setTouchEnabled(false);
            
            kuaijinToggleItem->setSelectedIndex(1);
            zidongToggleItem->setSelectedIndex(0);
            
            
            // talkingData
//            MMDataManager::get_instance()->onEvent("点击事件", "星途界面", "开启快进");
            
            wordCount = getContentLength();
            this->unschedule(schedule_selector(VipStoryScene::logic));
            this->schedule(schedule_selector(VipStoryScene::logic), .1f);
            
        }else{
            buttonBool1 = false;
            buttonBool2 = false;
            buttonBool3 = false;
            recordBool1 = buttonBool1;
            recordBool2 = buttonBool2;
            recordBool3 = buttonBool3;
            
            this->setTouchEnabled(true);
            
            kuaijinToggleItem->setSelectedIndex(0);
            zidongToggleItem->setSelectedIndex(0);
        }
    }else{
//        if (MMAudioManager::get_instance()->is_effect_on()) {
//            MMAudioManager::get_instance()->play_effect(kAudio_Button_errorCommon, false);
//        }
        
//        promptLayer->promptBox(31);
        
        buttonBool1 = false;
        recordBool1 = buttonBool1;
        
        this->setTouchEnabled(true);
        
        kuaijinToggleItem->setSelectedIndex(0);
    }
    
}
void VipStoryScene::button2CallBack(CCObject* pSender){
    AUDIO->common_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "vip情景界面", "点击回顾");
    
    if (!buttonBool2) {
        buttonBool2 = true;
        buttonBool1 = false;
        buttonBool3 = false;
        recordBool1 = buttonBool1;
        recordBool2 = buttonBool2;
        recordBool3 = buttonBool3;
        
        this->setTouchEnabled(false);
        
        kuaijinToggleItem->setSelectedIndex(0);
        zidongToggleItem->setSelectedIndex(0);
        
        // talkingData
//        MMDataManager::get_instance()->onEvent("点击事件", "星途界面", "开启日志");
        
        VipLabelColorLayer* layer = VipLabelColorLayer::create_with_index(labStr.c_str());
        layer->setTag(0x999999);
        this->addChild(layer, 100);
        
        
    }else{
        buttonBool1 = false;
        buttonBool2 = false;
        buttonBool3 = false;
        recordBool1 = buttonBool1;
        recordBool2 = buttonBool2;
        recordBool3 = buttonBool3;
        
        this->setTouchEnabled(true);
        
        kuaijinToggleItem->setSelectedIndex(0);
        zidongToggleItem->setSelectedIndex(0);
    }
}


void VipStoryScene::button3CallBack(CCObject* pSender){
    AUDIO->common_effect();
    
    // talkingData
    DATA->onEvent("点击事件", "vip情景界面", "点击自动");
    
    if (!buttonBool3) {
        buttonBool3 = true;
        buttonBool1 = false;
        buttonBool2 = false;
        recordBool1 = buttonBool1;
        recordBool2 = buttonBool2;
        recordBool3 = buttonBool3;
        
        kuaijinToggleItem->setSelectedIndex(0);
        zidongToggleItem->setSelectedIndex(1);
        
        // talkingData
//        MMDataManager::get_instance()->onEvent("点击事件", "星途界面", "开启播放");
        
        this->unschedule(schedule_selector(VipStoryScene::logic));
        this->schedule(schedule_selector(VipStoryScene::logic), .1f);
    }else{
        buttonBool1 = false;
        buttonBool2 = false;
        buttonBool3 = false;
        recordBool1 = buttonBool1;
        recordBool2 = buttonBool2;
        recordBool3 = buttonBool3;
        
        kuaijinToggleItem->setSelectedIndex(0);
        zidongToggleItem->setSelectedIndex(0);
    }
}

void VipStoryScene::initButton(){
    CCArray* arr = vipDialogItem->getVipArray();
    int nextIndex;
    CCString* str;
    
    recordBool1 = buttonBool1;
//    MZLog("recordBool1 === %d", recordBool1);
    recordBool2 = buttonBool2;
//    MZLog("recordBool2 === %d", recordBool2);
    recordBool3 = buttonBool3;
//    MZLog("recordBool3 === %d", recordBool3);
    buttonBool1 = false;
    buttonBool2 = false;
    buttonBool3 = false;
    
    for (int i = 0; i < arr->count(); i++){
        CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(i);
        nextIndex = dic->valueForKey("next")->intValue();
        str = (CCString* )dic->objectForKey("dialog");
//        CCLog("%s == %d", str->getCString(), nextIndex);
        
        this->creatButton(i);
        this->initLabel(i);
        this->menuEffect(i);
    }
}

void VipStoryScene::creatButton(int dex){
    buttonBool = true;
    
    if (dex == 0) {
        // 任务条
        CCSprite* spr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_rwtiao.png");
        CCSprite* spr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_rwtiao.png");
        CCMenuItem* item1 = CCMenuItemSprite::create(spr1, spr2, this, menu_selector(VipStoryScene::callBackMethods));
        item1->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .59f));
        item1->setTag(Tag_Item_0);
        CCMenu* menu1 = CCMenu::create(item1, NULL);
        menu1->setPosition(CCPointMake(0, 0));
        menu1->setTag(Tag_Menu_0);
        menu1->setVisible(true);
        this->addChild(menu1, 20);
        
    }else if (dex == 1){
        CCSprite* spr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_rwtiao.png");
        CCSprite* spr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_rwtiao.png");
        CCMenuItem* item2 = CCMenuItemSprite::create(spr1, spr2, this, menu_selector(VipStoryScene::callBackMethods));
        item2->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        item2->setTag(Tag_Item_1);
        CCMenu* menu2 = CCMenu::create(item2, NULL);
        menu2->setPosition(CCPointMake(0, 0));
        menu2->setTag(Tag_Menu_1);
        menu2->setVisible(true);
        this->addChild(menu2, 20);
        
    }else if (dex == 2){
        CCSprite* spr1 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_rwtiao.png");
        CCSprite* spr2 = CCSprite::create("res/pic/qingjingScene/storyscene/qj_rwtiao.png");
        CCMenuItem* item3 = CCMenuItemSprite::create(spr1, spr2, this, menu_selector(VipStoryScene::callBackMethods));
        item3->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .41f));
        item3->setTag(Tag_Item_2);
        CCMenu* menu3 = CCMenu::create(item3, NULL);
        menu3->setPosition(CCPointMake(0, 0));
        menu3->setTag(Tag_Menu_2);
        menu3->setVisible(true);
        this->addChild(menu3, 20);
    }
}
void VipStoryScene::initLabel(int dex){
//    CCArray* arr = dialogItem->getArray();
//    CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(dex);
//    dialogItem = (MMDialogItem* )dialog->getDialogs()->objectAtIndex(index);
    CCArray* arr = vipDialogItem->getVipArray();
    CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(dex);
    int nextIndex = dic->valueForKey("next")->intValue();
    CCString* str = (CCString* )dic->objectForKey("dialog");
//    CCLog("%s == %d", str->getCString(), nextIndex);
    
//    CCString* str = (CCString* )dic->objectForKey("dialog");
//    MZLog("====== str->length()%d ===== %d", dex, str->length());
//    CCLog("label ==== %s", str->getCString());
    
    CCLabelTTF* lab;
    if (str->length() > 75) {
        lab = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 28.f);
    }else if (str->length() > 85){
        lab = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 25.f);
    }
    else{
        lab = CCLabelTTF::create(str->getCString(), DISPLAY->fangzhengFont(), 34.f);
    }
    
    if (dex == 0){
        lab->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .59f));
        lab->setTag(Tag_Label_0);
    }else if (dex == 1){
        lab->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f));
        lab->setTag(Tag_Label_1);
    }else if (dex == 2){
        lab->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .41f));
        lab->setTag(Tag_Label_2);
    }
    lab->setColor(ccWHITE);
//    lab->enableStroke(ccWHITE, .4f);
    lab->setVisible(true);
    this->addChild(lab, 21);
}

void VipStoryScene::menuEffect(int dex){
    
    float actTime1 = 0.5f;
    float actTime2 = 0.6f;
    float actTime3 = 0.7f;
    if (dex == 0) {
        CCNode* node1 = this->getChildByTag(Tag_Menu_0);
        CCNode* labNode1 = this->getChildByTag(Tag_Label_0);
        if (node1 != NULL && labNode1 != NULL) {
            node1->setPositionX(-900);
            node1->runAction(CCSequence::create(CCMoveTo::create(actTime1, CCPointMake(0, 0)), NULL));
            
            labNode1->setPositionX(-900);
            labNode1->runAction(CCSequence::create(CCMoveTo::create(actTime1, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .59f)), NULL));
        }
        
    }else if (dex == 1){
        CCNode* node2 = this->getChildByTag(Tag_Menu_1);
        CCNode* labNode2 = this->getChildByTag(Tag_Label_1);
        if (node2 != NULL && labNode2 != NULL) {
            node2->setPositionX(-900);
            node2->runAction(CCSequence::create(CCMoveTo::create(actTime2, CCPointMake(0, 0)), NULL));
            
            labNode2->setPositionX(-900);
            labNode2->runAction(CCSequence::create(CCMoveTo::create(actTime2, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .5f)), NULL));
        }
        
    }else if (dex == 2){
        CCNode* node3 = this->getChildByTag(Tag_Menu_2);
        CCNode* labNode3 = this->getChildByTag(Tag_Label_2);
        if (node3 != NULL && labNode3 != NULL) {
            node3->setPositionX(-900);
            node3->runAction(CCSequence::create(CCMoveTo::create(actTime3, CCPointMake(0, 0)), NULL));
            
            labNode3->setPositionX(-900);
            labNode3->runAction(CCSequence::create(CCMoveTo::create(actTime3, ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .41f)), NULL));
        }
    }
    
    // 添加点击引导
//    if(MMDataManager::get_instance()->getPlayer_info()->getLevel() < 1)
//    {
//        if(!this->getChildByTag(0x1234)){
//            YDVipStoryScene * layer = YDVipStoryScene::create();
//            this->addChild(layer, 10, 0x1234);
//        }
//    }
}

void VipStoryScene::goCallBackMethods(){
    if(this->getChildByTag(0x1234))
    {
        this->removeChildByTag(0x1234, true);
    }
    
    CCNode* node;
    CCMenuItem* menuItem;
    CCArray* arr = vipDialogItem->getVipArray();
    int numIndex = arr->count();
//    MZLog("numIndex ==== %d", numIndex);
    
    CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(menuItem->getTag() - 30);
    int attribute = dic->valueForKey("attribute")->intValue();
    int attributeValue = dic->valueForKey("attributeValue")->intValue();
//    MZLog("attribute === %d", attribute);
//    MZLog("attributeValue === %d", attributeValue);
    
    float actTime = 0.4f;
    if (buttonBool) {
        buttonBool = false;
        
        CCNode* node1, * node2, * node3, * labNode1, * labNode2, * labNode3;
        if (this->getChildByTag(Tag_Menu_0) != NULL) {
            node1 = this->getChildByTag(Tag_Menu_0);
            labNode1 = this->getChildByTag(Tag_Label_0);
        }
        
        if (this->getChildByTag(Tag_Menu_1) != NULL) {
            node2 = this->getChildByTag(Tag_Menu_1);
            labNode2 = this->getChildByTag(Tag_Label_1);
        }
        
        if (this->getChildByTag(Tag_Menu_2) != NULL) {
            node3 = this->getChildByTag(Tag_Menu_2);
            labNode3 = this->getChildByTag(Tag_Label_2);
        }
        
        switch (menuItem->getTag()) {
            case Tag_Item_0:{
                tagIndex = Tag_Item_0;
                
                if (numIndex >= 1 && numIndex < 3) {
                    CCFiniteTimeAction* _Action2 = CCFadeOut::create(actTime);
                    node2->runAction(CCSequence::create(_Action2, NULL));
                    
                    CCFiniteTimeAction* _labAction2 = CCFadeOut::create(actTime);
                    labNode2->runAction(CCSequence::create(_labAction2, NULL));
                }else if (numIndex >= 1 && numIndex <= 3){
                    CCFiniteTimeAction* _Action2 = CCFadeOut::create(actTime);
                    node2->runAction(CCSequence::create(_Action2, NULL));
                    
                    CCFiniteTimeAction* _labAction2 = CCFadeOut::create(actTime);
                    labNode2->runAction(CCSequence::create(_labAction2, NULL));
                    
                    CCFiniteTimeAction* _Action3 = CCFadeOut::create(actTime);
                    node3->runAction(CCSequence::create(_Action3, NULL));
                    
                    CCFiniteTimeAction* _labAction3 = CCFadeOut::create(actTime);
                    labNode3->runAction(CCSequence::create(_labAction3, NULL));
                    
                }
                
                this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::buttonAction), 0.2f);
            }
                break;
                
            case Tag_Item_1:{
                tagIndex = Tag_Item_1;
                
                if (numIndex >= 1 && numIndex < 3) {
                    CCFiniteTimeAction* _Action1 = CCFadeOut::create(actTime);
                    node1->runAction(CCSequence::create(_Action1, NULL));
                    
                    CCFiniteTimeAction* _labAction1 = CCFadeOut::create(actTime);
                    labNode1->runAction(CCSequence::create(_labAction1, NULL));
                }else if (numIndex >= 1 && numIndex <= 3){
                    CCFiniteTimeAction* _Action1 = CCFadeOut::create(actTime);
                    node1->runAction(CCSequence::create(_Action1, NULL));
                    
                    CCFiniteTimeAction* _labAction1 = CCFadeOut::create(actTime);
                    labNode1->runAction(CCSequence::create(_labAction1, NULL));
                    
                    CCFiniteTimeAction* _Action3 = CCFadeOut::create(actTime);
                    node3->runAction(CCSequence::create(_Action3, NULL));
                    
                    CCFiniteTimeAction* _labAction3 = CCFadeOut::create(actTime);
                    labNode3->runAction(CCSequence::create(_labAction3, NULL));
                }
                
                this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::buttonAction), 0.2f);
                
            }
                break;
                
            case Tag_Item_2:{
                tagIndex = Tag_Item_2;
                
                CCFiniteTimeAction* _Action1 = CCFadeOut::create(actTime);
                node1->runAction(CCSequence::create(_Action1, NULL));
                
                CCFiniteTimeAction* _labAction1 = CCFadeOut::create(actTime);
                labNode1->runAction(CCSequence::create(_labAction1, NULL));
                
                CCFiniteTimeAction* _Action2 = CCFadeOut::create(actTime);
                node2->runAction(CCSequence::create(_Action2, NULL));
                
                CCFiniteTimeAction* _labAction2 = CCFadeOut::create(actTime);
                labNode2->runAction(CCSequence::create(_labAction2, NULL));
                
                this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::buttonAction), 0.2f);
                
            }
                break;
                
            default:
                break;
        }
    }
}
void VipStoryScene::callBackMethods(CCObject* pSender){
    if(this->getChildByTag(0x1234))
    {
        this->removeChildByTag(0x1234, true);
    }
    
    CCMenuItemImage* menuItem = (CCMenuItemImage* )pSender;
    CCArray* arr = vipDialogItem->getVipArray();
    int numIndex = arr->count();
//    MZLog("numIndex ==== %d", numIndex);
    
    CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(menuItem->getTag() - 30);
    int attribute = dic->valueForKey("attribute")->intValue();
    int attributeValue = dic->valueForKey("attributeValue")->intValue();
//    MZLog("attribute === %d", attribute);
//    MZLog("attributeValue === %d", attributeValue);
    
    float actTime = 0.4f;
    if (buttonBool) {
        buttonBool = false;
        
        CCNode* node1, * node2, * node3, * labNode1, * labNode2, * labNode3;
        if (this->getChildByTag(Tag_Menu_0) != NULL) {
            node1 = this->getChildByTag(Tag_Menu_0);
            labNode1 = this->getChildByTag(Tag_Label_0);
        }
        
        if (this->getChildByTag(Tag_Menu_1) != NULL) {
            node2 = this->getChildByTag(Tag_Menu_1);
            labNode2 = this->getChildByTag(Tag_Label_1);
        }
        
        if (this->getChildByTag(Tag_Menu_2) != NULL) {
            node3 = this->getChildByTag(Tag_Menu_2);
            labNode3 = this->getChildByTag(Tag_Label_2);
        }
        
        switch (menuItem->getTag()) {
            case Tag_Item_0:{
                tagIndex = Tag_Item_0;
                
//                if (MMAudioManager::get_instance()->is_effect_on()) {
//                    MMAudioManager::get_instance()->play_effect(kAudio_Button_Common, false);
//                }
                
                if (numIndex >= 1 && numIndex < 3) {
                    CCFiniteTimeAction* _Action2 = CCFadeOut::create(actTime);
                    node2->runAction(CCSequence::create(_Action2, NULL));
                    
                    CCFiniteTimeAction* _labAction2 = CCFadeOut::create(actTime);
                    labNode2->runAction(CCSequence::create(_labAction2, NULL));
                }else if (numIndex >= 1 && numIndex <= 3){
                    CCFiniteTimeAction* _Action2 = CCFadeOut::create(actTime);
                    node2->runAction(CCSequence::create(_Action2, NULL));
                    
                    CCFiniteTimeAction* _labAction2 = CCFadeOut::create(actTime);
                    labNode2->runAction(CCSequence::create(_labAction2, NULL));
                    
                    CCFiniteTimeAction* _Action3 = CCFadeOut::create(actTime);
                    node3->runAction(CCSequence::create(_Action3, NULL));
                    
                    CCFiniteTimeAction* _labAction3 = CCFadeOut::create(actTime);
                    labNode3->runAction(CCSequence::create(_labAction3, NULL));
                    
                }
                
                this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::buttonAction), 0.2f);
            }
                break;
                
            case Tag_Item_1:{
                tagIndex = Tag_Item_1;
                
                if (numIndex >= 1 && numIndex < 3) {
                    CCFiniteTimeAction* _Action1 = CCFadeOut::create(actTime);
                    node1->runAction(CCSequence::create(_Action1, NULL));
                    
                    CCFiniteTimeAction* _labAction1 = CCFadeOut::create(actTime);
                    labNode1->runAction(CCSequence::create(_labAction1, NULL));
                }else if (numIndex >= 1 && numIndex <= 3){
                    CCFiniteTimeAction* _Action1 = CCFadeOut::create(actTime);
                    node1->runAction(CCSequence::create(_Action1, NULL));
                    
                    CCFiniteTimeAction* _labAction1 = CCFadeOut::create(actTime);
                    labNode1->runAction(CCSequence::create(_labAction1, NULL));
                    
                    CCFiniteTimeAction* _Action3 = CCFadeOut::create(actTime);
                    node3->runAction(CCSequence::create(_Action3, NULL));
                    
                    CCFiniteTimeAction* _labAction3 = CCFadeOut::create(actTime);
                    labNode3->runAction(CCSequence::create(_labAction3, NULL));
                }
                
                this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::buttonAction), 0.2f);
                
            }
                break;
                
            case Tag_Item_2:{
                tagIndex = Tag_Item_2;
                
                CCFiniteTimeAction* _Action1 = CCFadeOut::create(actTime);
                node1->runAction(CCSequence::create(_Action1, NULL));
                
                CCFiniteTimeAction* _labAction1 = CCFadeOut::create(actTime);
                labNode1->runAction(CCSequence::create(_labAction1, NULL));
                
                CCFiniteTimeAction* _Action2 = CCFadeOut::create(actTime);
                node2->runAction(CCSequence::create(_Action2, NULL));
                
                CCFiniteTimeAction* _labAction2 = CCFadeOut::create(actTime);
                labNode2->runAction(CCSequence::create(_labAction2, NULL));
                
                this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::buttonAction), 0.2f);
                
            }
                break;
                
            default:
                break;
        }
    }
}

void VipStoryScene::buttonAction(float dt){
    CCNode* node, * labNode;
    _bSpr = CCSprite::create();
    if (tagIndex == Tag_Item_0) {
        node = this->getChildByTag(Tag_Menu_0);
        labNode = this->getChildByTag(Tag_Label_0);
        _bSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .53f));
    }else if (tagIndex == Tag_Item_1){
        node = this->getChildByTag(Tag_Menu_1);
        labNode = this->getChildByTag(Tag_Label_1);
        _bSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .44f));
    }else if (tagIndex == Tag_Item_2){
        node = this->getChildByTag(Tag_Menu_2);
        labNode = this->getChildByTag(Tag_Label_2);
        _bSpr->setPosition(ccp(DISPLAY->ScreenWidth()* .5f, DISPLAY->ScreenHeight()* .36f));
    }
    this->addChild(_bSpr, 9);
    
    this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::buttonOut), 0.6f);
}
void VipStoryScene::buttonOut(float dt){
    CCNode* node, * labNode;
    if (tagIndex == Tag_Item_0) {
        node = this->getChildByTag(Tag_Menu_0);
        labNode = this->getChildByTag(Tag_Label_0);
    }else if (tagIndex == Tag_Item_1){
        node = this->getChildByTag(Tag_Menu_1);
        labNode = this->getChildByTag(Tag_Label_1);
    }else if (tagIndex == Tag_Item_2){
        node = this->getChildByTag(Tag_Menu_2);
        labNode = this->getChildByTag(Tag_Label_2);
    }
    
    CCFiniteTimeAction* _Action2 = CCFadeOut::create(.6f);
    node->runAction(CCSequence::create(_Action2, NULL));
    
    CCFiniteTimeAction* _labAction2 = CCFadeOut::create(.6f);
    labNode->runAction(CCSequence::create(_labAction2, NULL));
    
    removeButton();
    
    if (tagIndex == Tag_Item_0) {
        this->callBack1();
    }else if (tagIndex == Tag_Item_1){
        this->callBack2();
    }else if (tagIndex == Tag_Item_2){
        this->callBack3();
    }
}

void VipStoryScene::removeButton(){
    CCArray* arr = vipDialogItem->getVipArray();
    int nextindex = 0;
    CCDictionary* dic;
    
    if (tagIndex == Tag_Item_0) {
        dic = (CCDictionary*)arr->objectAtIndex(0);
        index = dic->valueForKey("next")->intValue();
        CCString* str = (CCString* )dic->objectForKey("dialog");
//        CCLog("%s == %d", str->getCString(), index);
        nextindex = dic->valueForKey("next")->intValue();
    }else if (tagIndex == Tag_Item_1){
        dic = (CCDictionary*)arr->objectAtIndex(1);
        index = dic->valueForKey("next")->intValue();
        CCString* str = (CCString* )dic->objectForKey("dialog");
//        CCLog("%s == %d", str->getCString(), index);
        nextindex = dic->valueForKey("next")->intValue();
    }else if (tagIndex == Tag_Item_2){
        dic = (CCDictionary*)arr->objectAtIndex(2);
        index = dic->valueForKey("next")->intValue();
        CCString* str = (CCString* )dic->objectForKey("dialog");
//        CCLog("%s == %d", str->getCString(), index);
        nextindex = dic->valueForKey("next")->intValue();
    }
    
    
    openButtonBool = false;
    
//    dialogItem->setArray(CCArray);
    
    if (this->getChildByTag(Tag_Menu_0) != NULL) {
        this->removeChildByTag(Tag_Menu_0);
    }
    if (this->getChildByTag(Tag_Menu_1) != NULL) {
        this->removeChildByTag(Tag_Menu_1);
    }
    if (this->getChildByTag(Tag_Menu_2) != NULL) {
        this->removeChildByTag(Tag_Menu_2);
    }
    
    if (this->getChildByTag(Tag_Label_0) != NULL) {
        this->removeChildByTag(Tag_Label_0);
    }
    if (this->getChildByTag(Tag_Label_1) != NULL) {
        this->removeChildByTag(Tag_Label_1);
    }
    if (this->getChildByTag(Tag_Label_2) != NULL) {
        this->removeChildByTag(Tag_Label_2);
    }
    
    if (this->getChildByTag(Tag_Item_0) != NULL) {
        this->removeChildByTag(Tag_Item_0);
    }
    if (this->getChildByTag(Tag_Item_1) != NULL) {
        this->removeChildByTag(Tag_Item_1);
    }
    if (this->getChildByTag(Tag_Item_2) != NULL) {
        this->removeChildByTag(Tag_Item_2);
    }
}

void VipStoryScene::removeMan(){
    if (this->getChildByTag(Tag_GJ_man1) != NULL) {
        this->removeChildByTag(Tag_GJ_man1);
    }
    if (this->getChildByTag(Tag_GJ_head1) != NULL) {
        this->removeChildByTag(Tag_GJ_head1);
    }
    if (this->getChildByTag(Tag_GJ_man2) != NULL) {
        this->removeChildByTag(Tag_GJ_man2);
    }
    if (this->getChildByTag(Tag_GJ_head2) != NULL) {
        this->removeChildByTag(Tag_GJ_head2);
    }
    
    manSpr1 = NULL;
    manSpr2 = NULL;
    headSpr1 = NULL;
    headSpr2 = NULL;
}

void VipStoryScene::callBack1(){
    
    CCArray* arr = vipDialogItem->getVipArray();
    CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(0);
    
//    buttonBool1 = recordBool1;
    buttonBool1 = false;
    recordBool1 = buttonBool1;
    m_bIsKJSelect = buttonBool1;
    if (DATA->getFastForward()) {
        if (m_bIsKJSelect) {
            kuaijinToggleItem->setSelectedIndex(1);
        }else{
            kuaijinToggleItem->setSelectedIndex(0);
        }
    }else{
        kuaijinToggleItem->setSelectedIndex(0);
    }
//    MZLog("VipStoryScene::callBack1 ******* buttonBool1 === %d", buttonBool1);
    buttonBool2 = recordBool2;
//    MZLog("VipStoryScene::callBack1 ******* buttonBool2 === %d", buttonBool2);
    buttonBool3 = recordBool3;
//    MZLog("VipStoryScene::callBack1 ******* buttonBool3 === %d", buttonBool3);
    
    vipDialogItem = (VipDialogItem* )vipDialog->getVipDialogs()->objectAtIndex(dic->valueForKey("next")->intValue());
    wordCount = 0;
    this->init(vipDialogItem);
    this->dialogueControl(vipDialogItem);
}
void VipStoryScene::callBack2(){
    CCArray* arr = vipDialogItem->getVipArray();
    CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(1);
    
//    buttonBool1 = recordBool1;
    buttonBool1 = false;
    recordBool1 = buttonBool1;
    m_bIsKJSelect = buttonBool1;
    if (DATA->getFastForward()) {
        if (m_bIsKJSelect) {
            kuaijinToggleItem->setSelectedIndex(1);
        }else{
            kuaijinToggleItem->setSelectedIndex(0);
        }
    }else{
        kuaijinToggleItem->setSelectedIndex(0);
    }
//    MZLog("VipStoryScene::callBack2 ******* buttonBool1 === %d", buttonBool1);
    buttonBool2 = recordBool2;
//    MZLog("VipStoryScene::callBack2 ******* buttonBool2 === %d", buttonBool2);
    buttonBool3 = recordBool3;
//    MZLog("VipStoryScene::callBack2 ******* buttonBool3 === %d", buttonBool3);
    
    vipDialogItem = (VipDialogItem* )vipDialog->getVipDialogs()->objectAtIndex(dic->valueForKey("next")->intValue());
    wordCount = 0;
    this->init(vipDialogItem);
    this->dialogueControl(vipDialogItem);
}
void VipStoryScene::callBack3(){
    CCArray* arr = vipDialogItem->getVipArray();
    CCDictionary* dic = (CCDictionary*)arr->objectAtIndex(2);
    
//    buttonBool1 = recordBool1;
    buttonBool1 = false;
    recordBool1 = buttonBool1;
    m_bIsKJSelect = buttonBool1;
    if (DATA->getFastForward()) {
        if (m_bIsKJSelect) {
            kuaijinToggleItem->setSelectedIndex(1);
        }else{
            kuaijinToggleItem->setSelectedIndex(0);
        }
    }else{
        kuaijinToggleItem->setSelectedIndex(0);
    }
//    MZLog("VipStoryScene::callBack3 ******* buttonBool1 === %d", buttonBool1);
    buttonBool2 = recordBool2;
//    MZLog("VipStoryScene::callBack3 ******* buttonBool2 === %d", buttonBool2);
    buttonBool3 = recordBool3;
//    MZLog("VipStoryScene::callBack3 ******* buttonBool3 === %d", buttonBool3);
    
    vipDialogItem = (VipDialogItem* )vipDialog->getVipDialogs()->objectAtIndex(dic->valueForKey("next")->intValue());
    wordCount = 0;
    this->init(vipDialogItem);
    this->dialogueControl(vipDialogItem);
}

void VipStoryScene::gengxin(float dt){
    
    if (bgSpr != NULL) {
        CCPoint pos = bgSpr->getPosition();
        pos.x = DISPLAY->ScreenWidth()* .5f + playerVelocity.x;
        
        bool iPhoneOriPad = CCUserDefault::sharedUserDefault()->getBoolForKey("iPhoneOriPad");
        if (!iPhoneOriPad) {
            if (pos.x < DISPLAY->ScreenWidth()* .5f - 70) {
                pos.x = DISPLAY->ScreenWidth()* .5f - 70;
                playerVelocity = CCPointZero;
            }
            else if (pos.x > DISPLAY->ScreenWidth()* .5f + 70){
                pos.x = DISPLAY->ScreenWidth()* .5f + 70;
                playerVelocity = CCPointZero;
            }
        }else{
            if (pos.x < DISPLAY->ScreenWidth()* .5f - 10) {
                pos.x = DISPLAY->ScreenWidth()* .5f - 10;
                playerVelocity = CCPointZero;
            }
            else if (pos.x > DISPLAY->ScreenWidth()* .5f + 10){
                pos.x = DISPLAY->ScreenWidth()* .5f + 10;
                playerVelocity = CCPointZero;
            }
        }
        bgSpr->runAction(CCMoveTo::create(.1f, ccp(pos.x, pos.y)));
    }
}

void VipStoryScene::init(VipDialogItem* item){
    content = item->getVipSaid();
    contentLength = 0;
    
    int length = content.length();
    int i = 0;
    while (i < length) {
        char ch = getContent()[i];
        
        if (ch > -127 && ch < 0) {
            i += 3;
        }else{
            i++;
        }
        contentLength++;
    }
}

std::string VipStoryScene::getContentByLength(int length){
    if (length >= contentLength) {
        return getContent();
    }
    
    int i = 0;
    int index = 0;
    while (index < length) {
        char ch = getContent()[i];
        if (ch > -127 && ch < 0) {
            i += 3;
        }else{
            i++;
        }
        index++;
    }
    std::string str = content.substr(0, i);
//    MZLog("index ======= %d", index);
    return str;
}
int VipStoryScene::getContentLength(){
    return contentLength;
}

void VipStoryScene::onEnter()
{
    this->setAccelerometerEnabled(true);
    
    BaseScene::onEnter();
    
    BaseScene::hideBaseScene();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&VipStoryScene::_507CallBack), "HTTP_FINISHED_507", NULL);
    
    nc->addObserver(this, SEL_CallFuncO(&VipStoryScene::LabelColorFhCallBack), "LabelColorFhCallBack", NULL);
    nc->addObserver(this, SEL_CallFuncO(&VipStoryScene::goCallBackMethods), "GoCallBackMethods", NULL);
    nc->addObserver(this, SEL_CallFuncO(&VipStoryScene::button1CallBack), "Button1CallBack", NULL);
    
    
    
    // 播放背景音
//    if (MMAudioManager::get_instance()->is_music_on()) {
//        if (MMAudioManager::get_instance()->isBackgroundMusicPlaying()) {
//            MMAudioManager::get_instance()->play_music(kMusic_BG_Qingjing, true);
//        }
//        else {
//            MMAudioManager::get_instance()->play_music(kMusic_BG_Qingjing, true);
//        }
//    }

    this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::keyBackStatus), .8f);
}
void VipStoryScene::keyBackStatus(float dt){
    this->setKeypadEnabled(true);
}

void VipStoryScene::onExit(){
    this->setTouchEnabled(false);
    this->setAccelerometerEnabled(false);
    this->unscheduleAllSelectors();
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->removeAllObservers(this);
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
//    if (MMAudioManager::get_instance()->isBackgroundMusicPlaying()) {
//        bool is = CCUserDefault::sharedUserDefault()->getBoolForKey("play_1", true);
//        if (is) {
//            if (MMAudioManager::get_instance()->isBackgroundMusicPlaying()) {
//                MMAudioManager::get_instance()->play_music(kMusic_BG_main2, true);
//            }
//        } else {
//            if (MMAudioManager::get_instance()->isBackgroundMusicPlaying()) {
//                MMAudioManager::get_instance()->play_music(kMusic_BG_main1, true);
//            }
//        }
//    }
    
    BaseScene::onExit();
}

//void VipStoryScene::registerWithTouchDispatcher(){
//    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -1, true);
//}

bool VipStoryScene::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
//    CCLog("VipStoryScene::ccTouchBegan() ...");
    static int guide_count = 0;
    if (!openStory) {
        logIndex++;
        
        if (buttonBool3) {
            if (logIndex == 1) {
                
                this->unschedule(SEL_SCHEDULE(&VipStoryScene::getIndex));
                
                if (vipDialogItem->getVipStates() != 4) {
                    if (wordCount < contentLength) {
                        wordCount = getContentLength();
                        this->setTouchEnabled(false);
                        
                        this->closeEyesAnimation();
                        
                        this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::openTouch), .5f);
                        
                    }else{
                        
                        this->setTouchEnabled(false);
                        
                        this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::getIndex), .5f);
                        
                    }
                }else{
                    this->setTouchEnabled(false);
                    
                    this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::getIndex), .5f);
                }
            }
        }else{
            if (logIndex == 1) {
                this->unschedule(SEL_SCHEDULE(&VipStoryScene::getIndex));
                
                if (vipDialogItem->getVipStates() != 4) {
                    if (wordCount < contentLength) {
                        wordCount = getContentLength();
                        this->setTouchEnabled(false);
                        
                        this->closeEyesAnimation();
                        
                        this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::openTouch), .5f);
                        
                    }else{
                        
                        this->setTouchEnabled(false);
                        
                        this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::getIndex), .3f);
                        
                    }
                }else{
                    this->setTouchEnabled(false);
                    
                    this->scheduleOnce(SEL_SCHEDULE(&VipStoryScene::getIndex), .3f);
                }
            }
        }
    }
    
    return true;
}

void VipStoryScene::openTouch(float dt){
    logIndex = 0;
    this->setTouchEnabled(true);
}

void VipStoryScene::getIndex(float dt){
    if (quanSpr != NULL) {
        quanBool = false;
        quanSpr->setVisible(false);
    }
    
    if (vipDialogItem->getVipArray() == NULL) {
        index = vipDialogItem->getVipNext();
        
        CCString* _achievement = CCString::createWithFormat("%s",vipDialogItem->getVipAchievement().c_str());
        if (index == -1) {
            buttonBool1 = false;
            buttonBool2 = false;
            buttonBool3 = false;
            
            if (this->getChildByTag(0x88888) != NULL) {
                CCNode* node = this->getChildByTag(0x88888);
                if (node->getChildByTag(Tag_kuaijin) != NULL) {
                    kuaijinToggleItem->setSelectedIndex(0);
                }
                if (node->getChildByTag(Tag_zidong) != NULL) {
                    zidongToggleItem->setSelectedIndex(0);
                }
            }
            
            this->setTouchEnabled(false);
            
            storyIndex = 1;
            LOADING->show_loading();
            endingStr = CCString::createWithFormat("%s", "-1");
            endingStr->retain();
            CCString* indexStr = CCString::createWithFormat("%d", m_current_story_index_id);
            NET->commit_story2_507(indexStr->getCString(), endingStr->getCString());
            
        }else{
            vipDialogItem = (VipDialogItem* )vipDialog->getVipDialogs()->objectAtIndex(index);
            this->init(vipDialogItem);
            wordCount = 0;
            this->dialogueControl(vipDialogItem);
        }
        
    }else{
        if (!openButtonBool) {
            openButtonBool = true;
            this->initButton();
        }
    }
}

void VipStoryScene::didAccelerate( CCAcceleration* pAccelerationValue){
    float deceleration = 0 ;
    float sensitivity = 500;
    
    playerVelocity.x = playerVelocity.x * deceleration+ pAccelerationValue->x*sensitivity;
    
    playerVelocity.x = playerVelocity.x/ 10;
}

//void VipStoryScene::message_box_did_selected_button(AHMessageBox* box, AH_BUTTON_TYPE button_type, AH_BUTTON_TAGS button_tag){
//    box->animation_out();
//    if (button_type == AH_BUTTON_TYPE_YESNO2) {
//        if (button_tag == AH_BUTTON_TAG_YES) {
//            if (MMAudioManager::get_instance()->is_music_on()) {
//                MMAudioManager::get_instance()->stopBackgroundMusic();
//            }
//            
//            if (MMAudioManager::get_instance()->is_effect_on()) {
//                MMAudioManager::get_instance()->play_effect(kAudio_Button_Back, false);
//            }
//            
//            // talkingData
//            MMDataManager::get_instance()->onEvent("点击事件", "星途界面", "中途退出剧情");
//            
//            MMDataManager::get_instance()->setInGame(false);
//            MMLoadingLayer* layer = MMLoadingLayer::create_with_type(0);
//            this->addChild(layer, 5000);
//            MMNetManager::get_instance()->http_3000_get_pet_story(MMDataManager::get_instance()->get_current_manpet_id());
//        }
//    }
//}

void VipStoryScene::keyBackClicked(){
    num_child++;
//    CCLog("===== VipStoryScene  children_num: %d", num_child);
    if (num_child> 1) {
        return;
    }
    
    this->setKeypadEnabled(false);
    this->fhCallBack(this);
}

void VipStoryScene::fhCallBack(CCObject* pSender){
    
    // talkingData
    DATA->onEvent("点击事件", "vip情景界面", "点击退出");
    
    AUDIO->goback_effect();
//    if (MMAudioManager::get_instance()->is_effect_on()) {
//        MMAudioManager::get_instance()->play_effect(kAudio_audio_AHMessageBox, false);
//    }
    
//    AHMessageBox* mb = AHMessageBox::create_with_message(def_VipStoryScene_007, this, AH_AVATAR_TYPE_NO, AH_BUTTON_TYPE_YESNO2, true);
//    mb->setPosition(Center);
//    CCDirector::sharedDirector()->getRunningScene()->addChild(mb, 3010);
    
    CCScene* scene = VipQingjingScene::scene();
    CCDirector::sharedDirector()->replaceScene(scene);
}

void VipStoryScene::LabelColorFhCallBack(CCObject* pSender){
    if (this->getChildByTag(0x999999) != NULL) {
        this->removeChildByTag(0x999999);
    }
    
    buttonBool1 = false;
    buttonBool2 = false;
    buttonBool3 = false;
    recordBool1 = buttonBool1;
    recordBool2 = buttonBool2;
    recordBool3 = buttonBool3;
    
    this->setTouchEnabled(true);
}

void VipStoryScene::_507CallBack(CCObject* pSender){
    LOADING->remove();
//    CCLog("<><><>endingStr ==== %s", endingStr->getCString());
    VipStorySettlementOfTheAnimationLayer* layer = VipStorySettlementOfTheAnimationLayer::create_with_index(storyIndex, endingStr->getCString());
    this->addChild(layer, 1000);
}







