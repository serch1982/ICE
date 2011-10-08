#ifndef _ICE_MENU_H_
#define _ICE_MENU_H_

#include <OGRE/Ogre.h>
#include <Hikari.h>
#include "../States/iceStateManager.h"

class iceMenu
{
    public:
        iceMenu(Hikari::HikariManager* pHikariMgr);
		~iceMenu();

		void init(iceStateManager* pStateManager);

		void showMainMenu();
		void showPauseMenu();
		void hide();

		//hikari callbacks
		Hikari::FlashValue exitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);
		Hikari::FlashValue playClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);
		Hikari::FlashValue continueClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);

		//OIS
		bool keyPressed(const OIS::KeyEvent &arg);
		bool keyReleased(const OIS::KeyEvent &arg);
		bool mouseMoved(const OIS::MouseEvent &arg);
		bool mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id);
		bool mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id);

		Hikari::HikariManager* mHikariMgr;
	protected:
		iceStateManager* mStateManager;
		Hikari::FlashControl* mFlash;
};

#endif