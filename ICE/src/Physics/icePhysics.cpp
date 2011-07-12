#include "Physics\icePhysics.h"
#include "iceGame.h"
#include "Entities\iceBulletMgr.h"

icePhysics::icePhysics(void)
{
}

icePhysics::~icePhysics(void)
{

}

void icePhysics::initialize(Ogre::TerrainGroup* terrainGroup, icePlayer* p_Player, std::vector<iceEnemy*>* p_Enemies, std::vector<iceObject*> p_Objects)
{
	mPlayer = p_Player;
	mEnemies = p_Enemies;
	mObjects = p_Objects;
	mTerrainGroup = terrainGroup;
	mLog = iceGame::getGameLog();
}

void icePhysics::processBullets(void)
{
	iceBulletList bl = iceBulletMgr::getSingletonPtr()->getAllBullets();
	iceBulletIter iter = bl.begin();

	AxisAlignedBox pbox = mPlayer->getGeometry()->getWorldBoundingBox(mPlayer->getPosition());

	while(iter != bl.end()){
		AxisAlignedBox bbox = (*iter)->getGeometry()->getWorldBoundingBox((*iter)->getPosition());
		if(!(*iter)->isFromPlayer()){
			if(pbox.intersects(bbox)){
				mPlayer->addDamage((*iter)->getDamage(),(*iter)->getCritic());
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
	Ogre::Ray playerRay(mPlayer->getPosition(), Ogre::Vector3::NEGATIVE_UNIT_Y);
    Ogre::TerrainGroup::RayResult mResult =mTerrainGroup->rayIntersects(playerRay); 

	if (!mResult.hit){
		std::stringstream strMessage;
		strMessage << "Impacto en: (" << mPlayer->getPosition() << ")";
		mLog->logMessage( strMessage.str() );

		mPlayer->addDamage(2,false); //to change
	}
}

void icePhysics::processObjectCollision(void){
	AxisAlignedBox pbox = mPlayer->getGeometry()->getWorldBoundingBox(mPlayer->getPosition());
	
	//collions with objects
	for( unsigned j = 0; j < mObjects.size(); j++){
		AxisAlignedBox obox = ((iceObject*)(mObjects)[j])->getBox();
		if(pbox.intersects(obox)){
			std::stringstream strMessage;
			strMessage << "Impacto en: (" << mPlayer->getPosition() << ")";
			mLog->logMessage( strMessage.str() );

			mPlayer->addDamage(2,false); //to change
		}
	}

	//collision with enemy body
	for(unsigned j = 0; j < mEnemies->size(); j++){
		iceEnemy* enemy = (*mEnemies)[j];
		AxisAlignedBox ebox = enemy->getGeometry()->getWorldBoundingBox(enemy->getWorldPosition());
		if(enemy->isActive() && enemy->isAlive()){
			if(ebox.intersects(pbox)){
				mPlayer->addDamage(2,false); //to change
			}
		}
	}
		
}