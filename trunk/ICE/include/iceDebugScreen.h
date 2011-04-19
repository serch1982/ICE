#ifndef __iceDebugScreen_h_
#define __iceDebugScreen_h_

#include <OISMouse.h>
#include <SdkTrays.h>
#include <OgreRenderWindow.h>

class iceDebugScreen
{
public:
	static iceDebugScreen* getInstance();
	~iceDebugScreen();

	//Methods
	void setupChivato(Ogre::RenderWindow* mWindow, OIS::Mouse* mMouse,Ogre::WindowEventListener* winListener, OgreBites::SdkTrayListener* sdkListener);
    void updateChivato( int iparamNumber, Ogre::String sparamValue);
	void updateFrameEvent(const Ogre::FrameEvent& evt);
	void showFrameStats();
    void hideFrameStats();
    void hideChivatos();
    void showChivatos();
    bool isDialogVisible();
	void showCursor();
	void hideCursor();  
	void moveMouse(const OIS::MouseEvent &evt);
protected:
	iceDebugScreen();

private:
	static iceDebugScreen* pinstance;
	OgreBites::SdkTrayManager* mTrayMgr;
	OgreBites::ParamsPanel* mDetailsPanel; 
	int iChivatos;
};

#endif