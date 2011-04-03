#include "iceRPG.h"


iceRPG::iceRPG(void)
{
	mLevel = 1;
	mExperience = 0;
	mBaseLife = 10;
	mBaseArmor = 1;
	mBaseAttack = 1;
	mBaseAccuracy = 1;
	mBaseManiobrability = 1;
	mBaseLuck = 10;
	mCurrentLife = getMaxLife();
	mWeaponLevel[MACHINEGUN] = 0;
	mWeaponLevel[SHOTGUN] = 0;
	mWeaponLevel[MISILE_LAUNCHER] = 0;
	mWeaponBaseCadence[MACHINEGUN] = 7;
	mWeaponBaseCadence[SHOTGUN] = 3;
	mWeaponBaseCadence[MISILE_LAUNCHER] = 1;
	mShieldLevel = 0;
	mShieldEnergy = 0;
	mCurrentWeapon = MACHINEGUN;

	mTimeSinceLastShot = 0;
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

Ogre::Real iceRPG::getMaxLife(void)
{
	return Ogre::Real(mBaseLife * mLevel);
}

Ogre::Real iceRPG::getCurrentLife(void)
{
	return mCurrentLife;
}

Ogre::Real iceRPG::getArmor(void)
{
	return Ogre::Real(mBaseArmor * mLevel / 5);
}

Ogre::Real iceRPG::getAttack(void)
{
	return Ogre::Real(mBaseAttack * mLevel / 3);
}

Ogre::Real iceRPG::getAccuracy(void)
{
	return Ogre::Real(mBaseAccuracy * mLevel);
}

Ogre::Real iceRPG::getManiobrability(void)
{
	return Ogre::Real(mBaseManiobrability * mLevel);
}

//unsigned int iceRPG::getCadence(void)
//{
//	return mBaseCadence * mLevel;
//}

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

Ogre::Real iceRPG::getMaxShieldEnergy(void)
{
	return getMaxLife() * mShieldLevel / 10;
}
Ogre::Real iceRPG::getCurrentShieldEnergy(void)
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
	showLevelUp();

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

//void iceRPG::addDamage(Ogre::Real p_iDamage)
//{
//	Ogre::Real iDamage = p_iDamage - getArmor();
//	if (iDamage < MIN_DAMAGE)
//		iDamage = MIN_DAMAGE;
//
//	if (iDamage < (int)mCurrentLife)
//		mCurrentLife -= iDamage;
//	else
//	{
//		mCurrentLife = 0;
//	}
//}

void iceRPG::addLife(Ogre::Real p_iLife)
{
	mCurrentLife += p_iLife;
	if (mCurrentLife > getMaxLife())
		mCurrentLife = getMaxLife();
}

bool iceRPG::isAlive(void)
{
	return (mCurrentLife > 0);
}

void iceRPG::updateRPG(Ogre::Real p_fFrameTime)
{
	mTimeSinceLastShot += p_fFrameTime;
	if(mShieldEnergy < getMaxShieldEnergy())
		mShieldEnergy += 10*mShieldLevel*p_fFrameTime;
	if(mShieldEnergy > getMaxShieldEnergy())
		mShieldEnergy = getMaxShieldEnergy();
}

void iceRPG::shot(void)
{
	goto borram;

	Ogre::Real fTimeBetweenShots =mWeaponLevel[mCurrentWeapon] / mWeaponBaseCadence[mCurrentWeapon];
	fTimeBetweenShots += getModifierByLuck(-fTimeBetweenShots/10,fTimeBetweenShots/10);
		
	if (mTimeSinceLastShot < fTimeBetweenShots)
	{
		unsigned int iBulletsPerShot = 1;
		if (mCurrentWeapon == SHOTGUN)
			iBulletsPerShot = mWeaponLevel[mCurrentWeapon] * 3;

		for (unsigned int i=0;i<iBulletsPerShot;i++)
		{
			Ogre::Real fBaseDamage = getAttack();
			Ogre::Radian fMaxDeviation = Ogre::Radian(1/getAccuracy());
			bool bIsCritic = isCritic();
			switch(mCurrentWeapon)
			{
				case MACHINEGUN:
					fMaxDeviation = fMaxDeviation*2 - Ogre::Radian(Ogre::Real(mWeaponLevel[mCurrentWeapon]/20));
					break;
				case SHOTGUN:
					fMaxDeviation = fMaxDeviation*5 - Ogre::Radian(Ogre::Real(mWeaponLevel[mCurrentWeapon]/50));
					break;
				case MISILE_LAUNCHER:
					fBaseDamage *= 10;
					break;
			}
			Ogre::Real fDamage = 0.0;
			if(bIsCritic)
			{
				fDamage = fBaseDamage * 2;
				fMaxDeviation /= 2;
			}
			else
				fDamage = fBaseDamage + getModifierByLuck(-fBaseDamage/10,fBaseDamage/10);
			
			Ogre::Radian fDeviation = fMaxDeviation + Ogre::Radian(getModifierByLuck(-fMaxDeviation.valueRadians()/10,fMaxDeviation.valueRadians()/10));
			Ogre::Quaternion sOrientation(fDeviation,Ogre::Vector3(Ogre::Math::SymmetricRandom(),Ogre::Math::SymmetricRandom(),0.0));

			createShotEntity(mCurrentWeapon,sOrientation,fDamage,bIsCritic);
		}

		mTimeSinceLastShot = 0.0;
	}

borram:
	createShotEntity(0,Ogre::Quaternion(0,0,0,0),0,false);
}

void iceRPG::addDamage(Ogre::Real p_iDamage, bool p_bCritic)
{
	if(isFail() && !p_bCritic)
	{
		showFail();
	}
	else
	{
		if(mShieldEnergy > 0)
		{
			if(mShieldEnergy >= p_iDamage)
			{
				mShieldEnergy -= p_iDamage;
				showShieldDamage(p_iDamage,p_bCritic);
				p_iDamage = 0;
			}
			else
			{
				p_iDamage -= mShieldEnergy;
				showShieldDamage(mShieldEnergy,p_bCritic);
				mShieldEnergy = 0;
			}
		}
		if(p_iDamage > 0)
		{
			if(!p_bCritic)
				p_iDamage -= getArmor();
			if (p_iDamage < MIN_DAMAGE)
				p_iDamage = MIN_DAMAGE;

			if (p_iDamage < (int)mCurrentLife)
				mCurrentLife -= p_iDamage;
			else
				mCurrentLife = 0;

			showReceivedDamage(p_iDamage,p_bCritic);
		}
	}
}

bool iceRPG::isCritic(void)
{
	return (Ogre::Math::UnitRandom()*(99*100) < getLuck());
}

bool iceRPG::isFail(void)
{
	return (Ogre::Math::UnitRandom()*(99*100) < getLuck());
}

Ogre::Real iceRPG::getModifierByLuck(Ogre::Real p_fMin, Ogre::Real p_fMax)
{
	Ogre::Real fRange = p_fMax - p_fMin;
	Ogre::Real fLuckModifier = Ogre::Real(getLuck())/(99*100);

	Ogre::Real fOffset = p_fMin + fRange*fLuckModifier;
	Ogre::Real fDeviation = 1.0;


	if (fLuckModifier <= 0.5)
		fDeviation = p_fMax - fOffset;
	else
		fDeviation = fOffset - p_fMin;

	Ogre::Real fModifier = randn_notrig(fOffset,fDeviation);
	
	if (fModifier < p_fMin)
		fModifier = p_fMin;
	else if (fModifier > p_fMax)
		fModifier = p_fMax;

	return fModifier;
}

/******************************************************************************/
//	"Polar" version without trigonometric calls
Ogre::Real iceRPG::randn_notrig(Ogre::Real mu, Ogre::Real sigma) {
	static bool deviateAvailable=false;	//	flag
	static Ogre::Real storedDeviate;	//	deviate from previous calculation
	Ogre::Real polar, rsquared, var1, var2;
	
	//	If no deviate has been stored, the polar Box-Muller transformation is 
	//	performed, producing two independent normally-distributed random
	//	deviates.  One is stored for the next round, and one is returned.
	if (!deviateAvailable) {
		
		//	choose pairs of uniformly distributed deviates, discarding those 
		//	that don't fall within the unit circle
		do {
			var1=Ogre::Math::UnitRandom()*2.0f - 1.0f;
			var2=Ogre::Math::UnitRandom()*2.0f - 1.0f;
			rsquared=var1*var1+var2*var2;
		} while ( rsquared>=1.0 || rsquared == 0.0);
		
		//	calculate polar tranformation for each deviate
		polar=Ogre::Real(sqrt(-2.0*log(rsquared)/rsquared));
		
		//	store first deviate and set flag
		storedDeviate=var1*polar;
		deviateAvailable=true;
		
		//	return second deviate
		return var2*polar*sigma + mu;
	}
	
	//	If a deviate is available from a previous call to this function, it is
	//	returned, and the flag is set to false.
	else {
		deviateAvailable=false;
		return storedDeviate*sigma + mu;
	}
}

