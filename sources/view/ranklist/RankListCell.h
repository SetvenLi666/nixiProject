//
//  RankListCell.hpp
//  tiegao
//
//  Created by mac on 16/12/24.
//
//

#ifndef RankListCell_hpp
#define RankListCell_hpp

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class ShowComp;

class RankListCell : public CCSprite
{
public:
    void showRewardAction(const char* type, int num);

public:
    ~RankListCell();
    static RankListCell* create(const char *pszFileName);
    bool init(const char *pszFileName);
    void config(int idx, ShowComp* show, float cellWidth, float cellHeight);
    
    void goStar(int idx, int progress, int goal, int status);
    
private:
    CCSprite* getNumSprite(int num);
    void moveFinish(CCObject* pObj);
    void toBeFriend(CCMenuItem* btn);
    
    CCProgressTimer*        _progress;
    CCMenu*                 _menuBtn;
    CCSprite*               _sptButton1;
    CCSprite*               _sptButton2;
    CCSprite*               _star;
    
    CCSprite*               _rewardIcon;
};

#endif /* RankListCell_hpp */
