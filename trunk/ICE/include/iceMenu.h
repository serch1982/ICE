
#ifndef __iceMenu_h_
#define __iceMenu_h_

#include "Hikari.h"
#include <OgreViewport.h>
#include <OISMouse.h>

class iceMenu
{
public:
	static iceMenu* getInstance();
	~iceMenu();

	//Methods
	bool setupHikari(char* path, char* name, Ogre::Viewport* mViewport);
	void update();
	void show();
	void hide();

	//IOS
	void mouseMoved(const OIS::MouseEvent &arg);
	void mouseDown(OIS::MouseButtonID id);
	void mouseUp(OIS::MouseButtonID id);

	Hikari::HikariManager* getHikariManager();
protected:
	  iceMenu();
private:
	static iceMenu* pinstance;
	Hikari::HikariManager* hikariMgr;
	Hikari::FlashControl  *hikariMenu;

	Hikari::FlashValue menuExitClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue menuPlayClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);
	Hikari::FlashValue menuContinueClick(Hikari::FlashControl* caller, const Hikari::Arguments& args);

	Ogre::Log* mGameLog;
};

#endif