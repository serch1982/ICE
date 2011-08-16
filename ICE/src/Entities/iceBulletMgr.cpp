#include "Entities\iceBulletMgr.h"
#include "iceGame.h"

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
	return true;
}

void iceBulletMgr::finalize()
{
	mIceBulletList.clear();
}

iceBulletPtr iceBulletMgr::createBullet(bool fromPlayer, Ogre::String name,int bulletType, Ogre::Vector3 initPos, Ogre::Quaternion orientation, Ogre::Radian desviation, int damage, bool critic){
		iceBulletPtr bullet  = iceBulletPtr(new iceBullet());
		
		Ogre::SceneManager* sceneMgr =  iceGame::getSceneManager();
		Ogre::String nodeName = name + "_" + this->createUniqueId() ;
		Ogre::SceneNode* mBulletNode = sceneMgr->getRootSceneNode()->createChildSceneNode(nodeName,Ogre::Vector3( 0, 0, 0 ));	
		if (bulletType == 0){
					Ogre::BillboardSet* billboarSet = sceneMgr->createBillboardSet();
					if(fromPlayer)
						billboarSet->setMaterialName("machinegunShot");
					else
						billboarSet->setMaterialName("machinegunFire");
					billboarSet->createBillboard(0,0,0);
				    mBulletNode->attachObject(billboarSet);
					mBulletNode->setVisible(true);
				    mBulletNode->scale(.04,.04,.04);
					
					bullet->createBullet(fromPlayer,NULL, mBulletNode, Ogre::Vector3(3,3,3), initPos, orientation, desviation, "","");
					bullet->setSpeed(500);
					bullet->setCritic(critic);
					bullet->setDamage(damage);
		}
		if (bulletType == 1){
					Ogre::BillboardSet* billboarSet = sceneMgr->createBillboardSet();
					billboarSet->setMaterialName("shotgunShot");
					billboarSet->createBillboard(0,0,0);
				    mBulletNode->attachObject(billboarSet);
					mBulletNode->setVisible(true);
				    mBulletNode->scale(.05,.05,.05);
					
					bullet->createBullet(fromPlayer,NULL, mBulletNode,  Ogre::Vector3(3,3,3), initPos, orientation, desviation,"","");
					bullet->setSpeed(500);
					bullet->setCritic(critic);
					bullet->setDamage(damage);
		}
		if (bulletType == 2){
					Ogre::Entity* entity = sceneMgr->createEntity("missile.mesh");
					mBulletNode->attachObject(entity);
					mBulletNode->setVisible(true);
					
					bullet->createBullet(fromPlayer, entity, mBulletNode, Ogre::Vector3(6,6,8), initPos, orientation, desviation, "root","ice/iceMisilTurbo");
					bullet->setSpeed(300);
					bullet->setCritic(critic);
					bullet->setDamage(damage);
			}	
		
		mIceBulletList.push_back(bullet);
		
		return bullet;
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
	iceBulletIter it = mIceBulletList.begin();
	while (it != mIceBulletList.end()) {
		(*it)->setDebugEnabled(isDebugEnabled);
		(*it)->setTimer((*it)->getTimer() + timeSinceLastFrame);
		if ((((*it)->getTimer() >= (*it)->getTimeDuration()) && (*it)->isActive()) || (!(*it)->isActive()) ) {
			it = mIceBulletList.erase(it);
		}
		else{
			(*it)->move(timeSinceLastFrame);
			++it;
		}
		
	}

}