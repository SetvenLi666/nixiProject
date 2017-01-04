//
//  RankListView.cpp
//  tiegao
//
//  Created by stevenLi on 16/6/19.
//
//

#include "RankListView.h"
#include "DataManager.h"
#include "DisplayManager.h"
#include "NetManager.h"


#include "RankListCell.h"
#include "TotalRankScene.h"


#include "PromptLayer.h"

const float CELL_WIDTH = 252;
const float CELL_HEIGHT = 96;

void RankListView::reload() {
    _tv->reloadData();
}

RankListView::~RankListView() {

}

bool RankListView::init() {
    if (!CCLayer::init()) {
        return false;
    }
    
    _datasource = NULL;
    _selectedIndex = 0;
    
    CCSprite* panel = CCSprite::create("pic/ranklist/rl_panel.png");
    CCSize panelSize = panel->getContentSize();
    panel->setAnchorPoint(ccp(0.5, 0));
    panel->setPosition(ccp(DISPLAY->W() - panelSize.width * 0.5, DISPLAY->H() * 0.15));
    this->addChild(panel);
    
    {
        CCMenuItem* titleCompetition = CCMenuItemImage::create("pic/ranklist/rl_title_1.png", "pic/ranklist/rl_title_1.png");
        CCMenuItem* titleCollection = CCMenuItemImage::create("pic/ranklist/rl_title_2.png", "pic/ranklist/rl_title_2.png");
        CCMenuItemToggle* toggleTitle = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&RankListView::onTitleToggle), titleCollection, titleCompetition, NULL);
        CCMenu* menuTitle = CCMenu::createWithItem(toggleTitle);
        menuTitle->setPosition(ccp(panelSize.width * 0.52, panelSize.height * 0.95));
        panel->addChild(menuTitle);
    }
    {
        ShowComp* selfShow = DATA->getShow();
        RankListCell* sel1 = RankListCell::create("pic/ranklist/rl_bar_self.png");
        RankListCell* sel2 = RankListCell::create("pic/ranklist/rl_bar_self.png");
        RankListCell* sel3 = RankListCell::create("pic/ranklist/rl_bar_self.png");
        RankListCell* sel4 = RankListCell::create("pic/ranklist/rl_bar_self.png");
        
        sel1->config(-1, selfShow, CELL_WIDTH, CELL_HEIGHT);
        sel2->config(-1, selfShow, CELL_WIDTH, CELL_HEIGHT);
        sel3->config(-1, selfShow, CELL_WIDTH, CELL_HEIGHT);
        sel4->config(-1, selfShow, CELL_WIDTH, CELL_HEIGHT);
        
        sel1->setPosition(CCPointZero);
        sel2->setPosition(CCPointZero);
        sel3->setPosition(CCPointZero);
        sel4->setPosition(CCPointZero);
        
//        CCMenuItem* btnSelf1 = CCMenuItemImage::create("pic/ranklist/rl_bar_self.png", "pic/ranklist/rl_bar_self.png");
//        CCMenuItem* btnSelf2 = CCMenuItemImage::create("pic/ranklist/rl_bar_self.png", "pic/ranklist/rl_bar_self.png");
        CCMenuItem* btnSelf1 = CCMenuItemSprite::create(sel1, sel2);
        CCMenuItem* btnSelf2 = CCMenuItemSprite::create(sel3, sel4);
        
        CCSprite* sel5 = CCSprite::create("pic/ranklist/rl_bar_selected.png");
        sel5->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
        btnSelf2->addChild(sel5, -1);
        
        _toggleSelf = CCMenuItemToggle::createWithTarget(this, SEL_MenuHandler(&RankListView::onSelfBarToggle), btnSelf1, btnSelf2, NULL);
        CCMenu* menuSelf = CCMenu::createWithItem(_toggleSelf);
        menuSelf->setPosition(ccp(panelSize.width * 0.52, panelSize.height * 0.85));
        panel->addChild(menuSelf);
    }
    {
        _tv = CCTableView::create(this, CCSizeMake(CELL_WIDTH, 686));
        _tv->setDirection(kCCScrollViewDirectionVertical);
        _tv->setVerticalFillOrder(kCCTableViewFillTopDown);
        _tv->ignoreAnchorPointForPosition(false);
        _tv->setAnchorPoint(ccp(0, 0));
        _tv->setPosition(ccp(5, 2));
        _tv->setDelegate(this);
        panel->addChild(_tv);
    }
    
    return true;
}

