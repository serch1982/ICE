#include "Logic/icePhisics.h"

icePhisics::icePhisics(void)
{
}

icePhisics::~icePhisics(void)
{
	delete mPlayerBullets;
	delete mEnemyBullets;
}

void icePhisics::initialize(icePlayer* p_Player, std::vector<iceEnemy*>* p_Enemies, std::vector<iceBullet*>* p_PlayerBullets, std::vector<iceBullet*>* p_EnemyBullets)
{
	mPlayer = p_Player;
	mEnemies = p_Enemies;
	mPlayerBullets = p_PlayerBullets;
	mEnemyBullets = p_EnemyBullets;

	mLog = Ogre::LogManager::getSingleton().getLog("iceLog.log");
}

void icePhisics::processPlayerBullets(void)
{
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
					if (enemy->getBoundingBox().intersects(bullet->getWorldPosition()))
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

void icePhisics::processEnemyBullets(void)
{
}