#include "Physics\icePhysics.h"
#include "iceGame.h"
#include "Entities\iceBulletMgr.h"

#define DEFAULT_DAMAGE_WALL 1
#define DEFAULT_DAMAGE_OBJECTS 2
#define DEFAULT_RETURN_SHIP 4

icePhysics::icePhysics(void)
{
}

icePhysics::~icePhysics(void)
{

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
				AxisAlignedBox ebox = enemy->getGeometry()->getWorldBoundingBox(enemy->getWorldPosition());
				if(ebox.intersects(bbox)){
					enemy->addDamage((*iter)->getDamage(),(*iter)->getCritic());
					(*iter)->desactivate();
				}
			}
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
	Ogre::Ray playerRay(icePlayer::getSingletonPtr()->getPosition(), Ogre::Vector3::NEGATIVE_UNIT_Y);
    Ogre::TerrainGroup::RayResult mResult =mTerrainGroup->rayIntersects(playerRay, 0.1); 

	if (!mResult.hit){
		std::stringstream strMessage;
		strMessage << "Impacto en: (" << icePlayer::getSingletonPtr()->getPosition() << ")";
		mLog->logMessage( strMessage.str() );

		icePlayer::getSingletonPtr()->addDamage(2,false); //to change
		Ogre::Vector3 initPos = icePlayer::getSingletonPtr()->getShipPosition();
		Ogre::Vector3 lastPos = icePlayer::getSingletonPtr()->getShipLastPosition();
		icePlayer::getSingletonPtr()->setShipPosition(Ogre::Vector3(initPos.x > 0 ? lastPos.x - 1.5 : lastPos.x + 1.5, lastPos.y + .5,initPos.z));

	}
}

void icePhysics::processObjectCollision(void){
	AxisAlignedBox pbox = icePlayer::getSingletonPtr()->getGeometry()->getWorldBoundingBox(icePlayer::getSingletonPtr()->getPosition());
	
	//collions with objects
	for( unsigned j = 0; j < mObjects.size(); j++){
		AxisAlignedBox obox = ((iceObject*)(mObjects)[j])->getBox();
		if(pbox.intersects(obox)){
			std::stringstream strMessage;
			strMessage << "Impacto en: (" << icePlayer::getSingletonPtr()->getPosition() << ")";
			mLog->logMessage( strMessage.str() );

			icePlayer::getSingletonPtr()->addDamage(DEFAULT_DAMAGE_WALL,false); //to change
		}
	}

	//collision with enemy body
	for(unsigned j = 0; j < mEnemies->size(); j++){
		iceEnemy* enemy = (*mEnemies)[j];
		AxisAlignedBox ebox = enemy->getGeometry()->getWorldBoundingBox(enemy->getWorldPosition());
		if(enemy->isActive() && enemy->isAlive()){
			if(ebox.intersects(pbox)){
				enemy->setState(iceEnemy::DEAD);
				icePlayer::getSingletonPtr()->addDamage(DEFAULT_DAMAGE_OBJECTS,false); //to change
				Ogre::Vector3 initPos = icePlayer::getSingletonPtr()->getShipPosition();
				if( initPos.z <= 1){
					icePlayer::getSingletonPtr()->setShipPosition(Ogre::Vector3(initPos.x, initPos.y,initPos.z - DEFAULT_RETURN_SHIP));
				}
			}
		}
	}
		
}