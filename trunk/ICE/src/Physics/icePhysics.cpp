#include "Physics\icePhysics.h"
#include "iceGame.h"

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

void icePhysics::processPlayerBullets(void)
{
	std::vector<iceBullet*>* mPlayerBullets = mPlayer->getAllBullets();
	for(unsigned int i=0;i<mPlayerBullets->size();i++)
	{
		iceBullet* bullet = (*mPlayerBullets)[i];
		if (bullet->isActive())
		{
			for(unsigned int j=0;j<mEnemies->size();j++)
			{
				iceEnemy* enemy = (*mEnemies)[j];
				if(enemy->isActive() && enemy->isAlive())
				{
					if (enemy->getBoundingBox().contains(bullet->getWorldPosition()))
					{
						std::stringstream strMessage;
						strMessage << "Impacto en: (" << bullet->getWorldPosition() << ")";
						mLog->logMessage( strMessage.str() );

						enemy->addDamage(bullet->getDamage(),bullet->isCritic());
						bullet->deactivate();
					}
				}
			}
		}
	}
}
void icePhysics::update(void){
	processEnemyBullets();
	processObjectCollision();
	processPlayerBullets();	
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


void icePhysics::processEnemyBullets(void)
{
	for( unsigned j = 0; j < mEnemies->size(); j++){
		std::vector<iceBullet*>* mEnemyBullets = (*mEnemies)[j]->getAllBullets();
		//collions entities
		if(mPlayer->isAlive())
		{
			if (mPlayer->getBoundingBox().intersects((*mEnemies)[j]->getWorldPosition()))
			{
				std::stringstream strMessage;
				strMessage << "Impacto en: (" << (*mEnemies)[j]->getWorldPosition() << ")";
				mLog->logMessage( strMessage.str() );

				mPlayer->addDamage(2,false); //to change
			}
		}
		//bullet
		for(unsigned int i=0;i< mEnemyBullets->size();i++)
		{
			iceBullet* bullet = (*mEnemyBullets)[i];
			if (bullet->isActive())
			{
				if(mPlayer->isAlive())
					{
						if (mPlayer->getBoundingBox().intersects(bullet->getWorldPosition()))
						{
							std::stringstream strMessage;
							strMessage << "Impacto en: (" << bullet->getWorldPosition() << ")";
							mLog->logMessage( strMessage.str() );

							mPlayer->addDamage(bullet->getDamage(),bullet->isCritic());
							bullet->deactivate();
						}
					}
			}
		}
	}
}

void icePhysics::processObjectCollision(void){
	for( unsigned j = 0; j < mObjects.size(); j++){
		//collions entities
		if(mPlayer->isAlive())
		{
			if( (iceObject*)(mObjects)[j]->getBoundingBox().intersects(mPlayer->getPosition())) 
			{
				std::stringstream strMessage;
				strMessage << "Impacto en: (" << mPlayer->getPosition() << ")";
				mLog->logMessage( strMessage.str() );

				mPlayer->addDamage(2,false); //to change
			}
		}
	}
}