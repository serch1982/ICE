#include "iceBullet.h"


iceBullet::iceBullet(void)
:
	mbActive(false),						
	miWeapon(0),						
	miDamage(Ogre::Real(0)),				
	mbCritic(false),
	miSpeed(100),
	miCountDown(Ogre::Real(MAX_TIME_ACTIVE)),
	mvPosition(Ogre::Vector3(0,0,0)),
	msOrientation(Ogre::Quaternion(0,0,0,0))
{
}
iceBullet::~iceBullet(void)
{

}
void iceBullet::CreateEntities(Ogre::SceneManager* sceneMgr, Ogre::SceneNode* bulletNode, int p_iWeapon, int p_iBulletNumber)
{
		/* Initialize class atributes values */
		miWeapon = p_iWeapon;		
		msbulletNode = bulletNode->createChildSceneNode();
		

		/*Set bullet's entity depending on the weapons' type*/
		/*Set bullet's speed dependig on the weapon's type*/
		if (p_iWeapon == 0)
		{			
			/*machinegunShot Billboard*/			
			Ogre::BillboardSet* machinegunShotSet = sceneMgr->createBillboardSet();
			machinegunShotSet->setMaterialName("machinegunShot");
			Ogre::Billboard* machinegunShotBillboard = machinegunShotSet->createBillboard(0,0,0);
			msbulletNode->attachObject(machinegunShotSet);
			msbulletNode->setVisible(false);
			msbulletNode->scale(.05,.05,.05);
			miSpeed = 1750;
		}
		if (p_iWeapon == 1)
		{			
			/*machinegunShot Billboard*/			
			Ogre::BillboardSet* shotgunShotSet = sceneMgr->createBillboardSet();
			shotgunShotSet->setMaterialName("shotgunShot");
			Ogre::Billboard* shotgunShotBillboard = shotgunShotSet->createBillboard(0,0,0);
			msbulletNode->attachObject(shotgunShotSet);
			msbulletNode->setVisible(false);
			msbulletNode->scale(.03,.03,.03);
			miSpeed = 1700;
			
		}			
		if (p_iWeapon == 2)
		{			
			/* Misile mesh */
			Ogre::Entity* Shot_MisileLauncher = sceneMgr->createEntity("sphere.mesh");
			msbulletNode->attachObject(Shot_MisileLauncher);
			msbulletNode->setVisible(false);
			msbulletNode->scale(.005,.005,.005);
			miSpeed = 1800;	
			
			/* Misile particle system */			
			Ogre::ParticleSystem* misilParticle = sceneMgr->createParticleSystem("MisilParticle" + Ogre::StringConverter::toString(p_iBulletNumber), "misilParticle");			
			msbulletNode->attachObject(misilParticle);

					
		}		
}
bool iceBullet::Set(Ogre::SceneManager* sceneMgr, Ogre::SceneNode* shipNode,Ogre::Real p_iDamage, bool p_bCritic,int p_iShotSide)
{
	if (!mbActive) 
	{
		/*Activate bullet*/
		mbActive = true;
		msbulletNode->setVisible(true);

		/* Initialize class atributes values */
		miDamage = p_iDamage;
		mbCritic = p_bCritic;

		/*Set bullets initial orientation*/
		msOrientation = shipNode->_getDerivedOrientation();
		msbulletNode->setOrientation(msOrientation);
		
		/*Set bullets initial position*/
		mvPosition = shipNode->_getDerivedPosition();
		msbulletNode->setPosition(mvPosition);
		if (p_iShotSide == 0)
		{
			msbulletNode->translate(-1,0,0,Ogre::Node::TS_LOCAL);
		}
		else
		{
			msbulletNode->translate(1,0,0,Ogre::Node::TS_LOCAL);
		}
		return true;

	}else
	{
		return false;
	}

}
void iceBullet::Update(Ogre::Real timeSinceLastFrame)
{	
	/*Translate bullet*/
	if (mbActive)
	{
		/*evt.TimeSinceLastFrame  gives the time in seconds */
		msbulletNode->translate(0,0,timeSinceLastFrame * miSpeed,Ogre::Node::TS_LOCAL);		
		miCountDown = miCountDown - timeSinceLastFrame;

		/*Update bullet's atributes*/
		mvPosition = msbulletNode->_getDerivedPosition();		// Will refresh bullet's position relative to the root scene node
		msOrientation = msbulletNode->_getDerivedOrientation();	// Idem with bullet's orientation
	}
	/*Deactivate bullet and reset its life time to its initial value*/
	if ((mbActive)&&(miCountDown<=0))
	{
		miCountDown = MAX_TIME_ACTIVE;
		mbActive = false;
		msbulletNode->setVisible(false);
	}	

}