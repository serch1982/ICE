#ifndef __ICE_PHASE_H__
#define __ICE_PHASE_H__

#include "icePlayer.h"
#include "iceLocomotiveTrajectory.h"

#include <OgreSceneManager.h>
#include <OgreLog.h>
#include <OgreCamera.h>
#include <vector>
#include "iceState.h"

class icePhase
{
	public:
		icePhase();
		~icePhase();

		bool initialize();
		void finalize();
		virtual void update(Ogre::Real p_timeSinceLastFrame) = 0;

		virtual bool createScene( Ogre::SceneManager* p_SceneMgr, icePlayer* p_psPlayer );
		Ogre::Camera* getCamera( int p_iIndex );
		bool clearScene(void);

	protected:
		virtual void setCameras(void) = 0;
		virtual bool isPhaseEnded(void) = 0;

		Ogre::Log* mLog;
		vector<Ogre::Camera*> mvCameras;
		Ogre::SceneManager* mSceneManager;
		icePlayer* mPlayer;
};

#endif