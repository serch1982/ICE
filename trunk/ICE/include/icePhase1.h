#ifndef __ICE_PHASE1_H__
#define __ICE_PHASE1_H__

#include "icePhase.h"
#include "iceEnemy.h"
#include "iceStep.h"
#include <OgreBillboard.h>

#include <scene\DotSceneLoader.h>
#include "scene/ConfigScriptLoader.h"
#include <OgreQuaternion.h>
#include <stdlib.h>
#include <vector>
#include "iceLogicLua.h"

class icePhase1 : public icePhase
{
	public:
		icePhase1();
		~icePhase1();

		void update(Ogre::Real p_timeSinceLastFrame);

		bool createScene(Ogre::SceneManager* p_SceneMgr, icePlayer* p_psPlayer );
		bool isPhaseEnded(void);

	private:
		void setCameras(void);
};

#endif