#include "iceEnemy.h"

Ogre::NameGenerator iceEnemy::mNameGenerator("Enemy_");

iceEnemy::iceEnemy()
{

}

iceEnemy::~iceEnemy()
{

}

void iceEnemy::setState(ENEMYSTATE p_iState)
{
	mState = p_iState;
}

int iceEnemy::getState(void)
{
	return mState;
}

void iceEnemy::setPlayer(icePlayer* p_psPlayer)
{
	mPlayer = p_psPlayer;
}

bool iceEnemy::initialize(Ogre::SceneManager* p_psSceneManager, icePlayer* p_psPlayer)
{
	mPlayer = p_psPlayer;
	iceTrajectoryFollower::initialize(p_psSceneManager,mPlayer->getNode()->createChildSceneNode(mNameGenerator.generate()));

	stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();
	Ogre::Entity* mesh = mSceneManager->createEntity(entityName.str(), "razor.mesh");
	mNode->attachObject(mesh);
	mNode->scale(0.1,0.1,0.1);

	mState = INACTIVE;

	return true;
}

void iceEnemy::finalize()
{

}

void iceEnemy::update(Ogre::Real p_timeSinceLastFrame)
{
	iceRPG::update(p_timeSinceLastFrame);
	
	switch(mState)
	{
		case STOPPED:

			break;
		case FOLLOWING_TRAJECTORY:
			iceTrajectoryFollower::update(p_timeSinceLastFrame);
			break;
		case GOING_TO_PLAYER:
			break;
		case DEADING:
			//Dead sequence...
			//When dead sequence finished:
			mState = INACTIVE;
			break;
		case INACTIVE:
			mNode->setVisible(false);
			break;
	}

}

void iceEnemy::activate(void)
{
	mCurrentLife = getMaxLife();
	mNode->setVisible(true);
	mState = FOLLOWING_TRAJECTORY;
}

void iceEnemy::createShotEntity(int p_iWeapon, Ogre::Quaternion p_sOrientation, Ogre::Real p_iDamage, bool p_bCritic)
{
}

void iceEnemy::showReceivedDamage(Ogre::Real p_iDamage, bool p_bCritical)
{
}

void iceEnemy::showShieldDamage(Ogre::Real p_iDamage, bool p_bCritical)
{
}

void iceEnemy::showFail(void)
{
}

void iceEnemy::showLevelUp(unsigned int p_iLevel)
{
}