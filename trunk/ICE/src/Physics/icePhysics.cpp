#include "Physics\icePhysics.h"
#include "iceGame.h"

icePhysics::icePhysics(void)
{
}

icePhysics::~icePhysics(void)
{

}

void icePhysics::initialize(icePlayer* p_Player, std::vector<iceEnemy*>* p_Enemies)
{
	mPlayer = p_Player;
	mEnemies = p_Enemies;

	mLog = iceGame::getGameLog();
}

void icePhysics::processPlayerBullets(void)
{
	vector<iceBullet*>* mPlayerBullets = mPlayer->getAllBullets();
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
						stringstream strMessage;
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
	processPlayerBullets();	
}

void icePhysics::processEnemyBullets(void)
{
	for( unsigned j = 0; j < mEnemies->size(); j++){
		vector<iceBullet*>* mEnemyBullets = (*mEnemies)[j]->getAllBullets();
		//collions entities
		if(mPlayer->isAlive())
		{
			if (mPlayer->getBoundingBox().intersects((*mEnemies)[j]->getWorldPosition()))
			{
				stringstream strMessage;
				strMessage << "Impacto en: (" << (*mEnemies)[j]->getWorldPosition() << ")";
				mLog->logMessage( strMessage.str() );

				mPlayer->addDamage(2,false);
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
							stringstream strMessage;
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