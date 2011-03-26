#ifndef __ICE_LEVEL_H__
#define __ICE_LEVEL_H__

#include <OgreSceneManager.h>
#include <OgreLog.h>
#include "iceTrajectory.h"

class iceLevel
{
	public:
		iceLevel();
		~iceLevel();

		bool initialize();
		void finalize();
		void update(Ogre::Real p_timeSinceLastFrame);

		bool createScene( Ogre::SceneManager* p_SceneMgr, int p_iCurrentLevel );
		//bool cleanScene( Ogre::SceneManager* p_SceneMgr );

	private:
		int id;

		Ogre::Log* mLog;

		iceTrajectory mTrajectory;
};

#endif