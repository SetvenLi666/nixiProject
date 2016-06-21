//
//  StoryComp.hpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#ifndef StoryComp_hpp
#define StoryComp_hpp

#include "cocos2d.h"
#include "json_lib.h"

USING_NS_CC;
using namespace CSJson;

class StoryComp : public CCObject
{
public:
    ~StoryComp();
    CREATE_FUNC(StoryComp);
    bool init();
    void init_with_json(Value json);
    
public:
    CCDictionary* cmpleted_story();
    bool has_init_story();                          // 是否初始化已完成的剧情信息
    bool has_passed(const char* story_index);       // 剧情是否已通过
    CCArray* story_achievments(const char* story_index); // 剧情已获得成就列表(CCString)
    
private:
    CCDictionary*           _cmpleted_story;
};

#endif /* StoryComp_hpp */
