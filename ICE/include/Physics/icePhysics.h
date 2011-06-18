#ifndef ICEPHYSICS_H_
#define ICEPHYSICS_H_

#include <vector>
#include "../Entities/icePlayer.h"
#include "../Entities/iceEnemy.h"
#include "../Entities/iceBullet.h"

class icePhysics
{
public:
	icePhysics(void);
	
	virtual ~icePhysics(void);

	void initialize(icePlayer* p_Player, std::vector<iceEnemy*>* p_Enemies);

	void update();


protected:
	void processPlayerBullets(void);
	void processEnemyBullets(void);

	icePlayer* mPlayer;
	std::vector<iceEnemy*>* mEnemies;
	Ogre::Log* mLog;
};


#endif