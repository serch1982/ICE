#include "Entities\iceBullet.h"
#include "Particle\iceParticleMgr.h"
#include "iceGame.h"

iceBullet::iceBullet(int bulletType)
:
	mDuration(3),
	mActive(true),
	mDamage(0),
	mCritic(false),
	mSpeed(0),
	mTime(0),
	mPosition(Ogre::Vector3(0,0,0)),
	mOrientation(Ogre::Quaternion(0,0,0,0)),
	mBulletType(mBulletType)
{
}
iceBullet::~iceBullet(void)
{
	finalizeEntity();
}

void iceBullet::createBullet(bool fromPlayer, Ogre::Entity* bulletEntity, Ogre::SceneNode* bulletNode, Ogre::Vector3 boxSize, Ogre::Vector3 position, Ogre::Quaternion orientation,Ogre::Radian desviation, Ogre::String boneName, Ogre::String particleName){
		mFromPlayer = fromPlayer;

		bulletNode->setOrientation(orientation);
		bulletNode->setPosition(position);

		Ogre::Vector3 desviationDirection = Ogre::Vector3::UNIT_Z.randomDeviant(desviation);
		bulletNode->rotate(Ogre::Vector3::UNIT_Z.getRotationTo(desviationDirection));

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
	Ogre::Real vel =mSpeed * timeSinceLastFrame;
	mBulletNode->translate(0, 0, vel, Ogre::Node::TS_LOCAL);	
}

void iceBullet::crashEnemy(void){ 
	Ogre::SceneNode* node = iceGame::getSceneManager()->getRootSceneNode()->createChildSceneNode(mBulletNode->getName() + "_exp");
	node->setPosition(mBulletNode->_getDerivedPosition());
	Ogre::Vector3 scale(.05,.05,.05);
	node->scale(scale);
	iceParticleMgr::getSingletonPtr()->createParticle(node, "explotion_blue",scale);
	mActive = false; 
}

void iceBullet::crashPlayer(void){ 
	Ogre::SceneNode* node = iceGame::getSceneManager()->getRootSceneNode()->createChildSceneNode(mBulletNode->getName() + "_exp");
	node->setPosition(mBulletNode->_getDerivedPosition());
	Ogre::Vector3 scale(.05,.05,.05);
	node->scale(scale);
	iceParticleMgr::getSingletonPtr()->createParticle(node, "metal_hit",scale);
	mActive = false; 
}