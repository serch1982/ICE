#ifndef __ICEMenu_h_
#define __ICEMenu_h_


#include "Hikari.h"
#include <OgreViewport.h>
#include <OISMouse.h>

class ICEMenu
{
public:
	//CONSTRUCTOR
	ICEMenu(void);
	~ICEMenu(void);

	//MENU STATES
	enum MENUSTATE{
		MENU,
		PLAY,
		CONTINUE,
		EXIT
	};
	
	//Methods
	void setupHikari(char* path, char* name, Ogre::Viewport* mViewport, int wight, int height);
	void update();
	MENUSTATE getState();
	void showMenu();

	//IOS
	void mouseMoved(const OIS::MouseEvent &arg);
	void mouseDown(OIS::MouseButtonID id);
	void mouseUp(OIS::MouseButtonID id);

private:
	MENUSTATE mMENUSTATE;
	Hikari::HikariManager* hikariMgr;
	Hikari::FlashControl  *hikariMenu;

	Hikari::FlashValue menuExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue menuPlayClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue menuContinueClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);

	
};

#endif