#ifndef ICESDKTRAY_h_
#define ICESDKTRAY_h_

#include <OISMouse.h>
#include <SdkTrays.h>
#include <OgreRenderWindow.h>

class iceSdkTray
{
public:
	static iceSdkTray* getInstance();
	~iceSdkTray();

	//Methods
	/**
    *  setup information to show 
    */
	void setupScreenInfo(Ogre::RenderWindow* mWindow, OIS::Mouse* mMouse,Ogre::WindowEventListener* winListener);
	/**
    *  update information to show 
    */
    void updateScreenInfo( int iparamNumber, Ogre::String sparamValue);
	/**
    *  update  frame event
    */
	void updateFrameEvent(const Ogre::FrameEvent& evt);
	/**
    *  show frame stats
    */
	void showFrameStats();
    /**
    *  hide frame stats
    */
	void hideFrameStats();
    /**
    *  hide info
    */
	void hideInfo();
    /**
    *  show info
    */
	void showInfo();
	/**
    *  is sdktray dialog is visible
    */
    bool isDialogVisible();
	/**
    *  show cursor sdktray
    */
	void showCursor();
	/**
    * hide cursor sdktray
    */
	void hideCursor();  
	/**
    *  update mouse move for sdktray
    */
	void moveMouse(const OIS::MouseEvent &evt);
protected:
	/**
    *  private constructor 
    */
	iceSdkTray();

private:
	/**
    *  instance
    */
	static iceSdkTray* pinstance;

	//
	OgreBites::SdkTrayManager* mTrayMgr;
	OgreBites::ParamsPanel* mDetailsPanel; 
	int iItems;
};

#endif