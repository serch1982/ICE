#ifndef ICEDAMAGETEXTMANAGER_H_
#define ICEDAMAGETEXTMANAGER_H_

#include <vector>
#include <OGRE/Ogre.h>
#include "MovableTextOverlay.h"

struct TextOverlayOptions
{
	Ogre::String caption;
	Ogre::MovableObject* mov;
	MovableTextOverlayAttributes* attributes;
};


class iceDamageTextManager: public Ogre::Singleton<iceDamageTextManager> {
    public:
        iceDamageTextManager();
        ~iceDamageTextManager();

        static iceDamageTextManager& getSingleton();
        static iceDamageTextManager* getSingletonPtr();

		void update(Ogre::Real pTimeSinceLastFrame);

		void showEnemyDamage(Ogre::MovableObject *mov, unsigned int pDamage, bool pIsCritic);
		void showEnemyMiss(Ogre::MovableObject *mov);
		void showPlayerDamage(Ogre::MovableObject *mov, unsigned int pDamage, bool pIsCritic);
		void showPlayerShieldDamage(Ogre::MovableObject *mov, unsigned int pDamage, bool pIsCritic);
		void showPlayerHeal(Ogre::MovableObject *mov, unsigned int pHeal);
		void showPlayerMiss(Ogre::MovableObject *mov);
		void showPlayerLevelUp(Ogre::MovableObject *mov);

		void showMinigunLevelUp(Ogre::MovableObject *mov);
		void showShotgunLevelUp(Ogre::MovableObject *mov);
		void showMissileLevelUp(Ogre::MovableObject *mov);

		void hideAll();


    private:
		MovableTextOverlay* _getNextDisabledOverlayText();
		std::vector<MovableTextOverlay*> mTextOverlays;
		std::queue<TextOverlayOptions> mPlayerOverlayOptions;
		MovableTextOverlayAttributes* mEnemyDamageAttributes;
		MovableTextOverlayAttributes* mEnemyMissAttributes;
		MovableTextOverlayAttributes* mPlayerDamageAttributes;
		MovableTextOverlayAttributes* mPlayerShieldDamageAttributes;
		MovableTextOverlayAttributes* mPlayerHealAttributes;
		MovableTextOverlayAttributes* mPlayerMissAttributes;
		MovableTextOverlayAttributes* mPlayerLevelUpAttributes;

		Ogre::Real mTimeToNextPlayerLabel;

		void _showMPlayerNotification(Ogre::MovableObject *mov, Ogre::String caption);
};

#endif ICEDAMAGETEXTMANAGER_H_