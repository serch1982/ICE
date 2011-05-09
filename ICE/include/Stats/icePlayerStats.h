#ifndef ICEPLAYERSTATS_h_
#define ICEPLAYERSTATS_h_

#include <vector>
#include <string>
#include <vector>
#include <OgreSceneManager.h>
#include <OgreLog.h>

#define MACHINEGUN	0
#define SHOTGUN		1
#define MISILE_LAUNCHER	2

#define ACHIEVEMENTS_NUMBER 23

class icePlayerStats
{
public:
	static icePlayerStats* getInstance();
	~icePlayerStats();

	//Methods
	void activateAchievement(unsigned int p_iAchievementIndex);
	void showAchievement(unsigned int p_iAchievementIndex);

	void newLevel(unsigned int p_iLevel);//falta por llamar
	void levelCompleted(void);//falta por llamar
	void addReceivedDamage(unsigned int p_iDamage);
	void setPlayerLevel(unsigned int p_iPlayerLevel);
	void setWeaponLevel(unsigned int p_iWeapon, unsigned int p_iWeaponLevel);
	void addCriticalHit(void);//falta por llamar
	void addBulletEvaded(void);

	void addProducedDamageByWeapon(unsigned int p_iWeapon, unsigned int p_iDamage);//falta por llamar
	void addEnemiesKilledByWeapon(unsigned int p_iWeapon);//falta por llamar
	void addProducedDamageByHability(unsigned int p_iHability, unsigned int p_iDamage);//falta por llamar
	void addEnemiesKilledByHability(unsigned int p_iHability);//falta por llamar
	void habilityUsed(unsigned int p_iHability);//falta por llamar
	void addHit(void);//falta por llamar
	void resetHits(void);//falta por llamar


protected:
	icePlayerStats();
	//Achievements
	bool mCompletedAchievements[ACHIEVEMENTS_NUMBER];
	std::vector<std::string> mAchievementsName;
	Ogre::Log* mLog;

	//Contadores GLOBALES
	unsigned int mPlayerLevel;
	unsigned int mWeaponLevel[3];
	unsigned int mMaxLevelCompleted; //Referido a escenario
	unsigned int mCriticalHits;
	unsigned int mBulletsEvaded;

	//Contadores POR NIVEL
	unsigned int mCurrentLevel;
	unsigned int mReceivedDamage;
	unsigned int mProducedDamageByWeapon[3];
	unsigned int mEnemiesKilledByWeapon[3];
	unsigned int mProducedDamageByHability[3];
	unsigned int mEnemiesKilledByHability[3];
	unsigned int mHabilitiesUsed[3]; //AUN NO SE EL NUMERO DE HABILIDADES
	unsigned int mCurrentHits;
	unsigned int mMaxHits;

private:
	static icePlayerStats* mInstance;
};

#endif