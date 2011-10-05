#include <OGRE/Ogre.h>
#include "Utils/iceDamageTextManager.h"
#include <iostream>
#include "iceGame.h"

#define MAX_OVERLAYS 30
#define TIME_BETWEEN_PLAYER_TEXTS 0.25
#define MAX_QUEUE_CAPACITY 7

template<> iceDamageTextManager* Ogre::Singleton<iceDamageTextManager>::ms_Singleton = 0;

iceDamageTextManager::iceDamageTextManager()
{
	mTextOverlays.resize(MAX_OVERLAYS);
	for(unsigned int i=0;i<MAX_OVERLAYS;i++)
	{
		std::stringstream str;
		str << "DamageText_" << i;
		mTextOverlays[i] = new MovableTextOverlay(str.str());
	}

	mTimeToNextPlayerLabel = 0;

	mEnemyDamageAttributes =        new MovableTextOverlayAttributes("EnemyDamageAttributes",iceGame::getCamera(),"Labels",30,Ogre::ColourValue::Blue);
	mEnemyMissAttributes =          new MovableTextOverlayAttributes("EnemyMissAttributes",iceGame::getCamera(),"Labels",30,Ogre::ColourValue::Blue);
	mPlayerDamageAttributes =       new MovableTextOverlayAttributes("PlayerDamageAttributes",iceGame::getCamera(),"Labels",30,Ogre::ColourValue::Red);
	mPlayerShieldDamageAttributes = new MovableTextOverlayAttributes("PlayerShieldDamageAttributes",iceGame::getCamera(),"Labels",30,Ogre::ColourValue::Blue);
	mPlayerHealAttributes =         new MovableTextOverlayAttributes("PlayerHealAttributes",iceGame::getCamera(),"Labels",30,Ogre::ColourValue::Green);
	mPlayerMissAttributes =         new MovableTextOverlayAttributes("PlayerMissAttributes",iceGame::getCamera(),"Labels",30,Ogre::ColourValue::Red);
	mPlayerLevelUpAttributes =      new MovableTextOverlayAttributes("PlayerLevelUpAttributes",iceGame::getCamera(),"Labels",30,Ogre::ColourValue::ColourValue(1,1,0,1));
}

iceDamageTextManager::~iceDamageTextManager()
{
	for(unsigned int i=0;i < mTextOverlays.size() ;i++)
	{
		delete mTextOverlays[i];
	}

	delete mEnemyDamageAttributes;
	delete mEnemyMissAttributes;
	delete mPlayerDamageAttributes;
	delete mPlayerShieldDamageAttributes;
	delete mPlayerHealAttributes;
	delete mPlayerMissAttributes;
	delete mPlayerLevelUpAttributes;
}

iceDamageTextManager& iceDamageTextManager::getSingleton()
{
    assert(ms_Singleton);
    return (*ms_Singleton);
}

iceDamageTextManager* iceDamageTextManager::getSingletonPtr()
{
    assert(ms_Singleton);
    return ms_Singleton;
}

void iceDamageTextManager::update(Ogre::Real pTimeSinceLastFrame)
{
	for(unsigned int i=0;i<MAX_OVERLAYS;i++)
	{
		mTextOverlays[i]->update(pTimeSinceLastFrame);
	}

	if(!mPlayerOverlayOptions.empty() && mTimeToNextPlayerLabel<=0)
	{
		MovableTextOverlay* textOverlay = _getNextDisabledOverlayText();
		if(textOverlay != NULL)
		{
			TextOverlayOptions options = mPlayerOverlayOptions.front();
			textOverlay->initialize(options.caption,options.mov,options.attributes);
			textOverlay->enable();
			textOverlay->update(0);
			mPlayerOverlayOptions.pop();
			mTimeToNextPlayerLabel = TIME_BETWEEN_PLAYER_TEXTS;
		}
	}

	mTimeToNextPlayerLabel -= pTimeSinceLastFrame;
}

