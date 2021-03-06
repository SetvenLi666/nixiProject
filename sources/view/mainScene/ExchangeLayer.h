//
//  ExchangeLayer.h
//  tiegao
//
//  Created by mac on 16-8-15.
//
//

#ifndef __tiegao__ExchangeLayer__
#define __tiegao__ExchangeLayer__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;


class ExchangeLayer : public CCLayerColor
{
public:
    ExchangeLayer();
    ~ExchangeLayer();
    
    void onEnter();
    void onExit();
    
    bool init();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {}
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {}
    
    CREATE_FUNC(ExchangeLayer);
    
public:
    
    CCSprite* bgSpr;
    CCSprite* dikuangSpr;
    
    CCMenuItem* jiantouItem1,* jiantouItem2;
    
    CCDictionary* allClothesDic;
    CCDictionary* myClothesTemp;
    CCArray* suitsArr;
    CCArray* userArr;
    CCArray* allClthoes;
    
    CCLabelTTF* exchangeLabel;
    
    int nowIndex;
    int exchangeIndex;
    
    bool touchBool;
    
    
    CCSprite* _ManSpr;
    CCSprite* _touSpr;
    CCSprite* _tfSpr1; // 头发
    CCSprite* _tfSpr2;
    CCSprite* _tfSpr3;
    CCSprite* _wtSpr1; // 外套
    CCSprite* _wtSpr2;
    CCSprite* _wtSpr3;
    CCSprite* _sySpr1; // 上衣
    CCSprite* _sySpr2;
    CCSprite* _sySpr3;
    CCSprite* _kzSpr1; // 裤子
    CCSprite* _kzSpr2;
    CCSprite* _kzSpr3;
    CCSprite* _wzSpr1; // 袜子
    CCSprite* _wzSpr2;
    CCSprite* _wzSpr3;
    CCSprite* _xzSpr1; // 鞋子
    CCSprite* _xzSpr2;
    CCSprite* _xzSpr3;
    CCSprite* _spSpr1; // 饰品
    CCSprite* _spSpr2;
    CCSprite* _spSpr3;
    CCSprite* _bSpr1;  // 包
    CCSprite* _bSpr2;
    CCSprite* _bSpr3;
    CCSprite* _zrSpr1; // 妆容
    
public:
    
    void openTouch();
    void formattingData();
    void creat_View();
    void creat_Man();
    void initClothes();
    void creat_Label(CCSprite* spr, int price, int index);
    void creat_Clothes(CCSprite* spr, int index);
    
    void buttonCallBack(CCObject* pSender);
    void jiantou1CallBack(CCObject* pSender);
    void jiantou2CallBack(CCObject* pSender);
    void updataButton();
    void _311CallBack(CCObject* pSender);
};
#endif /* defined(__tiegao__ExchangeLayer__) */
