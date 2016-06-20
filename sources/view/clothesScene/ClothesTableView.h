//
//  ClothesTableView.h
//  tiegao
//
//  Created by mac on 16-6-15.
//
//

#ifndef __tiegao__ClothesTableView__
#define __tiegao__ClothesTableView__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#define def_TX_Time             0.7
#define def_Page_Index          4.5

class ClothesTableView
: public CCLayer
, public CCTableViewDataSource
, public CCTableViewDelegate
{
public:
    ClothesTableView();
    ~ClothesTableView();
    
    virtual bool init();
    
    
    CREATE_FUNC(ClothesTableView);
    
public:
    
    //CCTableViewDelegate继承自CCScrollViewDelegate
    virtual void scrollViewDidScroll(cocos2d::extension::CCScrollView* view);
    
    virtual void scrollViewDidZoom(cocos2d::extension::CCScrollView* view){}
    
    //点击哪个cell
    virtual void tableCellTouched(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    
    //每个cell的size
    virtual cocos2d::CCSize cellSizeForTable(cocos2d::extension::CCTableView *table);
    
    //生成cell
    virtual cocos2d::extension::CCTableViewCell* tableCellAtIndex(cocos2d::extension::CCTableView *table, unsigned int idx);
    
    //cell的数量
    virtual unsigned int numberOfCellsInTableView(cocos2d::extension::CCTableView *table);
    
    //按下去的时候，就是高亮显示，这里可以设置高亮状态
    virtual void tableCellHighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    
    //松开的时候，取消高亮状态
    virtual void tableCellUnhighlight(cocos2d::extension::CCTableView* table, cocos2d::extension::CCTableViewCell* cell);
    
    void onEnter();
    
    void onExit();
    
    void updateTableView(int type);
    int updataClothes(int type);
    void updateTableCell();
        
public:
    
    CCTableView* pTableView;
        
    float offsetFloat;
    
    bool openTouchBool;
    void isOpenTouchBool();
    void setOpenTouchBool();
    void changeClothesTouch(int cloth_id, CCInteger* clothesTemp_id, CCDictionary* clothesTemp, cocos2d::extension::CCTableViewCell* cell);
        
    int clothesType;
    int clothesPage;
    
    CCSprite* gouSpr, *kuangSpr, *kuangSpr1, *kuangSpr2, *kuangSpr3, *kuangSpr4, *kuangSpr5, *kuangSpr6, *kuangSpr7, *kuangSpr8, *kuangSpr9, *ycSpr;
    CCSprite* tf_kSpr, *wt_kSpr, *sy_kSpr, *kz_kSpr, *wz_kSpr, *xz_kSpr, *sp_kSpr;
    
};
#endif /* defined(__tiegao__ClothesTableView__) */