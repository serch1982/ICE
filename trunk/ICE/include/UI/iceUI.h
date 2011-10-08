#ifndef _ICE_UI_H_
#define _ICE_UI_H_

#include <OGRE/Ogre.h>
#include <Hikari.h>
#include "../States/iceStateManager.h"
#include "iceMenu.h"
#include "iceHUD.h"

class iceUI
{
    public:
        iceUI();
		~iceUI();

		void init(iceStateManager* pStateManager);
		void update(Ogre::Real evt);

		iceMenu* getMenu();
		iceHUD* getHUD();
		Hikari::HikariManager* getHikari();

	protected:
		iceStateManager* mStateManager;
		Hikari::HikariManager* mHikariMgr;
		iceMenu* mMenu;
		iceHUD* mHUD;
};

#endif