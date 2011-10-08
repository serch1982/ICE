#include "Entities\iceBulletMgr.h"
#include "iceGame.h"

#define MACHINEGUN_BULLETS 100
#define SHOTGUN_BULLETS 100
#define MISSILE_BULLETS 50
#define ENEMIES_BULLETS 100

template<> iceBulletMgr* Ogre::Singleton<iceBulletMgr>::ms_Singleton = 0;
iceBulletMgr* iceBulletMgr::getSingletonPtr(void)
{
    return ms_Singleton;
}
iceBulletMgr& iceBulletMgr::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}

iceBulletMgr::iceBulletMgr() 
: mId(0)
{
	
}

iceBulletMgr::~iceBulletMgr()
{
	finalize();
}

bool iceBulletMgr::initialize()
{

	mPlayerBullets.resize(MACHINEGUN_BULLETS+SHOTGUN_BULLETS+MISSILE_BULLETS);
	for(unsigned int i=0;i<MACHINEGUN_BULLETS;i++)
	{
		iceBullet* bullet = new iceBullet();
		bullet->createBullet(0,true);
		mPlayerBullets[i] = bullet;
	}
	for(unsigned int i=MACHINEGUN_BULLETS;i<MACHINEGUN_BULLETS+SHOTGUN_BULLETS;i++)
	{
		iceBullet* bullet = new iceBullet();
		bullet->createBullet(1,true);
		mPlayerBullets[i] = bullet;
	}
	for(unsigned int i=MACHINEGUN_BULLETS+SHOTGUN_BULLETS;i<MACHINEGUN_BULLETS+SHOTGUN_BULLETS+MISSILE_BULLETS;i++)
	{
		iceBullet* bullet = new iceBullet();
		bullet->createBullet(2,true);
		mPlayerBullets[i] = bullet;
	}

	mEnemiesBullets.resize(ENEMIES_BULLETS);
	for(unsigned int i=0;i<ENEMIES_BULLETS;i++)
	{
		iceBullet* bullet = new iceBullet();
		bullet->createBullet(0,false);
		mEnemiesBullets[i] = bullet;
	}

	return true;
}

void iceBulletMgr::finalize()
{
	for(unsigned int i=0;i<MACHINEGUN_BULLETS+SHOTGUN_BULLETS+MISSILE_BULLETS;i++)
	{
		delete mPlayerBullets[i];
	}
	for(unsigned int i=0;i<ENEMIES_BULLETS;i++)
	{
		delete mEnemiesBullets[i];
	}
}

