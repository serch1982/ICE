#ifndef _ICE_HUD_H_
#define _ICE_HUD_H_

#include <OGRE/Ogre.h>
#include <Hikari.h>
#include "../States/iceStateManager.h"

class iceHUD
{
    public:
        iceHUD(Hikari::HikariManager* pHikariMgr);
		~iceHUD();

		void init(iceStateManager* pStateManager);

		void show();
		void hide();

	protected:
		iceStateManager* mStateManager;
		Hikari::HikariManager* mHikariMgr;
		Hikari::FlashControl* mFlash;
};

#endif