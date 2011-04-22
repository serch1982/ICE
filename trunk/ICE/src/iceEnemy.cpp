#include "iceEnemy.h"

Ogre::NameGenerator iceEnemy::mNameGenerator("Enemy_");

iceEnemy::iceEnemy()
{
	mActivationTime = -1;
	mCurrentTime = 0;

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

bool iceEnemy::initialize(Ogre::SceneManager* p_psSceneManager, icePlayer* p_psPlayer, Ogre::Real p_fActivationTime, ENEMYTYPE p_Type, bool p_isAttachedToPlayer)
{
	mPlayer = p_psPlayer;
	mType = p_Type;
	mActivationTime = p_fActivationTime;
	mCurrentTime = 0;
	if(p_isAttachedToPlayer)
		iceTrajectoryFollower::initialize(p_psSceneManager,mPlayer->getNode()->createChildSceneNode(mNameGenerator.generate()));
	else
		iceTrajectoryFollower::initialize(p_psSceneManager,mPlayer->getNode()->getParentSceneNode()->createChildSceneNode(mNameGenerator.generate()));

	stringstream entityName;
	entityName << "Entity_" << mNameGenerator.generate();
	Ogre::Entity* mesh;
	switch(mType)
	{
		case MINIMAGMATON:
			mesh = mSceneManager->createEntity(entityName.str(), "minimagmaton.mesh");			
			break;
		case KAMIKAZE:
			mesh = mSceneManager->createEntity(entityName.str(), "kamikaze.mesh");
			break;
		case INTELLIGENT:
			mesh = mSceneManager->createEntity(entityName.str(), "intelligent.mesh");
			break;
		case VOLCANO:
			mesh = mSceneManager->createEntity(entityName.str(), "volcano.mesh");
			break;
		case MAGMATON:
			mesh = mSceneManager->createEntity(entityName.str(), "magmaton.mesh");
			break;
	}
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
			if(checkActivationTime(p_timeSinceLastFrame))
			{//active
				activate();
			}
			else
			{//inactive
				mNode->setVisible(false);
			}
			break;
	}
}

void iceEnemy::activate(void)
{
	mCurrentLife = getMaxLife();
	mNode->setVisible(true);
	mState = FOLLOWING_TRAJECTORY;
}

bool iceEnemy::checkActivationTime(Ogre::Real p_timeSinceLastFrame)
{
	if(mActivationTime < 0)
		return false;

	mCurrentTime += p_timeSinceLastFrame;
	if(mCurrentTime > mActivationTime)
	{
		mActivationTime = -1;
		return true;
	}
	else
	{
		return false;
	}
}

void iceEnemy::createShotEntity(int p_iWeapon, Ogre::Quaternion p_sOrientation, unsigned int p_iDamage, bool p_bCritic)
{
}

void iceEnemy::showReceivedDamage(unsigned int p_iDamage, bool p_bCritical)
{
}

void iceEnemy::showShieldDamage(unsigned int p_iDamage, bool p_bCritical)
{
}

void iceEnemy::showFail(void)
{
}

void iceEnemy::showLevelUp(unsigned int p_iLevel)
{
}