void iceBulletMgr::createBullet(bool fromPlayer, Ogre::String name,int bulletType, Ogre::Vector3 initPos, Ogre::Quaternion orientation, Ogre::Radian desviation, int damage, bool critic){


	iceBullet* bullet = NULL;
	if(fromPlayer)
	{
		bullet = getPlayerActiveBullet(bulletType);
	}
	else
	{
		bullet = getEnemiesActiveBullet();
	}

	if(bullet)
	{
		bullet->activate(initPos,orientation,desviation,damage,critic);
	}





		//iceBulletPtr bullet  = iceBulletPtr(new iceBullet(bulletType));
		//
		//Ogre::SceneManager* sceneMgr =  iceGame::getSceneManager();
		//Ogre::String nodeName = name + "_" + this->createUniqueId() ;
		//Ogre::SceneNode* mBulletNode = sceneMgr->getRootSceneNode()->createChildSceneNode(nodeName,Ogre::Vector3( 0, 0, 0 ));	
		//if (bulletType == 0){
		//			Ogre::BillboardSet* billboarSet = sceneMgr->createBillboardSet();
		//			if(fromPlayer)
		//				billboarSet->setMaterialName("machinegunShot");
		//			else
		//				billboarSet->setMaterialName("machinegunFire");
		//			billboarSet->createBillboard(0,0,0);
		//		    mBulletNode->attachObject(billboarSet);
		//			mBulletNode->setVisible(true);
		//		    mBulletNode->scale(.04,.04,.04);
		//			
		//			bullet->createBullet(fromPlayer,NULL, mBulletNode, Ogre::Vector3(3.5,3.5,3.5), initPos, orientation, desviation, "","");
		//			bullet->setSpeed(1000);
		//			bullet->setCritic(critic);
		//			bullet->setDamage(damage);
		//}
		//if (bulletType == 1){
		//			Ogre::BillboardSet* billboarSet = sceneMgr->createBillboardSet();
		//			billboarSet->setMaterialName("shotgunShot");
		//			billboarSet->createBillboard(0,0,0);
		//		    mBulletNode->attachObject(billboarSet);
		//			mBulletNode->setVisible(true);
		//		    mBulletNode->scale(.05,.05,.05);
		//			
		//			bullet->createBullet(fromPlayer,NULL, mBulletNode,  Ogre::Vector3(3.5,3.5,3.5), initPos, orientation, desviation,"","");
		//			bullet->setSpeed(1000);
		//			bullet->setCritic(critic);
		//			bullet->setDamage(damage);
		//}
		//if (bulletType == 2){
		//			Ogre::Entity* entity = sceneMgr->createEntity("missile.mesh");
		//			mBulletNode->attachObject(entity);
		//			mBulletNode->setVisible(true);
		//			
		//			bullet->createBullet(fromPlayer, entity, mBulletNode, Ogre::Vector3(6.5,6.5,8.5), initPos, orientation, desviation, "root","ice/iceMisilTurbo");
		//			bullet->setSpeed(500);
		//			bullet->setCritic(critic);
		//			bullet->setDamage(damage);
		//	}	
		//
		//mIceBulletList.push_back(bullet);
		
		//return bullet;
}


Ogre::String iceBulletMgr::createUniqueId()
{
	Ogre::StringStream countStrStr;
	countStrStr << mId;

	Ogre::String uniqueId = "bullet_" + countStrStr.str();

	mId++;

	return uniqueId;
}



void iceBulletMgr::update(Ogre::Real timeSinceLastFrame, bool isDebugEnabled)
{
	for(unsigned int i=0;i<MACHINEGUN_BULLETS+SHOTGUN_BULLETS+MISSILE_BULLETS;i++)
	{
		mPlayerBullets[i]->update(timeSinceLastFrame,isDebugEnabled);
		mPlayerBullets[i]->setDebugEnabled(isDebugEnabled);
	}
	for(unsigned int i=0;i<ENEMIES_BULLETS;i++)
	{
		mEnemiesBullets[i]->update(timeSinceLastFrame,isDebugEnabled);
		mEnemiesBullets[i]->setDebugEnabled(isDebugEnabled);
	}
}

iceBullet* iceBulletMgr::getPlayerActiveBullet(int bulletType)
{
	unsigned int begining = 0;
	unsigned int end = 0;
	if(bulletType == 0)
	{
		begining = 0;
		end = MACHINEGUN_BULLETS;
	}
	else if (bulletType == 1)
	{
		begining = MACHINEGUN_BULLETS;
		end = MACHINEGUN_BULLETS+SHOTGUN_BULLETS;
	}
	else if (bulletType == 2)
	{
		begining = MACHINEGUN_BULLETS+SHOTGUN_BULLETS;
		end = MACHINEGUN_BULLETS+SHOTGUN_BULLETS+MISSILE_BULLETS;
	}

	for(unsigned int i=begining;i<end;i++)
	{
		if(!mPlayerBullets[i]->isActive())
			return mPlayerBullets[i];
	}

	return NULL;
}

iceBullet* iceBulletMgr::getEnemiesActiveBullet()
{
	for(unsigned int i=0;i<ENEMIES_BULLETS;i++)
	{
		if(!mEnemiesBullets[i]->isActive())
			return mEnemiesBullets[i];
	}

	return NULL;
}