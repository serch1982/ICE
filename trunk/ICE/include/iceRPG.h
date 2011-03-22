#pragma once
#include <OgreVector3.h>

#define MACHINEGUN	0
#define SHOTGUN		1
#define MISILE_LAUNCHER	2

#define MIN_DAMAGE 10

class iceRPG
{
public:
	iceRPG(void);
	virtual ~iceRPG(void);

	//getters
	unsigned int getLevel(void);
	unsigned int getExperience(void);
	unsigned int getMaxLife(void);
	unsigned int getCurrentLife(void);
	unsigned int getArmor(void);
	unsigned int getAttack(void);
	unsigned int getAccuracy(void);
	unsigned int getManiobrability(void);
	unsigned int getCadence(void);
	unsigned int getLuck(void);
	unsigned int getWeaponLevel(unsigned int p_iWeapon);
	unsigned int getShieldLevel(void);
	unsigned int getMaxShieldEnergy(void);
	unsigned int getCurrentShieldEnergy(void);

	//setters
	void setLevel(unsigned int p_iLevel);
	void setExperience(unsigned int p_iExperience);
	void setCurrentWeapon(unsigned int p_iWeapon);

	//other methods
	void levelUp(void);
	void addExperience(unsigned int p_iExperience);
	void addDamage(unsigned int p_iDamage); //Raw damage, the enemy attack
	void addLife(unsigned int p_iLife);

	bool isAlive(void);
	void updateTime(Ogre::Real p_fFrameTime);

	virtual void createShotEntity(int p_iWeapon, Ogre::Vector3 p_sDirection, unsigned int p_iDamage) =0; //this method must be overridden

protected:
	//attributes
	unsigned int mLevel;
	unsigned int mExperience;
	unsigned int mBaseLife;
	unsigned int mBaseArmor;
	unsigned int mBaseAttack;
	unsigned int mBaseAccuracy;
	unsigned int mBaseManiobrability;
	unsigned int mBaseCadence;
	unsigned int mBaseLuck;
	unsigned int mCurrentLife;
	unsigned int mWeaponLevel[3];
	unsigned int mShieldLevel;
	unsigned int mShieldEnergy;
	unsigned int mCurrentWeapon;

	Ogre::Real mTimeSinceLastShot;

};

