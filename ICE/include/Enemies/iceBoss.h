#ifndef __ICE_BOSS_H__
#define __ICE_BOSS_H__

#include "Enemies\iceEnemy.h"

class iceBoss: public iceEnemy
{

public:
	iceBoss();
	~iceBoss();

	// Overrided functions
	virtual bool initialize(int id, Ogre::Vector3 p_Position, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer = false);
	virtual void finalize();
	virtual void update(Ogre::Real p_timeSinceLastFrame);
	virtual std::string getFunctionStr();
	//virtual void logic();
};

#endif
