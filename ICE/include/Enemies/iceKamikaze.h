#ifndef __ICE_KAMIKAZE_H__
#define __ICE_KAMIKAZE_H__

#include "Enemies\iceEnemy.h"
#include "Utils\iceStrategy.h"

class iceKamikaze: public iceEnemy
{

public:
	iceKamikaze();
	~iceKamikaze();

	// Overrided functions
	virtual bool initialize(int id, Ogre::Vector3 p_Position, Ogre::Real p_fActivationTime, const bool p_isAttachedToPlayer = false);
	virtual void finalize();
	virtual void update(Ogre::Real p_timeSinceLastFrame);
	// getter for the LUA function name
	virtual std::string getFunctionStr();
	virtual void setState(ENEMYSTATE p_iState);
	virtual void createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic);

	virtual void showReceivedDamage(unsigned int p_iDamage, bool p_bCritical);
	virtual void changeDirection(void);
private:
	Ogre::Vector3 mTargetPosition;
	int	mRenewTarget;
	iceParticlePtr mParticleFire;
	Ogre::Vector3 mVelocity;
};

#endif
