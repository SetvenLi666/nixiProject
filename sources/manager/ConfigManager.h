//
//  ConfigManager.hpp
//  tiegao
//
//  Created by mac on 16/5/30.
//
//

#ifndef ConfigManager_hpp
#define ConfigManager_hpp

#include "cocos2d.h"

USING_NS_CC;

#define CONFIG         ConfigManager::Inst()

class ConfigManager
{
public:
    ~ConfigManager();
    static ConfigManager* Inst();
    void config();
    
public:
    int                     net;
    string                  login_addr;

public:
    CCArray* mission();
    CCDictionary* missionDialog();
    CCDictionary* clothes();
    
    CCArray* getMissionDialog(int phase, int taskID);
    
    bool has_saved_uuid();
    void save_uuid(const char* str);
    string saved_uuid();
    bool has_saved_account();
    
    string saved_account();
    string saved_password();
    void save_account(const char* str);
    void save_password(const char* str);
    
private:
    void conf_login_addr(int net);
    void conf_mission();
    void conf_mission_dialog(int phase);
    void conf_clothes();

private:
    bool            _hasconfig;
    CCArray*        _mission;
    CCDictionary*   _missionDialog;
    CCDictionary*   _clothes;
};

#endif /* ConfigManager_hpp */
