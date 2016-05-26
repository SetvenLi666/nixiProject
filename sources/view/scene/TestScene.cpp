//
//  TestScene.cpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#include "TestScene.h"
#include "DataManager.h"
#include "NetManager.h"

bool TestScene::init() {
    if (!CCScene::init()) {
        return false;
    }
    
    this->setTouchSwallowEnabled(true);
    this->setTouchMode(kCCTouchesOneByOne);
    this->setTouchEnabled(true);
    
    CCSize win_size = CCDirector::sharedDirector()->getWinSize();
    
    CCSprite* bg = CCSprite::create("pic/test_bg.png");
    bg->setPosition(ccp(win_size.width * 0.5, win_size.height * 0.5));
    this->addChild(bg);
    
    _content = CCLayer::create();
    this->addChild(_content);
    
    return true;
}

void TestScene::onEnter() {
    CCScene::onEnter();
    
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    nc->addObserver(this, SEL_CallFuncO(&TestScene::login_game_server), "HTTP_FINISHED_900", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TestScene::social_view), "HTTP_FINISHED_902", NULL);
    nc->addObserver(this, SEL_CallFuncO(&TestScene::stranger_view), "HTTP_FINISHED_802", NULL);
//    nc->addObserver(this, SEL_CallFuncO(&TestScene::stranger_view), "HTTP_FINISHED_803", NULL);
    
    NetEnv netenv = NetManager::Inst()->obtain_net_env();
    std::string env_info;
    switch (netenv) {
        case e_NetEnv_NotReachable:
            env_info = "没有网络!~";
            break;
        case e_NetEnv_ViaWiFi:
            env_info = "使用 WiFi 接入";
            break;
        case e_NetEnv_ViaWWAN:
            env_info = "使用 WWAN 网络";
            break;
        case e_NetEnv_Unknown:
        default:
            env_info = "未知网络!~";
            break;
    }
    CCLOG("%s", env_info.c_str());
    
    this->login_view();
}

void TestScene::onExit() {
    this->unscheduleAllSelectors();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
    
    CCScene::onExit();
}

void TestScene::login_view() {
    _content->removeAllChildren();
    
    CCMenuItemFont* btn_fast_login = CCMenuItemFont::create("游客登入", this, SEL_MenuHandler(&TestScene::fast_login));
    CCMenu* menu = CCMenu::createWithItem(btn_fast_login);
    menu->setColor(ccRED);
    menu->alignItemsVerticallyWithPadding(10);
    _content->addChild(menu);
}

void TestScene::social_view() {
    _content->removeAllChildren();
    {
        CCMenuItemFont* btn_return = CCMenuItemFont::create("返回 登入", this, SEL_MenuHandler(&TestScene::on_return));
        btn_return->setTag(900);
        CCMenu* rtn_menu = CCMenu::createWithItem(btn_return);
        rtn_menu->setColor(ccRED);
        rtn_menu->setPosition(ccp(550, 1066));
        _content->addChild(rtn_menu);
    }
    
    CCMenuItemFont* btn_recommend_stranger = CCMenuItemFont::create("推荐陌生人", this, SEL_MenuHandler(&TestScene::recommend_stranger));
    
    CCMenu* menu = CCMenu::create(btn_recommend_stranger, NULL);
    menu->setColor(ccORANGE);
    menu->alignItemsVerticallyWithPadding(10);
    _content->addChild(menu);
}

void TestScene::stranger_view() {
    _content->removeAllChildren();
    {
        CCMenuItemFont* btn_return = CCMenuItemFont::create("返回 社交", this, SEL_MenuHandler(&TestScene::on_return));
        btn_return->setTag(800);
        CCMenu* rtn_menu = CCMenu::createWithItem(btn_return);
        rtn_menu->setColor(ccRED);
        rtn_menu->setPosition(ccp(550, 1066));
        _content->addChild(rtn_menu);
    }
    
    CCDictionary* strangers = DataManager::Inst()->getSocial()->strangers();
    CCArray* sids = strangers->allKeys();
    CCObject* pObj = NULL;
    CCArray* btns = CCArray::create();
    CCARRAY_FOREACH(sids, pObj) {
        CCString* sid = (CCString* )pObj;
        CCMenuItemFont* btn = CCMenuItemFont::create(sid->getCString(), this, SEL_MenuHandler(&TestScene::msg_with_friend_ask));
        btn->setUserObject(ccs(sid->getCString()));
        btns->addObject(btn);
    }
    
    CCMenu* menu = CCMenu::createWithArray(btns);
    menu->setColor(ccBLUE);
    menu->alignItemsVerticallyWithPadding(10);
    menu->getChildren();
    _content->addChild(menu);
}

void TestScene::on_return(CCMenuItem* btn) {
    int tag = btn->getTag();
    switch (tag) {
        case 900: {
            this->login_view();
        } break;
            
        case 800: {
            this->social_view();
        } break;
            
        default:
            break;
    }
}

void TestScene::fast_login() {
    NetManager::Inst()->fast_login_900(DataManager::Inst()->getLogin()->obtain_UUID());
}

void TestScene::login_game_server() {
    NetManager::Inst()->login_game_server_902();
}

void TestScene::recommend_stranger() {
    NetManager::Inst()->recommend_stranger_802();
}

void TestScene::msg_with_friend_ask(CCMenuItem* btn) {
    CCString* other_sid = (CCString* )btn->getUserObject();
    CCLOG("Send msg to other: %s", other_sid->getCString());
    NetManager::Inst()->send_message_803(other_sid->getCString(), e_Msg_Friend_Ask);
}
