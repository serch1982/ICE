#include "Enemies\iceEnemy.h"
#include "Utils/iceDamageTextManager.h"
#include "iceGame.h"

Ogre::NameGenerator iceEnemy::mNameGenerator("Enemy_");

iceEnemy::iceEnemy()
{
	mActivationTime = -1;
	mCurrentTime = 0;
	mAnimDyingTicks = 0;
	mShowingBoundingBox = false;
	mLog = iceGame::getGameLog();
	mExperienceGived = false;
}

iceEnemy::~iceEnemy()
{
	if(mIceStrategy) mIceStrategy.reset();
	if(iceAnimationPtr) iceAnimationPtr.reset();
	icePhysicEntity::finalizePhysics();
}

void iceEnemy::setState(ENEMYSTATE p_iState)
{
	mState = p_iState;
}

int iceEnemy::getState(void)
{
	return mState;
}


bool iceEnemy::initialize(int id, Ogre::Vector3 p_Position, Ogre::Real p_fActivationTime, bool p_isAttachedToPlayer)
{
	iceAnimationPtr = iceAnimationMgrPtr(new iceAnimationMgr());
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();
	mRaySceneQuery = sceneManager->createRayQuery(Ogre::Ray());
	mActivationTime = p_fActivationTime;
	mCurrentTime = 0;
	Ogre::String name = mNameGenerator.generate();
	if(p_isAttachedToPlayer)
		enemyNode = icePlayer::getSingletonPtr()->getNode()->createChildSceneNode(name);
	else
		enemyNode = iceGame::getSceneManager()->getRootSceneNode()->createChildSceneNode(name);
			//icePlayer::getSingletonPtr()->getNode()->getParentSceneNode()->createChildSceneNode(name);

	iceTrajectoryFollower::initialize(enemyNode);
	enemyNode->setPosition(p_Position);

	//Dummy Trajectory
	setTrajectory(new iceTrajectory());
	mTrajectory->setNodeToLookAt(icePlayer::getSingletonPtr()->getShipNode());
	
	mState = INACTIVE;
	setLevel(1);

	mWeaponBaseCadence[MACHINEGUN] = 1;
	return true;
}
Ogre::SceneNode* iceEnemy::getEnemySceneNode(void){
	return enemyNode;
}

void iceEnemy::setBillboard(iceBillboard* billboard){
	mBillboard =  billboard;
} 

void iceEnemy::finalize()
{
	if(mIceStrategy) mIceStrategy.reset();
	icePhysicEntity::finalizePhysics();
}

void iceEnemy::update(Ogre::Real p_timeSinceLastFrame)
{
	_lastPosition = enemyNode->getPosition();
	iceRPG::update(p_timeSinceLastFrame);
	//for(std::map<Ogre::String,Ogre::AnimationState*>::const_iterator i = mAnimations.begin();i!=mAnimations.end();i++)
	//{
	//	i->second->addTime(p_timeSinceLastFrame);
	//}
}

std::string iceEnemy::getFunctionStr(){
	return "";
}

void iceEnemy::activate(void)
{
	mCurrentLife = getMaxLife();
	enemyNode->setVisible(true);
	mState = STOPPED;
	//mState = FOLLOWING_TRAJECTORY;
	if(mShowingBoundingBox)
	{
		setDebug(mShowingBoundingBox);
	}
}


void iceEnemy::desactivate(void)
{
	enemyNode->setVisible(false);
	mState = INACTIVE;
}

bool iceEnemy::isActive(void)
{
	return (mState != INACTIVE);
}

bool iceEnemy::checkActivationTime(Ogre::Real p_timeSinceLastFrame)
{
	p_timeSinceLastFrame *= icePlayer::getSingletonPtr()->getTimeMultiplier();
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

void iceEnemy::showReceivedDamage(unsigned int p_iDamage, bool p_bCritical)
{
	//stringstream strMessage;
	//strMessage << "Adding damage to an enemy = " << p_iDamage << ". Current life = " << getCurrentLife();
	//iceGame::getGameLog()->logMessage(strMessage.str());
	iceDamageTextManager::getSingletonPtr()->showEnemyDamage(icePhysicEntity::getGeometry()->getMovableObject(),p_iDamage,p_bCritical);
}

void iceEnemy::showShieldDamage(unsigned int p_iDamage, bool p_bCritical)
{
}

void iceEnemy::showFail(void)
{
	iceDamageTextManager::getSingletonPtr()->showEnemyMiss(icePhysicEntity::getGeometry()->getMovableObject());
}

void iceEnemy::showLevelUp(unsigned int p_iLevel)
{ //Do Nothing
}


bool iceEnemy::isVisiblePlayerCam(){
	Ogre::AxisAlignedBox vCamBBox = icePlayer::getSingletonPtr()->getVitualCamBBox();
	Ogre::AxisAlignedBox eBox = getGeometry()->getWorldBoundingBox(enemyNode->_getDerivedPosition());

	if(eBox.intersects(vCamBBox))
		return true;
	else
		return false;
}
//return if the eneymy whether or not  is inside the wide (super) camera
bool iceEnemy::isVisibleWideCam(){
	return icePlayer::getSingletonPtr()->isPositionBackToPlayer(this->getNode()->_getDerivedPosition());
}

float iceEnemy::rangeAttack(){
	Ogre::Real dis = icePlayer::getSingletonPtr()->getPosition().distance(enemyNode->_getDerivedPosition());
	return (float)dis;
}

Ogre::Vector3 iceEnemy::getWorldPosition(void)
{
	return enemyNode->_getDerivedPosition();
}

bool iceEnemy::isAnimDyingEnded()
{
	if( mAnimDyingTicks > 30 ){
		return true;
	}else
		return false;
}

void iceEnemy::setAnimDyingEnded( Ogre::Real ticks )
{
	mAnimDyingTicks = ticks;
}

void iceEnemy::createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic)
{
}

void iceEnemy::changeDirection(void){

}

void iceEnemy::giveExperieceToPlayer(void)
{
	if(!mExperienceGived)
	{
		icePlayer::getSingletonPtr()->addExperience(mLevel * 10000);
		mExperienceGived = true;
	}
}