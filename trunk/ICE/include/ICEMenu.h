#ifndef __ICEMenu_h_
#define __ICEMenu_h_


#include "Hikari.h"
#include <OgreViewport.h>
#include <OISMouse.h>

class ICEMenu
{
public:
	//MENU STATES
	enum MENUSTATE{
		MENU,
		PLAY,
		GOD,
		CONTINUE,
		GAMEOVER,
		EXIT
	};
	static ICEMenu* instance();
	~ICEMenu();

	//Methods
	bool setupHikari(char* path, char* name, Ogre::Viewport* mViewport, int wight, int height);
	void update();
	MENUSTATE getState();
	void setState(ICEMenu::MENUSTATE state);

	//IOS
	void mouseMoved(const OIS::MouseEvent &arg);
	void mouseDown(OIS::MouseButtonID id);
	void mouseUp(OIS::MouseButtonID id);
protected:
	  ICEMenu();
private:
	static ICEMenu* pinstance;
	MENUSTATE mMENUSTATE;
	Hikari::HikariManager* hikariMgr;
	Hikari::FlashControl  *hikariMenu;

	Hikari::FlashValue menuExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue menuPlayClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue menuContinueClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);

	
};

#endif