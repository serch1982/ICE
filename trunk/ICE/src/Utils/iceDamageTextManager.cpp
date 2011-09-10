#include <OGRE/Ogre.h>
#include "Utils/iceDamageTextManager.h"
#include <iostream>
#include "iceGame.h"

#define MAX_OVERLAYS 20

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
	MovableTextOverlay* textOverlay = _getNextDisabledOverlayText();
	if(textOverlay == NULL)
	{
		return;
	}
	std::stringstream damageText;
	damageText << pDamage << (pIsCritic?"!":"");
	textOverlay->initialize(damageText.str(),mov,mPlayerDamageAttributes);
	textOverlay->enable();
}

void iceDamageTextManager::showPlayerShieldDamage(Ogre::MovableObject *mov, unsigned int pDamage, bool pIsCritic)
{
	MovableTextOverlay* textOverlay = _getNextDisabledOverlayText();
	if(textOverlay == NULL)
	{
		return;
	}
	std::stringstream damageText;
	damageText << pDamage << (pIsCritic?"!":"");
	textOverlay->initialize(damageText.str(),mov,mPlayerShieldDamageAttributes);
	textOverlay->enable();
}

void iceDamageTextManager::showPlayerHeal(Ogre::MovableObject *mov, unsigned int pHeal)
{
	MovableTextOverlay* textOverlay = _getNextDisabledOverlayText();
	if(textOverlay == NULL)
	{
		return;
	}
	std::stringstream damageText;
	damageText << pHeal;
	textOverlay->initialize(damageText.str(),mov,mPlayerHealAttributes);
	textOverlay->enable();
}

void iceDamageTextManager::showPlayerMiss(Ogre::MovableObject *mov)
{
	MovableTextOverlay* textOverlay = _getNextDisabledOverlayText();
	if(textOverlay == NULL)
	{
		return;
	}
	textOverlay->initialize("Miss!",mov,mPlayerMissAttributes);
	textOverlay->enable();
}

void iceDamageTextManager::showPlayerLevelUp(Ogre::MovableObject *mov)
{
	MovableTextOverlay* textOverlay = _getNextDisabledOverlayText();
	if(textOverlay == NULL)
	{
		return;
	}
	textOverlay->initialize("Level Up!",mov,mPlayerLevelUpAttributes);
	textOverlay->enable();
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