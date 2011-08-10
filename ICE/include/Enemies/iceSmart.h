#ifndef __ICE_SMART_H__
#define __ICE_SMART_H__

#include "Enemies\iceEnemy.h"

class iceSmart: public iceEnemy
{

public:
	iceSmart();
	~iceSmart();

	// Overrided functions
	virtual bool initialize(int id, Ogre::Vector3 p_Position,  Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer = false);
	virtual void finalize();
	virtual void update(Ogre::Real p_timeSinceLastFrame);
	// getter for the LUA function name
	virtual std::string getFunctionStr();
	virtual void showReceivedDamage(unsigned int p_iDamage, bool p_bCritical);
	virtual void createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic);
	virtual void changeDirection(void);

private:
	iceParticlePtr mParticleBoom;
	
};

#endif
