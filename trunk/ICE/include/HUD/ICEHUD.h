#ifndef __ICEHUD_h_
#define __ICEHUD_h_

#include <OISMouse.h>
#include <SdkTrays.h>
#include <OgreRenderWindow.h>
#include <vector>
#include <OgreString.h>

class ICEHUD
{
public:
	static ICEHUD* instance();
	~ICEHUD();

	//Methods
	bool setupChivato(Ogre::RenderWindow* mWindow, OIS::Mouse* mMouse,Ogre::WindowEventListener* winListener, OgreBites::SdkTrayListener* sdkListener);
	void updateChivato(const Ogre::FrameEvent& evt, Ogre::String paramName, Ogre::String paramValue);
	void showFrameStats();
protected:
	  ICEHUD();
private:
	static ICEHUD* pinstance;
	OgreBites::SdkTrayManager* mTrayMgr;
	OgreBites::ParamsPanel* mDetailsPanel; 
	vector<char*> vChivatos;
};

#endif