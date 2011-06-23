#ifndef __ICE_VOLCANO_H__
#define __ICE_VOLCANO_H__

#include "Enemies\iceEnemy.h"

class iceVolcano: public iceEnemy
{

public:
	iceVolcano();
	~iceVolcano();

	virtual bool initialize(int id, Ogre::Vector3 p_Position, icePlayer* p_psPlayer, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer = false);
	virtual void finalize();
	virtual void update(Ogre::Real p_timeSinceLastFrame);
	//virtual void logic();
};

#endif