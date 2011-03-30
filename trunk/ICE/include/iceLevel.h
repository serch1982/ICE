#ifndef __ICE_LEVEL_H__
#define __ICE_LEVEL_H__

#include "iceTrajectory.h"

#include <OgreSceneManager.h>
#include <OgreLog.h>
#include <OgreCamera.h>
#include <vector>

class iceLevel
{
	public:
		iceLevel();
		~iceLevel();

		bool initialize();
		void finalize();
		void update(Ogre::Real p_timeSinceLastFrame);

		bool createScene( Ogre::SceneManager* p_SceneMgr, int p_iCurrentLevel );
		Ogre::Camera* getCamera( int p_iIndex );
		//bool cleanScene( Ogre::SceneManager* p_SceneMgr );

	private:
		void setCameras();
		
		int id;

		Ogre::Log* mLog;
		vector<Ogre::Camera*> mvCameras;

		iceTrajectory mTrajectory;
};

#endif