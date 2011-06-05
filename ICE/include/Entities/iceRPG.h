#pragma once
#include <OgreVector3.h>

//#include "iceCounters.h"

#define MACHINEGUN	0
#define SHOTGUN		1
#define MISILE_LAUNCHER	2
#define MIN_DAMAGE 10

//Pau * NUMBER OF BULLETS PER WEAPON-----------------------------------------------//
#define BULLET_VECTOR_SIZE		  100
//---------------------------------------------------------------------------------//



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
	Ogre::Real getAccuracy(void);
	Ogre::Real getManiobrability(void);
	//unsigned int getCadence(void);
	unsigned int getLuck(void);
	unsigned int getWeaponLevel(unsigned int p_iWeapon);
	unsigned int getShieldLevel(void);
	unsigned int getMaxShieldEnergy(void);
	unsigned int getCurrentShieldEnergy(void);

	//setters
	void setLevel(unsigned int p_iLevel);
	void setExperience(unsigned int p_iExperience);
	void setCurrentWeapon(unsigned int p_iWeapon);
	virtual void setWeaponLevel(unsigned int p_iWeapon,unsigned int p_iLevel);

	//other methods
	void levelUp(void);
	void addExperience(unsigned int p_iExperience);
	void addLife(unsigned int p_iLife);

	bool isAlive(void);
	virtual void update(Ogre::Real p_fFrameTime);
	void shot(void);
	void addDamage(unsigned int p_iDamage, bool p_bCritic);

	//this methods must be overridden
	virtual void createShotEntity(int p_iWeapon, Ogre::Quaternion p_sOrientation, unsigned int p_iDamage, bool p_bCritic) =0; 
	virtual void showReceivedDamage(unsigned int p_iDamage, bool p_bCritical) =0;
	virtual void showShieldDamage(unsigned int p_iDamage, bool p_bCritical) =0;
	virtual void showFail(void) =0;
	virtual void showLevelUp(unsigned int p_iLevel) =0;

protected:
	bool isCritic(void);
	bool isFail(void);
	Ogre::Real getModifierByLuck(Ogre::Real p_fMin, Ogre::Real p_fMax);
	Ogre::Real randn_notrig(Ogre::Real mu=0.0, Ogre::Real sigma=1.0);

	//attributes
	unsigned int mLevel;
	unsigned int mExperience;
	unsigned int mBaseLife;
	unsigned int mBaseArmor;
	unsigned int mBaseAttack;
	unsigned int mBaseAccuracy;
	unsigned int mBaseManiobrability;
	//unsigned int mBaseCadence;
	unsigned int mBaseLuck;
	unsigned int mCurrentLife;
	unsigned int mWeaponLevel[3];
	Ogre::Real mWeaponBaseCadence[3];
	unsigned int mShieldLevel;
	unsigned int mShieldEnergy;
	unsigned int mCurrentWeapon;

	Ogre::Real mTimeSinceLastShot;
};

