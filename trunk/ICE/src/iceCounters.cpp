#include "iceCounters.h"

iceCounters* iceCounters::mInstance = 0;
iceCounters* iceCounters::instance()
{
	if (mInstance == 0)
          mInstance = new iceCounters;
    return mInstance;
}


iceCounters::iceCounters(void)
{
	for (unsigned int i=0;i<ACHIEVEMENTS_NUMBER;i++)
		mCompletedAchievements[i] = false;

	mAchievementsName.resize(ACHIEVEMENTS_NUMBER);
	
	mAchievementsName[0]  = "World 1 Sapiens";
	mAchievementsName[1]  = "World 2 Sapiens";
	mAchievementsName[2]  = "World 3 Sapiens";
	mAchievementsName[3]  = "The End";
	mAchievementsName[4]  = "Game Master";
	mAchievementsName[5]  = "I Love the MachineGun";
	mAchievementsName[6]  = "I Love the ShotGun";
	mAchievementsName[7]  = "I Love the Misile Launcher";
	mAchievementsName[8]  = "NonViolent Player";
	mAchievementsName[9]  = "Habilities are for Noobs";
	mAchievementsName[10] = "Untouched";
	mAchievementsName[11] = "IceFighter Noob";
	mAchievementsName[12] = "IceFighter Intermediate";
	mAchievementsName[13] = "IceFighter Advanced";
	mAchievementsName[14] = "IceFighter Master";
	mAchievementsName[15] = "MachineGun Master";
	mAchievementsName[16] = "ShotGun Master";
	mAchievementsName[17] = "Misile Launcher Master";
	mAchievementsName[18] = "Good Accuracy";
	mAchievementsName[19] = "Awesome Accuracy";
	mAchievementsName[20] = "Accuracy Master";
	mAchievementsName[21] = "Lucky Ice";
	mAchievementsName[22] = "Ninja";

	mPlayerLevel = 0;
	mMaxLevelCompleted = 0;
	mCriticalHits = 0;
	mBulletsEvaded = 0;

	mCurrentLevel = 0;
	mReceivedDamage = 0;
	mCurrentHits = 0;
	mMaxHits = 0;

	for (unsigned int i=0;i<3;i++)
	{
		mWeaponLevel[i] = 0;
		mProducedDamageByWeapon[i] = 0;
		mEnemiesKilledByWeapon[i] = 0;
		mProducedDamageByHability[i] = 0;
		mEnemiesKilledByHability[i] = 0;
		mHabilitiesUsed[i] = 0; 

	}
}

iceCounters::~iceCounters(void)
{
}

void iceCounters::activateAchievement(unsigned int p_iAchievementIndex)
{
	if(!mCompletedAchievements[p_iAchievementIndex])
	{
		mCompletedAchievements[p_iAchievementIndex] = true;
		showAchievement(p_iAchievementIndex);
	}
}

void iceCounters::showAchievement(unsigned int p_iAchievementIndex)
{
	mLog = Ogre::LogManager::getSingletonPtr()->getLog( "iceLog.log" );
	stringstream strMessage;
	strMessage << "New Achievement(" << p_iAchievementIndex << "): " << mAchievementsName[p_iAchievementIndex];
	mLog->logMessage( strMessage.str() );
}

void iceCounters::levelCompleted(void)
{
	activateAchievement(mCurrentLevel-1);
	if (mCurrentLevel == 3)
		activateAchievement(3);

	if (mProducedDamageByWeapon[MACHINEGUN]>0 &&
		mProducedDamageByWeapon[SHOTGUN]==0 &&
		mProducedDamageByWeapon[MISILE_LAUNCHER]==0)
		activateAchievement(5);

	if (mProducedDamageByWeapon[MACHINEGUN]==0 &&
		mProducedDamageByWeapon[SHOTGUN]>0 &&
		mProducedDamageByWeapon[MISILE_LAUNCHER]==0)
		activateAchievement(6);
	
	if (mProducedDamageByWeapon[MACHINEGUN]==0 &&
		mProducedDamageByWeapon[SHOTGUN]==0 &&
		mProducedDamageByWeapon[MISILE_LAUNCHER]>0)
		activateAchievement(7);
	
	if (mProducedDamageByWeapon[MACHINEGUN]==0 &&
		mProducedDamageByWeapon[SHOTGUN]==0 &&
		mProducedDamageByWeapon[MISILE_LAUNCHER]==0)
		activateAchievement(8);

	if (mProducedDamageByHability[0]==0 &&
		mProducedDamageByHability[1]==0 &&
		mProducedDamageByHability[2]==0)
		activateAchievement(9);

	if (mReceivedDamage == 0)
		activateAchievement(10);
}

void iceCounters::newLevel(unsigned int p_iLevel)
{
	mCurrentLevel = p_iLevel;
	mReceivedDamage = 0;
	mCurrentHits = 0;
	mMaxHits = 0;


	for (unsigned int i=0;i<3;i++)
	{
		mProducedDamageByWeapon[i] = 0;
		mEnemiesKilledByWeapon[i] = 0;
		mProducedDamageByHability[i] = 0;
		mEnemiesKilledByHability[i] = 0;
		mHabilitiesUsed[i] = 0; 

	}
}

void iceCounters::addReceivedDamage(unsigned int p_iDamage)
{
	mReceivedDamage += p_iDamage;
}

void iceCounters::setPlayerLevel(unsigned int p_iPlayerLevel)
{
	mPlayerLevel = p_iPlayerLevel;

	if(mPlayerLevel >= 10)
		activateAchievement(11);
	if(mPlayerLevel >= 30)
		activateAchievement(12);
	if(mPlayerLevel >= 60)
		activateAchievement(13);
	if(mPlayerLevel == 99)
		activateAchievement(14);
}

void iceCounters::setWeaponLevel(unsigned int p_iWeapon, unsigned int p_iWeaponLevel)
{
	mWeaponLevel[p_iWeapon] = p_iWeaponLevel;
	if (mWeaponLevel[p_iWeapon] == 3)
		activateAchievement(15 + p_iWeapon);
}

void iceCounters::addCriticalHit(void)
{
	mCriticalHits++;
	if (mCriticalHits == 100)
		activateAchievement(21);
}

void iceCounters::addBulletEvaded(void)
{
	mBulletsEvaded++;
	if(mBulletsEvaded == 100)
		activateAchievement(22);
}

void iceCounters::addProducedDamageByWeapon(unsigned int p_iWeapon, unsigned int p_iDamage)
{
	mProducedDamageByWeapon[p_iWeapon] += p_iDamage;
}

void iceCounters::addEnemiesKilledByWeapon(unsigned int p_iWeapon)
{
	mEnemiesKilledByWeapon[p_iWeapon]++;
}

void iceCounters::addProducedDamageByHability(unsigned int p_iHability, unsigned int p_iDamage)
{
	mProducedDamageByHability[p_iHability] += p_iDamage;
}

void iceCounters::addEnemiesKilledByHability(unsigned int p_iHability)
{
	mEnemiesKilledByHability[p_iHability]++;
}

void iceCounters::habilityUsed(unsigned int p_iHability)
{
	mHabilitiesUsed[p_iHability]++;
}

void iceCounters::addHit(void)
{
	mCurrentHits++;

	if(mCurrentHits == 20)
		activateAchievement(18);
	if(mCurrentHits == 50)
		activateAchievement(19);
	if(mCurrentHits == 100)
		activateAchievement(20);
}

void iceCounters::resetHits(void)
{
	mCurrentHits = 0;
}
