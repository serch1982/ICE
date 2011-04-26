#ifndef __ICE_PHASE3_H__
#define __ICE_PHASE3_H__

#include "icePhase.h"

class icePhase3 : public icePhase
{
	public:
		icePhase3();
		~icePhase3();

		void update(Ogre::Real p_timeSinceLastFrame);

		bool createScene(Ogre::SceneManager* p_SceneMgr, icePlayer* p_psPlayer );
		bool isPhaseEnded(void);
	private:
		void setCameras(void);
		vector<iceStep> mEndTrajectorySteps;

};

#endif