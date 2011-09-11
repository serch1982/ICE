#ifndef __ICE_VOLCANO_H__
#define __ICE_VOLCANO_H__

#include "Enemies\iceEnemy.h"

class iceVolcano: public iceEnemy
{

public:
	iceVolcano();
	~iceVolcano();

	// Overrided functions
	virtual bool initialize(int id, Ogre::Vector3 p_Position,  Ogre::Real p_fActivationTime, Ogre::Vector3 p_Scale = Ogre::Vector3::UNIT_SCALE, Ogre::Quaternion rotation = Ogre::Quaternion::IDENTITY, const bool p_isAttachedToPlayer = false);
	virtual void finalize();
	virtual void update(Ogre::Real p_timeSinceLastFrame);
	// getter for the LUA function name
	virtual std::string getFunctionStr();
	virtual void createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic);

	//detect collision with the player against the lava
	bool detectLavaCollision(Ogre::AxisAlignedBox pbox);
	
	virtual void showReceivedDamage(unsigned int p_iDamage, bool p_bCritical);

private:
	Ogre::SceneNode* mLavaNode;
	Ogre::Entity* mLavaMesh;
	bool mLavaCreated;
	unsigned int mAttackDamage;
	bool mAttackIsCritic;
	Ogre::Real mLavaTime;
	Ogre::Real mTimeToNextAtack;
	Ogre::Vector3 mLavaInitialPosition;
};

#endif