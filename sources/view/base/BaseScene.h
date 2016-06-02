//
//  BaseScene.h
//  tiegao
//
//  Created by mac on 16-5-30.
//
//

#ifndef __tiegao__BaseScene__
#define __tiegao__BaseScene__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
using namespace extension;

class BaseScene : public CCLayer
{
public:
    ~BaseScene();
    static CCScene* scene();
    CREATE_FUNC(BaseScene);
    virtual bool init();
    
    void init_UI();
    void hideBaseScene();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual void will_number_scroll();
    virtual void did_number_stoped();
    
    virtual void will_number_scroll2();
    virtual void did_number_stoped2();
    
    virtual void will_number_scroll3();
    virtual void did_number_stoped3();
    
    
    void tiliCallBack(CCObject* pSender);
    void goldCallBack(CCObject* pSender);
    void coinCallBack(CCObject* pSender);
    
public:
    
    CCMenuItem* nameItem, *tiliItem, *coinItem, *goldItem;
    CCMenu* barMenu;
};

#endif /* defined(__tiegao__BaseScene__) */
