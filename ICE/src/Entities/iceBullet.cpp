#include "Entities\iceBullet.h"

iceBullet::iceBullet(void)
:
	mDuration(3),
	mActive(true),
	mDamage(0),
	mCritic(false),
	mSpeed(0),
	mTime(0),
	mPosition(Ogre::Vector3(0,0,0)),
	mOrientation(Ogre::Quaternion(0,0,0,0))
{
}
iceBullet::~iceBullet(void)
{
	finalizeEntity();
}

void iceBullet::createBullet(bool fromPlayer, Ogre::Entity* bulletEntity, Ogre::SceneNode* bulletNode, Ogre::Vector3 boxSize, Ogre::Vector3 position, Ogre::Quaternion orientation,Ogre::Radian desviation, Ogre::String boneName, Ogre::String particleName){
		mFromPlayer = fromPlayer;

		Ogre::Vector3 desviationDirection = Ogre::Vector3::UNIT_Z.randomDeviant(desviation);
		bulletNode->rotate(Ogre::Vector3::UNIT_Z.getRotationTo(desviationDirection));

		bulletNode->setOrientation(orientation);
		bulletNode->setPosition(position);

		icePhysicEntity::initializePhysics(bulletNode->getName(), boxSize);
		bulletNode->attachObject(getGeometry()->getMovableObject());

		if((boneName != "") &&  (particleName != ""))
		mParticlePtr = iceParticleMgr::getSingletonPtr()->createPartAttachToBone(bulletEntity,boneName,particleName,true);

		mBulletNode = bulletNode;
}

void iceBullet::finalizeEntity()
{
	if(mParticlePtr) iceParticleMgr::getSingletonPtr()->removeParticle(mParticlePtr);

	if(mBulletNode){
		mBulletNode->detachAllObjects();
		mBulletNode->removeAndDestroyAllChildren();
	}
	icePhysicEntity::finalizePhysics();
}

void iceBullet::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;
		getGeometry()->getMovableObject()->setVisible(mIsDebugEnabled);
	}
}

void iceBullet::move(Ogre::Real timeSinceLastFrame)
{
		mBulletNode->translate(0,0,timeSinceLastFrame * mSpeed,Ogre::Node::TS_LOCAL);	
}
