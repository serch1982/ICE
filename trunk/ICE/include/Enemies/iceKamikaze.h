#ifndef __ICE_KAMIKAZE_H__
#define __ICE_KAMIKAZE_H__

#include "Enemies\iceEnemy.h"

class iceKamikaze: public iceEnemy
{

public:
	iceKamikaze();
	~iceKamikaze();

	// Overrided functions
	virtual bool initialize(int id, Ogre::Vector3 p_Position, icePlayer* p_psPlayer, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer = false);
	virtual void finalize();
	virtual void update(Ogre::Real p_timeSinceLastFrame);
	// getter for the LUA function name
	virtual std::string getFunctionStr();
	//virtual void logic();
};

#endif
