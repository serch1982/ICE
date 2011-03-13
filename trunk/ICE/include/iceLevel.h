#ifndef __ICE_LEVEL_H__
#define __ICE_LEVEL_H__

#include <OgreSceneManager.h>

class iceLevel
{
	public:
		iceLevel();
		~iceLevel();

		bool initialize();
		void finalize();
		void update();

		bool createScene( Ogre::SceneManager* a_SceneMgr );

	private:
		int id;
};

#endif