#ifndef ICEPHYSICS_H_
#define ICEPHYSICS_H_

#include <vector>
#include <Terrain/OgreTerrainGroup.h>
#include "../Entities/icePlayer.h"
#include "../Entities/iceEnemy.h"
#include "../Entities/iceBullet.h"
#include "Entities\iceObject.h"

class icePhysics
{
public:
	icePhysics(void);
	
	virtual ~icePhysics(void);

	void initialize(Ogre::TerrainGroup* terrainGroup, icePlayer* p_Player, std::vector<iceEnemy*>* p_Enemies, std::vector<iceObject*> p_Objects);

	void update();


protected:
	void processPlayerBullets(void);
	void processEnemyBullets(void);
	void processObjectCollision(void);
	void processTerrainCollision(void);

	icePlayer* mPlayer;
	Ogre::TerrainGroup* mTerrainGroup;
	std::vector<iceEnemy*>* mEnemies;
	std::vector<iceObject*> mObjects;
	Ogre::Log* mLog;
};


#endif