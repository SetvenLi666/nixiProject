//
//  AchievementComp.hpp
//  tiegao
//
//  Created by mac on 16/7/29.
//
//

#ifndef AchievementComp_hpp
#define AchievementComp_hpp

#include "cocos2d.h"
#include "cocos-ext.h"
#include "json_lib.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace CSJson;


class AchievementItem : public CCObject
{
public:
    ~AchievementItem() {}
    CREATE_FUNC(AchievementItem);
    virtual bool init() { return true; }
    void config(Value json);
    
    CC_SYNTHESIZE(int, _id, Id);
    CC_SYNTHESIZE(int, _goal, Goal);
    CC_SYNTHESIZE(int, _num, Num);
    CC_SYNTHESIZE_PASS_BY_REF(string, _desc, Desc);
    CC_SYNTHESIZE(int, _status, Status);
};


class AchievementComp : public CCObject
{
public:
    ~AchievementComp();
    CREATE_FUNC(AchievementComp);
    bool init();
    
    void init_template(Value json);
    void update_user_accumulated(Value json);
    void update_user_achieved(Value json);
    void update_user_finished(Value json);
    int itemCount();
    AchievementItem* fetchItem(int idx);
    int fetchItemState(int id);
    int fetchItemAccumulate(int id);
    
private:
    int                     _totalProgress;
    CCArray*                _items;
    
    CCDictionary*           _accumulated;
    CCArray*                _achieved;
    CCArray*                _finished;
};

#endif /* AchievementComp_hpp */
