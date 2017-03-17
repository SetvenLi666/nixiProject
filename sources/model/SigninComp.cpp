//
//  SigninComp.cpp
//  tiegao
//
//  Created by mac on 16/5/24.
//
//

#include "SigninComp.h"
#include "AppUtil.h"
#include "SigninComp.h"

#pragma mark - Export API

bool SigninComp::has_init_signin7_template() {
    return (_signin7Template != NULL);
}

CCArray* SigninComp::signin7_template() const {
    return _signin7Template;
}

SigninState SigninComp::fetch_signin7_state(const string& id) {
    CCInteger* state = (CCInteger*)_signin7Info->objectForKey(id);
    if (state == NULL) {
        return e_SigninState_Locked;
    }
    else {
        return (SigninState)state->getValue();
    }
}

#pragma mark - Inner API

SigninComp::~SigninComp() {
    CC_SAFE_DELETE(_signin7Info);
    CC_SAFE_DELETE(_signin7Template);
}

bool SigninComp::init() {
    _signin7Template = NULL;
    _signin7Info = NULL;

    return true;
}

void SigninComp::init_signin7_template(Value json) {
    if (json.type() == nullValue) {
        CCLOG("SigninComp::init_signin7_template() json object error.");
        return;
    }
#error "跟另一个error相关的信息"
    /*
     "id" : 跟第几天相关
     "type" : 衣服-"clothes", 其他-"wealth"
     "rewards" : type == clothes时，为衣服的id（CCInteger）的数组CCArray，
                 type == wealth时，为CCDictionary:
                    "rewards": {
                        "diam": 0,
                        "coin": 0,
                        "piece": 0,
                        "energy": 0
                    }，
     */
    CCDictionary* dic = AppUtil::dictionary_with_json(json);
    CCArray* keys = dic->allKeys();
    AppUtil::sort_string_array(keys);
    _signin7Template = CCArray::create();
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(keys, pObj) {
        CCString* key = (CCString*)pObj;
        _signin7Template->addObject(dic->objectForKey(key->getCString()));
    }
    _signin7Template->retain();
    this->test_signin7Template();
}

void SigninComp::update_signin7_info(Value json) {
    if (json.type() == nullValue) {
        CCLOG("SigninComp::parse_signin7_info() json object error.");
        return;
    }
    
    CC_SAFE_RELEASE(_signin7Info);
    _signin7Info = AppUtil::dictionary_with_json(json);
    _signin7Info->retain();
}

void SigninComp::test_signin7Template() {
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(_signin7Template, pObj) {
        CCDictionary* item = (CCDictionary*)pObj;
        CCLOG("签到 id = %s", item->valueForKey("id")->getCString());
        CCLOG("签到 type = %s", item->valueForKey("type")->getCString());
        CCLOG("签到 uri = %s", item->valueForKey("uri")->getCString());
    }
}
