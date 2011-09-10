#ifndef ICEPHYSICS_H_
#define ICEPHYSICS_H_

#include <vector>
#include <Terrain/OgreTerrainGroup.h>
#include "Entities\icePlayer.h"
#include "Enemies\iceEnemy.h"
#include "Enemies\iceVolcano.h"
#include "Enemies\iceBoss.h"
#include "Entities\iceBullet.h"
#include "Entities\iceObject.h"

class icePhysics
{
public:
	/**
    *  contructor
    */
	icePhysics(void);
	/**
    *  descontructor
    */
	~icePhysics(void);
	/**
	*  initialize physics
	*/
	void initialize(Ogre::TerrainGroup* terrainGroup, std::vector<iceEnemy*>* p_Enemies, std::vector<iceObject*> p_Objects);

	/**
	*  update physics
	*/
	void update();
protected:
	/**
	*  process collision with bullets
	*/
	void processBullets(void);
	/**
	*  process collision with object in to the scene
	*/
	void processObjectCollision(void);
	/**
	*  process collision with the terrain
	*/
	void processTerrainCollision(void);

	//vars
	Ogre::TerrainGroup* mTerrainGroup;
	std::vector<iceEnemy*>* mEnemies;
	std::vector<iceObject*> mObjects;
	std::vector<Ogre::Entity*> mSoftMagmatonObjects;
	std::vector<Ogre::Entity*> mHardMagmatonObjects;
	std::vector<Ogre::Entity*> mAttackMagmatonObjects;
	Ogre::Log* mLog;
};

typedef boost::shared_ptr<icePhysics> icePhysicsPtr;

#endif