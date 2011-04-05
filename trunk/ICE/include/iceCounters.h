#ifndef __iceCounters_h_
#define __iceCounters_h_

#include <vector>
#include <string>

#define MACHINEGUN	0
#define SHOTGUN		1
#define MISILE_LAUNCHER	2

#define ACHIEVEMENTS_NUMBER 23

using namespace std;

class iceCounters
{
public:
	static iceCounters* instance();
	~iceCounters();

	//Methods
	void activateAchievement(unsigned int p_iAchievementIndex);
	void showAchievement(unsigned int p_iAchievementIndex);

	void newLevel(unsigned int p_iLevel);
	void levelCompleted(void);
	void addReceivedDamage(unsigned int p_iDamage);
	void setPlayerLevel(unsigned int p_iPlayerLevel);
	void setWeaponLevel(unsigned int p_iWeapon, unsigned int p_iWeaponLevel);
	void addCriticalHit(void);
	void addBulletEvaded(void);

	void addProducedDamageByWeapon(unsigned int p_iWeapon, unsigned int p_iDamage);
	void addEnemiesKilledByWeapon(unsigned int p_iWeapon);
	void addProducedDamageByHability(unsigned int p_iHability, unsigned int p_iDamage);
	void addEnemiesKilledByHability(unsigned int p_iHability);
	void habilityUsed(unsigned int p_iHability);
	void addHit(void);
	void resetHits(void);


protected:
	iceCounters();
	//Achievements
	bool mCompletedAchievements[ACHIEVEMENTS_NUMBER];
	vector<string> mAchievementsName;

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
	static iceCounters* mInstance;
};

#endif