//
//  TestScene.hpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#ifndef TestScene_hpp
#define TestScene_hpp

#include "cocos2d.h"

USING_NS_CC;

class TestScene : public CCScene
{
public:
    CREATE_FUNC(TestScene);
    virtual bool init();
    virtual void onEnter();
    virtual void onExit();
    
protected:
    CCLayer*        _content;
    
private:
    void login_view();
    void social_view();
    void stranger_view();
    
    void on_return(CCMenuItem* btn);
    
    void fast_login();
    void login_game_server();
    void recommend_stranger();
    void msg_with_friend_ask(CCMenuItem* btn);
};

#endif /* TestScene_hpp */
