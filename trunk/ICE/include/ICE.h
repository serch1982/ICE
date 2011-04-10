
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
#include "iceLogicLua.h"
#include <string>
#include <OgreLog.h>
#include <OgreLogManager.h>
#include <stdlib.h>

class ICE : public CORE
{
public:
    ICE(void);
    virtual ~ICE(void);
	
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

	void setCurrentCamera( Ogre::Camera* p_pNewCamera );
	//void updateCameras(const Ogre::FrameEvent& evt);

private:

	ICEMenu* mIceMenu;
	iceLevel mLevel;
	icePlayer mPlayer;
	Ogre::Log* mGameLog;

	int m_iCurrentLevel;

	void update( Ogre::Real p_timeSinceLastFrame );
};

#endif // #ifndef __ICE_H_
