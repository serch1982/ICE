#include "Physics\icePhysics.h"
#include "iceGame.h"
#include "Entities\iceBulletMgr.h"

#define DEFAULT_DAMAGE_WALL 1
#define DEFAULT_DAMAGE_OBJECTS 2
#define DEFAULT_RETURN_SHIP 4
#define DEFAULT_RETURN_SHIP_MAX -16

icePhysics::icePhysics(void)
{
}

icePhysics::~icePhysics(void)
{
	mEnemies->clear();
	mObjects.clear();
}

void icePhysics::initialize(Ogre::TerrainGroup* terrainGroup,  std::vector<iceEnemy*>* p_Enemies, std::vector<iceObject*> p_Objects)
{
	mEnemies = p_Enemies;
	mObjects = p_Objects;
	mTerrainGroup = terrainGroup;
	mLog = iceGame::getGameLog();
}

void icePhysics::processBullets(void)
{
	iceBulletList bl = iceBulletMgr::getSingletonPtr()->getAllBullets();
	iceBulletIter iter = bl.begin();

	AxisAlignedBox pbox = icePlayer::getSingletonPtr()->getGeometry()->getWorldBoundingBox(icePlayer::getSingletonPtr()->getPosition());
	//bullets against bodies 
	while(iter != bl.end()){
		AxisAlignedBox bbox = (*iter)->getGeometry()->getWorldBoundingBox((*iter)->getPosition());
		if(!(*iter)->isFromPlayer()){
			if(pbox.intersects(bbox)){
				icePlayer::getSingletonPtr()->addDamage((*iter)->getDamage(),(*iter)->getCritic());
				(*iter)->desactivate();
			}
		}
		else{
			for(unsigned j = 0; j < mEnemies->size(); j++){
				iceEnemy* enemy = (*mEnemies)[j];
				if((enemy->isActive()) && (enemy->isAlive())){
					AxisAlignedBox ebox = enemy->getGeometry()->getWorldBoundingBox(enemy->getWorldPosition());
					if(ebox.intersects(bbox)){
						enemy->addDamage((*iter)->getDamage(),(*iter)->getCritic());
						(*iter)->desactivate();
					}
				}
			}
		}
		//detect collision between the player and the terrain
		Ogre::Ray bulletRayNY((*iter)->getPosition(), Ogre::Vector3::NEGATIVE_UNIT_Y);
		Ogre::TerrainGroup::RayResult mResult =mTerrainGroup->rayIntersects(bulletRayNY,0.1); 
		if (!mResult.hit){
			(*iter)->desactivate();
		}
		++iter;
	}
}

void icePhysics::update(void){
	processBullets();
	processObjectCollision();
	processTerrainCollision();
}

void icePhysics::processTerrainCollision(void){
	Ogre::Real dis = 0.1;
	Ogre::Real xgap = 1;
	Ogre::Real ygap = 0.6;
	
	Ogre::Vector3 posp = icePlayer::getSingletonPtr()->getPosition();
	posp.y = posp.y - 2;
	Ogre::Ray playerRayNY(posp, Ogre::Vector3::NEGATIVE_UNIT_Y);
	
	//detect collision between the player and the terrain
	Ogre::TerrainGroup::RayResult mResult =mTerrainGroup->rayIntersects(playerRayNY,dis); 
	
	if (!mResult.hit){
		Ogre::Vector3 initPos = icePlayer::getSingletonPtr()->getShipPosition();
		iceSdkTray::getInstance()->updateScreenInfo( 18, Ogre::StringConverter::toString(initPos.x) + " - " + Ogre::StringConverter::toString(initPos.y)+ " - " + Ogre::StringConverter::toString(initPos.z));
		icePlayer::getSingletonPtr()->addDamage(DEFAULT_DAMAGE_WALL,false);
		if(initPos.x > 0) xgap = -xgap;
		if((initPos.x > -5) && (initPos.x < 5)){
			xgap = 0;
		}
		icePlayer::getSingletonPtr()->setShipTranslate(Ogre::Vector3(xgap, ygap,0));
	}

	//detect collision between enemies and the terrain
	for(unsigned j = 0; j < mEnemies->size(); j++){
		iceEnemy* enemy = (*mEnemies)[j];

		if(enemy->isActive() && enemy->isAlive()){
			Ogre::Ray enemyRayNY(enemy->getWorldPosition(), Ogre::Vector3::NEGATIVE_UNIT_Y);
			mResult =mTerrainGroup->rayIntersects(enemyRayNY,dis); 
			if (!mResult.hit){
				enemy->changeDirection();
			}
		}
	}
	
}

void icePhysics::processObjectCollision(void){
	AxisAlignedBox pbox = icePlayer::getSingletonPtr()->getGeometry()->getWorldBoundingBox(icePlayer::getSingletonPtr()->getPosition());
	
	//collions with objects
	for( unsigned j = 0; j < mObjects.size(); j++){
		AxisAlignedBox obox = ((iceObject*)(mObjects)[j])->getBox();
		if(pbox.intersects(obox)){
			icePlayer::getSingletonPtr()->addDamage(DEFAULT_DAMAGE_WALL,false); 
		}
	}

	//collision with enemy body
	for(unsigned j = 0; j < mEnemies->size(); j++){
		iceEnemy* enemy = (*mEnemies)[j];
		AxisAlignedBox ebox = enemy->getGeometry()->getWorldBoundingBox(enemy->getWorldPosition());
		if(enemy->isActive() && enemy->isAlive()){
			if(ebox.intersects(pbox)){
				enemy->setState(iceEnemy::DEAD);
				icePlayer::getSingletonPtr()->addDamage(DEFAULT_DAMAGE_OBJECTS,false);
				Ogre::Vector3 initPos = icePlayer::getSingletonPtr()->getShipPosition();
				if( initPos.z <= 1 && initPos.z > DEFAULT_RETURN_SHIP_MAX){
					icePlayer::getSingletonPtr()->setShipTranslate(Ogre::Vector3(0, 0,-DEFAULT_RETURN_SHIP));
				}
			}
		}
	}
		
}