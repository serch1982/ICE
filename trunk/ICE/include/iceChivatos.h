#ifndef __iceChivatos_h_
#define __iceChivatos_h_

#include <OISMouse.h>
#include <SdkTrays.h>
#include <OgreRenderWindow.h>

class iceChivatos
{
public:
	static iceChivatos* instance();
	~iceChivatos();

	//Methods
	void setupChivato(Ogre::RenderWindow* mWindow, OIS::Mouse* mMouse,Ogre::WindowEventListener* winListener, OgreBites::SdkTrayListener* sdkListener);
    void updateChivato( int iparamNumber, Ogre::String sparamValue);
	void updateFrameEvent(const Ogre::FrameEvent& evt);
	void showFrameStats();
    void hideFrameStats();
    void hideChivatos();
    void showChivatos();
    bool isDialogVisible();
protected:
	iceChivatos();

private:
	static iceChivatos* pinstance;
	OgreBites::SdkTrayManager* mTrayMgr;
	OgreBites::ParamsPanel* mDetailsPanel; 
	int iChivatos;
};

#endif