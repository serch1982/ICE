#pragma once
#include <OgreVector3.h>

#define MACHINEGUN	0
#define SHOTGUN		1
#define MISILE_LAUNCHER	2
#define MIN_DAMAGE 10

//Pau * INITIAL AMMO PER WEAPON----------------------------------------------------//
#define MACHINEGUN_RELOAD		  20	// Machinegun's ammo number per reload
#define SHOTGUN_RELOAD			  15	// Machinegun's ammo number per reload
#define MISILE_LAUNCHER_RELOAD	  10	// Machinegun's ammo number per reload
//---------------------------------------------------------------------------------//



class iceRPG
{
public:
	iceRPG(void);
	virtual ~iceRPG(void);

	//getters
	unsigned int getLevel(void);
	unsigned int getExperience(void);
	Ogre::Real getMaxLife(void);
	Ogre::Real getCurrentLife(void);
	Ogre::Real getArmor(void);
	Ogre::Real getAttack(void);
	Ogre::Real getAccuracy(void);
	Ogre::Real getManiobrability(void);
	//unsigned int getCadence(void);
	unsigned int getLuck(void);
	unsigned int getWeaponLevel(unsigned int p_iWeapon);
	unsigned int getShieldLevel(void);
	Ogre::Real getMaxShieldEnergy(void);
	Ogre::Real getCurrentShieldEnergy(void);

	//setters
	void setLevel(unsigned int p_iLevel);
	void setExperience(unsigned int p_iExperience);
	void setCurrentWeapon(unsigned int p_iWeapon);

	//other methods
	void levelUp(void);
	void addExperience(unsigned int p_iExperience);
	//void addDamage(Ogre::Real p_iDamage); //Raw damage, the enemy attack
	void addLife(Ogre::Real p_iLife);

	bool isAlive(void);
	void updateRPG(Ogre::Real p_fFrameTime);
	void shot(void);
	void addDamage(Ogre::Real p_iDamage, bool p_bCritic);

	//this methods must be overridden
	virtual void createShotEntity(int p_iWeapon, Ogre::Quaternion p_sOrientation, Ogre::Real p_iDamage, bool p_bCritic) =0; 
	virtual void showReceivedDamage(Ogre::Real p_iDamage, bool p_bCritical) =0;
	virtual void showShieldDamage(Ogre::Real p_iDamage, bool p_bCritical) =0;
	virtual void showFail(void) =0;
	virtual void showLevelUp(void) =0;

protected:
	bool isCritic(void);
	bool isFail(void);
	Ogre::Real getModifierByLuck(Ogre::Real p_fMin, Ogre::Real p_fMax);
	Ogre::Real randn_notrig(Ogre::Real mu=0.0, Ogre::Real sigma=1.0);

	//attributes
	unsigned int mMachinegunAmmo;		// Current machinegun ammo number
	unsigned int mShotgunAmmo;			// Current shotgun ammo number
	unsigned int mMisileLauncherAmmo;	// Current misile launcher ammo number

	unsigned int mLevel;
	unsigned int mExperience;
	unsigned int mBaseLife;
	unsigned int mBaseArmor;
	unsigned int mBaseAttack;
	unsigned int mBaseAccuracy;
	unsigned int mBaseManiobrability;
	//unsigned int mBaseCadence;
	unsigned int mBaseLuck;
	Ogre::Real mCurrentLife;
	unsigned int mWeaponLevel[3];
	Ogre::Real mWeaponBaseCadence[3];
	unsigned int mShieldLevel;
	Ogre::Real mShieldEnergy;
	unsigned int mCurrentWeapon;

	Ogre::Real mTimeSinceLastShot;

};

