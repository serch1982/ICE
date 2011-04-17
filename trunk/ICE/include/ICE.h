
#ifndef __ICE_h_
#define __ICE_h_

#include "CORE.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "../res/resource.h"
#endif

#include "icePhase.h"
#include "icePhase1.h"
#include "icePhase2.h"
#include "icePhase3.h"
#include "icePlayer.h"
#include "iceTrajectory.h"
#include "iceMenu.h"
#include <string>
#include <OgreLog.h>
#include <OgreLogManager.h>
#include <stdlib.h>
#include "iceHUD.h"
#include "icePhysicsMgr.h"

class ICE : public CORE
{
public:
    ICE(void);
    virtual ~ICE(void);
	
protected:
	virtual bool setup();
    virtual void createScene(void);
	
	virtual bool frameStarted( const Ogre::FrameEvent& evt);
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	virtual bool frameEnded( const Ogre::FrameEvent& evt); 
	
	// OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );

	void setCurrentCamera( Ogre::Camera* p_pNewCamera );
	//void updateCameras(const Ogre::FrameEvent& evt);

private:
	// Levels of the game
	std::vector<icePhase*> mPhases;
	// Player
	icePlayer mPlayer;
	// Game Log
	Ogre::Log* mGameLog;
	// Current level
	int m_iCurrentPhase;
	// Physical World
	icePhysicsMgr* mPhysics;

	void update( Ogre::Real p_timeSinceLastFrame );
};

#endif // #ifndef __ICE_H_
