#ifndef __ICE_MINI_H__
#define __ICE_MINI_H__

#include "Enemies\iceEnemy.h"

class iceMini: public iceEnemy
{

public:
	iceMini();
	~iceMini();

	// Overrided functions
	virtual bool initialize(int id, Ogre::Vector3 p_Position, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer = false);
	virtual void finalize();
	virtual void update(Ogre::Real p_timeSinceLastFrame);
	// getter for the LUA function name
	virtual std::string getFunctionStr();
	virtual void setState( ENEMYSTATE pState );
	//virtual void logic();
};

#endif
