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


bool iceEnemy::initialize(int id, Ogre::Vector3 p_Position, Ogre::Real p_fActivationTime, bool p_isAttachedToPlayer)
{
	Ogre::SceneManager* sceneManager = iceGame::getSceneManager();
	mRaySceneQuery = sceneManager->createRayQuery(Ogre::Ray());
	mActivationTime = p_fActivationTime;
	mCurrentTime = 0;
	Ogre::String name = mNameGenerator.generate();
	if(p_isAttachedToPlayer)
		enemyNode = icePlayer::getSingletonPtr()->getNode()->createChildSceneNode(name);
	else
		enemyNode = icePlayer::getSingletonPtr()->getNode()->getParentSceneNode()->createChildSceneNode(name);

	iceTrajectoryFollower::initialize(enemyNode);
	mNode->setPosition(p_Position);

	//Dummy Trajectory
	setTrajectory(new iceTrajectory());
	mTrajectory->setNodeToLookAt(icePlayer::getSingletonPtr()->shipNode);
	
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

}

void iceEnemy::update(Ogre::Real p_timeSinceLastFrame)
{
	iceRPG::update(p_timeSinceLastFrame);
}

std::string iceEnemy::getFunctionStr(){
	return "";
}

void iceEnemy::activate(void)
{
	if(mIceParticle){
		mIceParticle->start();
	}
	mCurrentLife = getMaxLife();
	mNode->setVisible(true);
	mState = STOPPED;
	//mState = FOLLOWING_TRAJECTORY;
	if(mShowingBoundingBox)
	{
		setDebug(mShowingBoundingBox);
	}
}

void iceEnemy::setIceParticle(iceParticlePtr particlePtr){
	mIceParticle = particlePtr;
}

void iceEnemy::desactivate(void)
{
	if(mIceParticle){
		mIceParticle->stop();
	}
	mNode->setVisible(false);
	mState = INACTIVE;
}

bool iceEnemy::isActive(void)
{
	return (mState != INACTIVE);
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

//return if the eneymy whether or not is inside the player camera
bool iceEnemy::isVisiblePlayerCam(){
	Ogre::Camera* playCam = icePlayer::getSingletonPtr()->getCamera();
	Ogre::Ray ray(playCam->getPosition(), playCam->getDirection());
	mRaySceneQuery->setRay(ray);
    Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator itr;
    for (itr = result.begin(); itr != result.end(); itr++) {
        if (itr->movable->getName().compare(enemyNode->getName())!=0) {
            return true;
        }
    }
    return false;

	
}
//return if the eneymy whether or not  is inside the wide (super) camera
bool iceEnemy::isVisibleWideCam(){
	Ogre::Camera *playCam = icePlayer::getSingletonPtr()->getCamera();
	Ogre::Ray ray(playCam->getPosition(), playCam->getDirection() * 2);
	mRaySceneQuery->setRay(ray);
    Ogre::RaySceneQueryResult &result = mRaySceneQuery->execute();
    Ogre::RaySceneQueryResult::iterator itr;
    for (itr = result.begin(); itr != result.end(); itr++) {
        if (itr->movable->getName().compare(enemyNode->getName())!=0) {
            return true;
        }
    }
    return false;
}
//return if the eneymy whether or not can shoot
float iceEnemy::rangeAttack(){
	Ogre::Real pZ = icePlayer::getSingletonPtr()->getPosition().z;
	Ogre::Real eZ = enemyNode->getPosition().z;
	Ogre::Real tZ = eZ - pZ;
	if ((tZ > 500) ||(tZ < 10)) {
	int i=0;
	}
	return (float)tZ;
}

void iceEnemy::createShotEntity(int p_iWeapon, Ogre::Radian p_fDeviation, unsigned int p_iDamage, bool p_bCritic)

{
	iceBulletMgr::getSingletonPtr()->createBullet(false, "bt_enemy_",p_iWeapon, mNode->_getDerivedPosition(), -mNode->_getDerivedOrientation(), p_fDeviation,p_iDamage, p_bCritic);
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