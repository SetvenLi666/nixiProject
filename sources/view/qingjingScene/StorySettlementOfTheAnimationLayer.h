//
//  StorySettlementOfTheAnimationLayer.h
//  tiegao
//
//  Created by mac on 16-6-21.
//
//

#ifndef __tiegao__StorySettlementOfTheAnimationLayer__
#define __tiegao__StorySettlementOfTheAnimationLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;


class StorySettlementOfTheAnimationLayer : public CCLayerColor
{
public:
    StorySettlementOfTheAnimationLayer();
    ~StorySettlementOfTheAnimationLayer();
    
    void onEnter();
    void onExit();
    
    bool init();
    
    static StorySettlementOfTheAnimationLayer* create_with_index(int index);
    void init_with_index(int index);
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {}
    
    CREATE_FUNC(StorySettlementOfTheAnimationLayer);
    
public:
    
    void openTouch(float dt);
    void closeCallBack(CCObject* pSender);
    
    void creatStar1(float dt);
    void creatStar2(float dt);
    void creatStar3(float dt);
    void creatStar4(float dt);
    void creatStar5(float dt);
    void creatStar6(float dt);
};



#endif /* defined(__tiegao__StorySettlementOfTheAnimationLayer__) */
