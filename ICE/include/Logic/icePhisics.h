#ifndef ICEPHISICS_H_
#define ICEPHISICS_H_

#include <vector>
#include "../Entities/icePlayer.h"
#include "../Entities/iceEnemy.h"
#include "../Entities/iceBullet.h"

class icePhisics
{
public:
	icePhisics(void);
	
	virtual ~icePhisics(void);

	void initialize(icePlayer* p_Player, std::vector<iceEnemy*>* p_Enemies, std::vector<iceBullet*>* p_PlayerBullets, std::vector<iceBullet*>* p_EnemyBullets);

	void processPlayerBullets(void);
	void processEnemyBullets(void);


protected:
	icePlayer* mPlayer;
	std::vector<iceEnemy*>* mEnemies;

	std::vector<iceBullet*>* mPlayerBullets;						// Bullet has been shooted and is now into the scene.
	std::vector<iceBullet*>* mEnemyBullets;
	Ogre::Log* mLog;
};


#endif