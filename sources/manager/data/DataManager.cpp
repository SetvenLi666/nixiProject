//
//  DataManager.cpp
//  tiegao
//
//  Created by mac on 16/5/7.
//
//

#include "DataManager.h"
#include <sys/time.h>

static DataManager* _instance = nullptr;

DataManager::~DataManager() {
    
}

DataManager* DataManager::Inst() {
    if (_instance == nullptr) {
        _instance = new DataManager();
        _instance->init_data();
    }
    
    return _instance;
}

void DataManager::init_data() {
    this->setLogin(LoginComp::create());
    this->setPlayer(PlayerComp::create());
    this->setClothes(ClothesComp::create());
    this->setIAP(IAPComp::create());
    this->setMail(MailComp::create());
    this->setMessage(MessageComp::create());
    this->setMission(MissionComp::create());
    this->setNews(NewsComp::create());
    this->setNotice(NoticeComp::create());
    this->setRanking(RankingComp::create());
    this->setShow(ShowComp::create());
    this->setSignin(SigninComp::create());
    this->setSocial(SocialComp::create());
    this->setStory(StoryComp::create());
}

time_t DataManager::cur_timestamp() {
    //    time_t t = time(NULL);
    struct timeval t;
    gettimeofday(&t, NULL);
    time_t rtn = t.tv_sec * 100000 + t.tv_usec;
    CCLOG("Now time = %ld", rtn);
    return rtn;
}

void DataManager::http_response_error(int code, string msg) {
    
}

void DataManager::http_response_handle(int resp_code, string response) {
    if (200 != resp_code) {
        CCLOG("DataManager::http_response_handle() - HTTP response status code: %d.", resp_code);
        return;
    }
    
    Reader reader;
    Value root;
    if (!reader.parse(response, root, false)) {
        CCLOG("DataManager::http_response_handle() json reader error.");
        return;
    }
    
    int code = root["code"].asInt();
    if (0 == code) {
        int cid = root["cid"].asInt();
        Value content = root["content"];
        this->handle_protocol(cid, content);
    }
    else {
//        string msg = root["content"].asString();
    }
}

void DataManager::handle_protocol(int cid, Value content) {
    CCNotificationCenter* nc = CCNotificationCenter::sharedNotificationCenter();
    const char* notif_format = "HTTP_FINISHED_%d";
    switch (cid) {
        case 900: {
            _login->init_with_json(content);
        } break;
            
        case 902: {
            _player->init_with_json(content["player"]);
            _show->init_with_json(content["show"]);
            _news->init_with_json(content["news"]);
        } break;
            
        case 802: {
            _social->update_stranger(content["strangers"]);
        } break;
            
        case 803: {
            // 只返回OK
//            _social->init_with_json(content["social"]);
        } break;
            
        case 804: {
            _message->init_with_json(content["messages"]);
        } break;
            
        default:
            break;
    }
    
    nc->postNotification(CCString::createWithFormat(notif_format, cid)->getCString());
}


