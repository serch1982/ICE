#ifndef __ICE_PHASE2_H__
#define __ICE_PHASE2_H__

#include "icePhase.h"

class icePhase2 : public icePhase
{
	public:
		icePhase2();
		~icePhase2();

		void update(Ogre::Real p_timeSinceLastFrame);

		bool createScene(Ogre::SceneManager* p_SceneMgr, icePlayer* p_psPlayer );

	private:
		void setCameras(void);
		bool isPhaseEnded(void);
};

#endif