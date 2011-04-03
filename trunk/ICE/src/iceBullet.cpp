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
void iceBullet::CreateEntities(Ogre::SceneManager* sceneMgr, Ogre::SceneNode* bulletNode, int p_iWeapon)
{
		/* Initialize class atributes values */
		miWeapon = p_iWeapon;		
		msbulletNode = bulletNode->createChildSceneNode();

		/*Set bullet's entity depending on the weapons' type*/
		/*Set bullet's speed dependig on the weapon's type*/
		if (p_iWeapon == 0)
		{			
			Ogre::Entity* Shot_MachineGun = sceneMgr->createEntity("sphere.mesh");
			msbulletNode->attachObject(Shot_MachineGun);
			msbulletNode->setVisible(false);
			msbulletNode->scale(.05,.05,.2);
			miSpeed = 650;			
		}
		if (p_iWeapon == 1)
		{			
			Ogre::Entity* Shot_ShotGun = sceneMgr->createEntity("sphere.mesh");
			msbulletNode->attachObject(Shot_ShotGun);
			msbulletNode->setVisible(false);
			msbulletNode->scale(.05,.05,.2);
			miSpeed = 600;			
		}			
		if (p_iWeapon == 2)
		{			
			Ogre::Entity* Shot_MisileLauncher = sceneMgr->createEntity("sphere.mesh");
			msbulletNode->attachObject(Shot_MisileLauncher);
			msbulletNode->setVisible(false);
			msbulletNode->scale(.05,.05,.2);
			miSpeed = 700;			
		}		
}
bool iceBullet::Set(Ogre::SceneNode* shipNode,Ogre::Real p_iDamage, bool p_bCritic)
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
		msbulletNode->translate(0,0,60,Ogre::Node::TS_LOCAL);

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