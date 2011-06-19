#ifndef ICEDAMAGETEXTMANAGER_H_
#define ICEDAMAGETEXTMANAGER_H_

#include <vector>
#include <OGRE/Ogre.h>
#include "MovableTextOverlay.h"


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


    private:
		MovableTextOverlay* _getNextDisabledOverlayText();
		std::vector<MovableTextOverlay*> mTextOverlays;
		MovableTextOverlayAttributes* mEnemyDamageAttributes;
		MovableTextOverlayAttributes* mEnemyMissAttributes;
		MovableTextOverlayAttributes* mPlayerDamageAttributes;
		MovableTextOverlayAttributes* mPlayerShieldDamageAttributes;
		MovableTextOverlayAttributes* mPlayerHealAttributes;
		MovableTextOverlayAttributes* mPlayerMissAttributes;
		MovableTextOverlayAttributes* mPlayerLevelUpAttributes;
};

#endif ICEDAMAGETEXTMANAGER_H_