void RankListView::onEnter(){
    CCLayer::onEnter();
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, SEL_CallFuncO(&RankListView::tobeFriend_callback_803), "HTTP_FINISHED_803", NULL);
}

void RankListView::onExit(){
    CCLayer::onExit();
    CCNotificationCenter::sharedNotificationCenter()->removeAllObservers(this);
}

RankListCell* RankListView::createItemCell(unsigned int idx) {
    RankListCell* rtn = NULL;
    if (idx == 0) {
        rtn = RankListCell::create("pic/ranklist/rl_bar_first.png");
    }
    else if (idx == 1) {
        rtn = RankListCell::create("pic/ranklist/rl_bar_second.png");
    }
    else if (idx == 2) {
        rtn = RankListCell::create("pic/ranklist/rl_bar_third.png");
    }
    else {
        rtn = RankListCell::create("pic/ranklist/rl_bar_other.png");
    }
    
    ShowComp* show = (ShowComp* )_datasource->objectAtIndex(idx);
    rtn->config(idx, show, CELL_WIDTH, CELL_HEIGHT);
    
    return rtn;
}

void RankListView::onTitleToggle(CCMenuItemToggle *btn) {
    
}

void RankListView::onSelfBarToggle(CCMenuItemToggle *btn) {
    int oldSelected = _selectedIndex;
    if (btn->getSelectedIndex() == 1) {
        if (oldSelected != -1) {
            _selectedIndex = -1;
            _tv->updateCellAtIndex(oldSelected);
        }
        
        _toggleSelf->setEnabled(false);
    }
}

void RankListView::tobeFriend_callback_803(){
    PromptLayer* tip = PromptLayer::create();
    tip->show_prompt(CCDirector::sharedDirector()->getRunningScene(), "成功添加好友~!");
}


#pragma mark - CCScrollViewDatasource

unsigned int RankListView::numberOfCellsInTableView(CCTableView *table){
    if (_datasource) {
        return _datasource->count();
    }
    else {
        return 0;
    }
}

//每个cell的size
cocos2d::CCSize RankListView::cellSizeForTable(CCTableView *table){
    return CCSizeMake(CELL_WIDTH, CELL_HEIGHT);
}

//生成cell
CCTableViewCell* RankListView::tableCellAtIndex(CCTableView *table, unsigned int idx){
    CCTableViewCell* pCell = new CCTableViewCell();
    pCell->autorelease();
    RankListCell* itemCell = this->createItemCell(idx);
    pCell->addChild(itemCell, 2);
    if (idx == _selectedIndex) {
        CCSprite* sel = CCSprite::create("pic/ranklist/rl_bar_selected.png");
        sel->setPosition(ccp(CELL_WIDTH * 0.5, CELL_HEIGHT * 0.5));
        pCell->addChild(sel, 1);
    }
    
    return pCell;
}

#pragma mark - CCTableViewDelegate

//点击哪个cell
void RankListView::tableCellTouched(CCTableView* table, CCTableViewCell* cell) {
    int idx = cell->getIdx();
    if (idx != _selectedIndex) {
        int oldSelected = _selectedIndex;
        _selectedIndex = idx;
        table->updateCellAtIndex(oldSelected);
        table->updateCellAtIndex(idx);
    }
    
    _toggleSelf->setSelectedIndex(0);
    _toggleSelf->setEnabled(true);
}

//按下去的时候，就是高亮显示，这里可以设置高亮状态
void RankListView::tableCellHighlight(CCTableView* table, CCTableViewCell* cell) {

}

//松开的时候，取消高亮状态
void RankListView::tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell) {

}

#pragma mark - CCScrollViewDelegate

void RankListView::scrollViewDidScroll(CCScrollView* view){
    CCPoint contOffsetPos = _tv->getContentOffset();
    if (_datasource->count() > 6) {
        if (contOffsetPos.y < _tv->minContainerOffset().y) {
            _tv->setContentOffset(CCPoint(contOffsetPos.x, _tv->minContainerOffset().y));
        }else if (contOffsetPos.y > _tv->maxContainerOffset().y){
            _tv->setContentOffset(CCPoint(contOffsetPos.x, _tv->maxContainerOffset().y));
        }
    }
}

void RankListView::scrollViewDidZoom(CCScrollView* view) {
    
}