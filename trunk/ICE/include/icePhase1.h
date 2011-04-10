#ifndef __ICE_PHASE1_H__
#define __ICE_PHASE1_H__

#include "icePhase.h"
#include "iceEnemy.h"

class icePhase1 : public icePhase
{
	public:
		icePhase1();
		~icePhase1();

		void update(Ogre::Real p_timeSinceLastFrame);

		bool createScene(Ogre::SceneManager* p_SceneMgr, icePlayer* p_psPlayer );

	private:
		void setCameras(void);
		bool isPhaseEnded(void);
		iceEnemy mEnemy;
};

#endif