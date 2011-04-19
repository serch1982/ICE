#ifndef __iceHUD_h_
#define __iceHUD_h_


#include "Hikari.h"
#include <OgreViewport.h>
#include <OISMouse.h>
#include "iceMenu.h"

class iceHUD
{
public:
	static iceHUD* getInstance();
	~iceHUD();

	//Methods
	bool setupHUD(char* path, char* name, Ogre::Viewport* mViewport);
	void update();
	void show();
	void hide();

	void setWeapon(char* name);
	void setLife(int life);

	//IOS
	void mouseMoved(const OIS::MouseEvent &arg);
	void mouseDown(OIS::MouseButtonID id);
	void mouseUp(OIS::MouseButtonID id);
protected:
	  iceHUD();
private:
	bool isLoad;
	static iceHUD* pinstance;
	Hikari::HikariManager *hikariMgrHUD;
	Hikari::FlashControl  *hikariHUD;
	Ogre::Log* mGameLog;
};

#endif