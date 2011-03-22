#include "iceRPG.h"


iceRPG::iceRPG(void)
{
}


iceRPG::~iceRPG(void)
{
}

//getters
unsigned int iceRPG::getLevel(void)
{
	return mLevel;
}

unsigned int iceRPG::getExperience(void)
{
	return mExperience;
}

unsigned int iceRPG::getMaxLife(void)
{
	return mBaseLife * mLevel;
}

unsigned int iceRPG::getCurrentLife(void)
{
	return mCurrentLife;
}

unsigned int iceRPG::getArmor(void)
{
	return mBaseArmor * mLevel / 5;
}

unsigned int iceRPG::getAttack(void)
{
	return mBaseAttack * mLevel / 3;
}

unsigned int iceRPG::getAccuracy(void)
{
	return mBaseAccuracy * mLevel;
}

unsigned int iceRPG::getManiobrability(void)
{
	return mBaseManiobrability * mLevel;
}

unsigned int iceRPG::getCadence(void)
{
	return mBaseCadence * mLevel;
}

unsigned int iceRPG::getLuck(void)
{
	return mBaseLuck * mLevel / 10;
}

unsigned int iceRPG::getWeaponLevel(unsigned int p_iWeapon)
{
	return mWeaponLevel[p_iWeapon];
}

unsigned int iceRPG::getShieldLevel(void)
{
	return mShieldLevel;
}

unsigned int iceRPG::getMaxShieldEnergy(void)
{
	return getMaxLife() * mShieldLevel / 10;
}
unsigned int iceRPG::getCurrentShieldEnergy(void)
{
	return mShieldEnergy;
}

//setters
void iceRPG::setLevel(unsigned int p_iLevel)
{
	if (p_iLevel < 1)
		p_iLevel = 1;
	else if (p_iLevel > 99)
		p_iLevel = 99;
	mLevel = p_iLevel;
	mExperience = 0;
}

void iceRPG::setExperience(unsigned int p_iExperience)
{
	mExperience = p_iExperience;
}

void iceRPG::setCurrentWeapon(unsigned int p_iWeapon)
{
	mCurrentWeapon = p_iWeapon;
}

//other methods
void iceRPG::levelUp(void)
{
	mLevel++;
	if (mLevel > 99)
		mLevel = 99;
	mExperience = 0;

	//Las armas empiezan a estar disponibles a partir de cierto nivel
	//De momento harcoded:
	
	switch(mLevel)
	{
		case 1:
			mWeaponLevel[MACHINEGUN] = 1;
		case 10:
			mWeaponLevel[SHOTGUN] = 1;
		case 15:
			mShieldLevel = 1;
		case 20:
			mWeaponLevel[MISILE_LAUNCHER] = 1;
		case 30:
			mWeaponLevel[MACHINEGUN] = 2;
		case 40:
			mWeaponLevel[SHOTGUN] = 2;
		case 45:
			mShieldLevel = 2;
		case 50:
			mWeaponLevel[MISILE_LAUNCHER] = 2;
		case 60:
			mWeaponLevel[MACHINEGUN] = 3;
		case 65:
			mShieldLevel = 3;
		case 70:
			mWeaponLevel[SHOTGUN] = 3;
		case 80:
			mWeaponLevel[MISILE_LAUNCHER] = 3;
	}

}
void iceRPG::addExperience(unsigned int p_iExperience)
{
	if (mLevel != 99)
	{
		mExperience += p_iExperience;

		if (mExperience > mLevel*350+300)
		{
			levelUp();	
		}
	}	
}

void iceRPG::addDamage(unsigned int p_iDamage)
{
	int iDamage = p_iDamage - getArmor();
	if (iDamage < MIN_DAMAGE)
		iDamage = MIN_DAMAGE;

	if (iDamage < (int)mCurrentLife)
		mCurrentLife -= iDamage;
	else
	{
		mCurrentLife = 0;
	}
}

void iceRPG::addLife(unsigned int p_iLife)
{
	mCurrentLife += p_iLife;
	if (mCurrentLife > getMaxLife())
		mCurrentLife = getMaxLife();
}

bool iceRPG::isAlive(void)
{
	return (mCurrentLife > 0);
}

void iceRPG::updateTime(Ogre::Real p_fFrameTime)
{
	mTimeSinceLastShot += p_fFrameTime;
}
