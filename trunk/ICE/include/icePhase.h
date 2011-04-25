#ifndef __ICE_PHASE_H__
#define __ICE_PHASE_H__

#include "icePlayer.h"
#include "iceLocomotiveTrajectory.h"
#include "scene/ConfigScriptLoader.h"

#include <OgreSceneManager.h>
#include <OgreLog.h>
#include <OgreCamera.h>
#include <vector>
#include "iceState.h"
#include "iceEnemy.h"

class icePhase
{
	public:
		icePhase();
		~icePhase();

		bool initialize();
		void finalize();
		virtual void update(Ogre::Real p_timeSinceLastFrame);

		virtual bool createScene( Ogre::SceneManager* p_SceneMgr, icePlayer* p_psPlayer );
		Ogre::Camera* getCamera( int p_iIndex );
		bool clearScene(void);
		virtual bool isPhaseEnded(void) = 0;
		vector<iceEnemy*>& getEnemies();

	protected:
		virtual void setCameras(void) = 0;
		vector<iceStep> getStepsFromResources(ConfigNode* p_sStepsNode);
		void loadEnemies(ConfigNode* p_sEnemiesNode);

		Ogre::Log* mLog;
		vector<Ogre::Camera*> mvCameras;
		Ogre::SceneManager* mSceneManager;
		icePlayer* mPlayer;

		//Enemies
		vector<iceEnemy*> mEnemies;
};

#endif