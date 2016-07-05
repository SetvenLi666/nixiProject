//
//  SpecialManager.cpp
//  tiegao
//
//  Created by mac on 16/7/1.
//
//

#include "SpecialManager.h"
#include "AppUtil.h"

static SpecialManager* _instance = nullptr;

SpecialManager::~SpecialManager() {
    CC_SAFE_DELETE(_specials);
}

SpecialManager* SpecialManager::Inst() {
    if (_instance == nullptr) {
        _instance = new SpecialManager();
    }
    
    return _instance;
}

void SpecialManager::init() {
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("pic/special/star.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("pic/special/petal.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("pic/special/petal2.plist");
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("pic/special/spot.plist");
    
    _specials = CCDictionary::create();
    _specials->retain();
    
    CCAnimation* star = AppUtil::animationWithFrame("star_%d.png", 30, 1, 0.1f);
    _specials->setObject(star, "star");
    
    CCAnimation* petal = AppUtil::animationWithFrame("petal_%d.png", 24, 1, 0.1f);
    _specials->setObject(petal, "petal");
    
    CCAnimation* petal2 = AppUtil::animationWithFrame("petal2_%d.png", 52, 1, 0.1f);
    _specials->setObject(petal2, "petal2");
    
    CCAnimation* spot = AppUtil::animationWithFrame("spot_%d.png", 40, 1, 0.03f);
    _specials->setObject(spot, "spot");
}

void SpecialManager::showStarAt(CCNode* parent, CCPoint pos, int zOrder) {
    CCSprite* star = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("star_1.png"));
    star->setPosition(pos);
    parent->addChild(star, zOrder);
    
    CCActionInterval* anim = CCAnimate::create(animationByName("star"));
    CCCallFuncN* purge = CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::purgeActionNode));
    CCSequence* act = CCSequence::create(anim, purge, NULL);
    star->runAction(act);
}

void SpecialManager::showPetalAt(CCNode* parent, CCPoint pos, int zOrder) {
    CCSprite* petal = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("petal_1.png"));
    petal->setPosition(pos);
    parent->addChild(petal, zOrder);
    
    CCActionInterval* anim = CCAnimate::create(animationByName("petal"));
    CCCallFuncN* purge = CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::purgeActionNode));
    CCSequence* act = CCSequence::create(anim, purge, NULL);
    petal->runAction(act);
}

void SpecialManager::showPetal2At(CCNode* parent, CCPoint pos, int zOrder) {
    CCSprite* petal2 = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("petal2_1.png"));
    petal2->setPosition(pos);
    parent->addChild(petal2, zOrder);
    
    CCActionInterval* anim = CCAnimate::create(animationByName("petal2"));
    CCCallFuncN* purge = CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::purgeActionNode));
    CCSequence* act = CCSequence::create(anim, purge, NULL);
    petal2->runAction(act);
}

void SpecialManager::showSpotAt(CCNode *parent, CCPoint pos, int zOrder) {
    CCSprite* star = CCSprite::createWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("spot_1.png"));
    star->setPosition(pos);
    parent->addChild(star, zOrder);
    
    CCActionInterval* anim = CCAnimate::create(animationByName("spot"));
    CCCallFuncN* purge = CCCallFuncN::create(this, SEL_CallFuncN(&SpecialManager::purgeActionNode));
    CCSequence* act = CCSequence::create(anim, purge, NULL);
    star->runAction(act);
}

CCAnimation* SpecialManager::animationByName(const char* name) {
    return (CCAnimation*)_specials->objectForKey(name);
}

void SpecialManager::purgeActionNode(CCNode *node) {
    if (node) {
        node->removeFromParentAndCleanup(true);
    }
}