void iceDamageTextManager::showEnemyDamage(Ogre::MovableObject *mov, unsigned int pDamage, bool pIsCritic)
{
	MovableTextOverlay* textOverlay = _getNextDisabledOverlayText();
	if(textOverlay == NULL)
	{
		return;
	}
	std::stringstream damageText;
	damageText << pDamage << (pIsCritic?"!":"");
	Ogre::String StreamAlloc = damageText.str();
	textOverlay->initialize(damageText.str(),mov,mEnemyDamageAttributes);
	textOverlay->enable();
}

void iceDamageTextManager::showEnemyMiss(Ogre::MovableObject *mov)
{
	MovableTextOverlay* textOverlay = _getNextDisabledOverlayText();
	if(textOverlay == NULL)
	{
		return;
	}
	textOverlay->initialize("Miss!",mov,mEnemyMissAttributes);
	textOverlay->enable();
}

void iceDamageTextManager::showPlayerDamage(Ogre::MovableObject *mov, unsigned int pDamage, bool pIsCritic)
{
	if(mPlayerOverlayOptions.size() < MAX_QUEUE_CAPACITY)
	{
		TextOverlayOptions options;

		std::stringstream damageText;
		damageText << pDamage << (pIsCritic?"!":"");

		options.caption = damageText.str();
		options.mov = mov;
		options.attributes = mPlayerDamageAttributes;

		mPlayerOverlayOptions.push(options);
	}
}

void iceDamageTextManager::showPlayerShieldDamage(Ogre::MovableObject *mov, unsigned int pDamage, bool pIsCritic)
{
	if(mPlayerOverlayOptions.size() < MAX_QUEUE_CAPACITY)
	{
		TextOverlayOptions options;

		std::stringstream damageText;
		damageText << pDamage << (pIsCritic?"!":"");

		options.caption = damageText.str();
		options.mov = mov;
		options.attributes = mPlayerShieldDamageAttributes;

		mPlayerOverlayOptions.push(options);
	}
}

void iceDamageTextManager::showPlayerHeal(Ogre::MovableObject *mov, unsigned int pHeal)
{
	TextOverlayOptions options;

	std::stringstream damageText;
	damageText << pHeal;

	options.caption = damageText.str();
	options.mov = mov;
	options.attributes = mPlayerHealAttributes;

	mPlayerOverlayOptions.push(options);
}

void iceDamageTextManager::showPlayerMiss(Ogre::MovableObject *mov)
{
	if(mPlayerOverlayOptions.size() < MAX_QUEUE_CAPACITY)
	{
		TextOverlayOptions options;

		options.caption = "Miss!";
		options.mov = mov;
		options.attributes = mPlayerMissAttributes;

		mPlayerOverlayOptions.push(options);
	}
}

void iceDamageTextManager::showPlayerLevelUp(Ogre::MovableObject *mov)
{
	_showMPlayerNotification(mov,"Level Up!");
}

void iceDamageTextManager::showMinigunLevelUp(Ogre::MovableObject *mov)
{
	_showMPlayerNotification(mov,"MiniGun Level Up!");
}

void iceDamageTextManager::showShotgunLevelUp(Ogre::MovableObject *mov)
{
	_showMPlayerNotification(mov,"ShotGun Level Up!");
}

void iceDamageTextManager::showMissileLevelUp(Ogre::MovableObject *mov)
{
	_showMPlayerNotification(mov,"Missile Launcher Level Up!");
}


MovableTextOverlay* iceDamageTextManager::_getNextDisabledOverlayText()
{
	for(unsigned int i=0;i<MAX_OVERLAYS;i++)
	{
		if(!mTextOverlays[i]->isEnabled())
		{
			return mTextOverlays[i];
		}
	}
	return NULL;
}

void iceDamageTextManager::_showMPlayerNotification(Ogre::MovableObject *mov, Ogre::String caption)
{
	TextOverlayOptions options;

	options.caption = caption;
	options.mov = mov;
	options.attributes = mPlayerLevelUpAttributes;

	mPlayerOverlayOptions.push(options);
}