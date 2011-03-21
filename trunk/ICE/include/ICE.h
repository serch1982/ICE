/*
-----------------------------------------------------------------------------
Filename:    ICE.h
-----------------------------------------------------------------------------


This source file is generated by the
   ___                   _              __    __ _                  _ 
  /___\__ _ _ __ ___    /_\  _ __  _ __/ / /\ \ (_)______ _ _ __ __| |
 //  // _` | '__/ _ \  //_\\| '_ \| '_ \ \/  \/ / |_  / _` | '__/ _` |
/ \_// (_| | | |  __/ /  _  \ |_) | |_) \  /\  /| |/ / (_| | | | (_| |
\___/ \__, |_|  \___| \_/ \_/ .__/| .__/ \/  \/ |_/___\__,_|_|  \__,_|
      |___/                 |_|   |_|                                 
      Ogre 1.7.x Application Wizard for VC10 (August 2010)
      http://code.google.com/p/ogreappwizards/
-----------------------------------------------------------------------------
*/
#ifndef __ICE_h_
#define __ICE_h_

#include "CORE.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif

#include "iceLevel.h"
#include "icePlayer.h"
#include "iceTrajectory.h"
#include "ICEMenu.h"

class ICE : public CORE
{
public:
    ICE(void);
    virtual ~ICE(void);

	static ICE* getInstance();
	//Deleting the instance
	static void deleteInstance(){
		if( pinstance != NULL ) 
			delete pinstance;
	}

protected:
	virtual bool setup();
    virtual void createScene(void);
	
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	
	// OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	//void setCurrentCamera(unsigned int cameraIndex);
	//void updateCameras(const Ogre::FrameEvent& evt);

private:

	//Singleton
	static ICE* pinstance;

	ICEMenu* mIceMenu;
	iceLevel mLevel;
	icePlayer mPlayer;
	iceTrajectory mTrajectory;

	//Game States
	enum STATE{
		MENU,
		PLAY,
		PAUSE,
		LOAD,
		GAME_OVER,
		GOD
	} eState;

	int m_iCurrentLevel;
};

#endif // #ifndef __ICE_H_
