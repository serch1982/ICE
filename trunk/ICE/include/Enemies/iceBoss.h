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
	virtual void createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic);
	virtual void activate(void);

	void addSoftPhysicObject(Ogre::Entity* pEntity);
	void addHardPhysicObject(Ogre::Entity* pEntity);
	void addAttackPhysicObject(Ogre::Entity* pEntity);

	std::vector<Ogre::Entity*> getSoftPhysicObjects(void);
	std::vector<Ogre::Entity*> getHardPhysicObjects(void);
	std::vector<Ogre::Entity*> getAttackPhysicObjects(void);

	void addSoftDamage(unsigned int p_iDamage, bool p_bCritic);
	void addHardDamage(unsigned int p_iDamage, bool p_bCritic);

	private:

	std::vector<Ogre::Entity*> mSoftPhysicObjects;
	std::vector<Ogre::Entity*> mHardPhysicObjects;
	std::vector<Ogre::Entity*> mAttackPhysicObjects;
};

#endif
