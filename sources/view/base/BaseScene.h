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
#include "FlashNumberLabel.h"
#include "FlashNumberLabel2.h"

USING_NS_CC;
using namespace extension;

class BaseScene
: public CCLayer
, public FlashNumberLabelDelegate
, public FlashNumberLabel2Delegate
{
public:
    ~BaseScene();
    static CCScene* scene();
    CREATE_FUNC(BaseScene);
    virtual bool init();
    
    void init_UI();
    void hideBaseScene();
    void openBaseScene();
    
    virtual void onEnter();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch * pTouch, CCEvent * pEvent);
    
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
    CCSprite* timeKuangSpr;
    
    int _minute, _second;
    CCLabelTTF* m_time_num, *m_tili_num;
    unsigned tili_num;
    void updataTileTime(float dt);
    
    FlashNumberLabel* m_lbl_coin;
    FlashNumberLabel2* m_lbl_gold;
    void updataMoney();
    
private:
    void show_purchase_panel();
    void show_energybuy_panel();
    void updatePhaseProgress();
    
    void nc_need_coin_fly(CCObject* pObj);
    void nc_need_gold_fly(CCObject* pObj);
    void nc_need_energy_fly(CCObject* pObj);
    void nc_coin_fly_completed(CCObject* pObj);
    void nc_gold_fly_completed(CCObject* pObj);
    void nc_energy_fly_completed(CCObject* pObj);
    
private:
    CCSprite*                   _phaseStar;
    CCProgressTimer*            _progress;
};

#endif /* defined(__tiegao__BaseScene__) */
