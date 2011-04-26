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
	mWeaponBaseCadence[MACHINEGUN] = 10;
	mWeaponBaseCadence[SHOTGUN] = 5;
	mWeaponBaseCadence[MISILE_LAUNCHER] = 2;
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

void iceRPG::setWeaponLevel(unsigned int p_iWeapon,unsigned int p_iLevel)
{
	mWeaponLevel[p_iWeapon] = p_iLevel;
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
	showLevelUp(mLevel);

	if (mLevel == 10)
		setWeaponLevel(SHOTGUN,1);
	if (mLevel == 15)
		mShieldLevel = 1;
	if (mLevel == 20)
		setWeaponLevel(MISILE_LAUNCHER,1);
	if (mLevel == 30)
		setWeaponLevel(MACHINEGUN,2);
	if (mLevel == 40)
		setWeaponLevel(SHOTGUN,2);
	if (mLevel == 45)
		mShieldLevel = 2;
	if (mLevel == 50)
		setWeaponLevel(MISILE_LAUNCHER,2);
	if (mLevel == 60)
		setWeaponLevel(MACHINEGUN,3);
	if (mLevel == 65)
		mShieldLevel = 3;
	if (mLevel == 70)
		setWeaponLevel(SHOTGUN,3);
	if (mLevel == 80)
		setWeaponLevel(MISILE_LAUNCHER,3);
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

void iceRPG::update(Ogre::Real p_fFrameTime)
{
	mTimeSinceLastShot += p_fFrameTime;
	if(mShieldEnergy < getMaxShieldEnergy())
		mShieldEnergy += (unsigned int)(10*mShieldLevel*p_fFrameTime);
	if(mShieldEnergy > getMaxShieldEnergy())
		mShieldEnergy = getMaxShieldEnergy();
}

void iceRPG::shot(void)
{
	//goto borram;
	Ogre::Real rd =(mWeaponLevel[mCurrentWeapon] * mWeaponBaseCadence[mCurrentWeapon]);
	if (rd == 0) rd =1;
	Ogre::Real fTimeBetweenShots = 1 / rd;
	fTimeBetweenShots += getModifierByLuck(-fTimeBetweenShots/10,fTimeBetweenShots/10);
		
	if (mTimeSinceLastShot > fTimeBetweenShots)
	{
		unsigned int iBulletsPerShot = 1;
		if (mCurrentWeapon == SHOTGUN)
			iBulletsPerShot = mWeaponLevel[mCurrentWeapon] * 3;

		for (unsigned int i=0;i<iBulletsPerShot;i++)
		{
			unsigned int fBaseDamage = getAttack();
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
			unsigned int fDamage = 0;
			if(bIsCritic)
			{
				fDamage = fBaseDamage * 2;
				fMaxDeviation /= 2;
			}
			else
				fDamage = fBaseDamage + (unsigned int)getModifierByLuck(-(Ogre::Real)fBaseDamage/10,(Ogre::Real)fBaseDamage/10);
			
			Ogre::Radian fDeviation = fMaxDeviation + Ogre::Radian(getModifierByLuck(-fMaxDeviation.valueRadians()/10,fMaxDeviation.valueRadians()/10));
			Ogre::Quaternion sOrientation(fDeviation,Ogre::Vector3(Ogre::Math::SymmetricRandom(),Ogre::Math::SymmetricRandom(),0.0));
			//Ogre::Quaternion sOrientation = Ogre::Quaternion::IDENTITY;
			
			createShotEntity(mCurrentWeapon,sOrientation,fDamage,bIsCritic);
		}

		mTimeSinceLastShot = 0.0;
	}

//borram:
//	createShotEntity(mCurrentWeapon,Ogre::Quaternion(0,0,0,0),0,false);
}

void iceRPG::addDamage(unsigned int p_iDamage, bool p_bCritic)
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

