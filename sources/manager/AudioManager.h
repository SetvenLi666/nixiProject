//
//  AudioManager.hpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#ifndef AudioManager_hpp
#define AudioManager_hpp

#include "cocos2d.h"
#include "SimpleAudioEngine.h"


USING_NS_CC;
using namespace CocosDenshion;

#define AUDIO       AudioManager::Inst()

class AudioManager : public SimpleAudioEngine
{
public:
    ~AudioManager();
    static AudioManager* Inst();
    void init(bool music, bool effect);
    void preload_audios();
    void purge();
    
public:
    void play_music(const char* pszFilePath, bool bLoop);
    unsigned int play_effect(const char* pszFilePath, bool bLoop);
    
    void pause_music();
    void resume_music();
    
    void pause_effects();
    void resume_all_effects();
    
    bool is_music_on();
    bool is_effect_on();
    
    bool is_music_paused();
    bool is_effect_paused();
    
    void set_music_on(bool is_on);
    void set_effect_on(bool is_on);
    
private:
    bool                        m_is_music_on;
    bool                        m_is_music_paused;
    
    bool                        m_is_effect_on;
    bool                        m_is_effect_paused;
};

#endif /* AudioManager_hpp */
