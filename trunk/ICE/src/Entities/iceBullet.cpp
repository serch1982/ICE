#include "Entities\iceBullet.h"
#include "Particle\iceParticleMgr.h"
#include "iceGame.h"

#define TIME_TO_LIVE 3

iceBullet::iceBullet()
{
}
iceBullet::~iceBullet(void)
{
	finalizeEntity();
}

void iceBullet::createBullet(int bulletType, bool fromPlayer){
	mBulletType = bulletType;
	mFromPlayer = fromPlayer;

	Ogre::SceneManager* sceneMgr =  iceGame::getSceneManager();
	mBulletNode = sceneMgr->getRootSceneNode()->createChildSceneNode();
	Ogre::Vector3 boxSize;
	if (mBulletType == 0){
		Ogre::BillboardSet* billboarSet = sceneMgr->createBillboardSet();
		if(mFromPlayer)
			billboarSet->setMaterialName("machinegunShot");
		else
			billboarSet->setMaterialName("machinegunFire");
		billboarSet->createBillboard(0,0,0);
		mBulletNode->attachObject(billboarSet);
		mBulletNode->scale(.04,.04,.04);

		boxSize = Ogre::Vector3(3.5,3.5,3.5);
					
		mSpeed = 1000;
	}
	if (mBulletType == 1){
		Ogre::BillboardSet* billboarSet = sceneMgr->createBillboardSet();
		billboarSet->setMaterialName("shotgunShot");
		billboarSet->createBillboard(0,0,0);
		mBulletNode->attachObject(billboarSet);
		mBulletNode->scale(.05,.05,.05);
					
		boxSize = Ogre::Vector3(3.5,3.5,3.5);
		mSpeed = 1000;
	}
	if (mBulletType == 2){
		Ogre::Entity* entity = sceneMgr->createEntity("missile.mesh");
		mBulletNode->attachObject(entity);
					
		boxSize = Ogre::Vector3(6.5,6.5,8.5);
		mSpeed = 500;

		mParticlePtr = iceParticleMgr::getSingletonPtr()->createPartAttachToBone(entity,"root","ice/iceMisilTurbo",true);
		mParticlePtr->stop();
	}

	mBulletNode->setVisible(false);

	icePhysicEntity::initializePhysics(mBulletNode->getName() + "_phy", boxSize);
	mBulletNode->attachObject(getGeometry()->getMovableObject());

	mActive = false;
}

void iceBullet::activate(Ogre::Vector3 position, Ogre::Quaternion orientation,Ogre::Radian desviation, unsigned int pDamage, bool pCritic)
{
	mBulletNode->setOrientation(orientation);
	mBulletNode->setPosition(position);

	Ogre::Vector3 desviationDirection = Ogre::Vector3::UNIT_Z.randomDeviant(desviation);
	mBulletNode->rotate(Ogre::Vector3::UNIT_Z.getRotationTo(desviationDirection));
	mDamage = pDamage;
	mCritic = pCritic;
	mActive = true;
	mTime = 0;
	mBulletNode->setVisible(true);
	if(mParticlePtr)
		mParticlePtr->start();
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

void iceBullet::update(Ogre::Real timeSinceLastFrame,bool isDebugEnabled)
{
	if(mActive)
	{
		setDebugEnabled(isDebugEnabled);
		Ogre::Real vel =mSpeed * timeSinceLastFrame;
		mBulletNode->translate(0, 0, vel, Ogre::Node::TS_LOCAL);

		mTime += timeSinceLastFrame;
		if(mTime > TIME_TO_LIVE)
			desactivate();
	}
}

void iceBullet::crashEnemy(void){ 
	Ogre::SceneNode* node = iceGame::getSceneManager()->getRootSceneNode()->createChildSceneNode();
	node->setPosition(mBulletNode->_getDerivedPosition());
	Ogre::Vector3 scale(.05,.05,.05);
	node->scale(scale);
	iceParticleMgr::getSingletonPtr()->createParticle(node, "explotion_blue",scale);
	desactivate();
}

void iceBullet::crashPlayer(void){ 
	Ogre::SceneNode* node = iceGame::getSceneManager()->getRootSceneNode()->createChildSceneNode();
	node->setPosition(mBulletNode->_getDerivedPosition());
	Ogre::Vector3 scale(.05,.05,.05);
	node->scale(scale);
	iceParticleMgr::getSingletonPtr()->createParticle(node, "metal_hit",scale);
	desactivate();
}