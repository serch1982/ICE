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
void iceBullet::Set(Ogre::SceneManager* sceneMgr,Ogre::SceneNode* shipNode, Ogre::SceneNode* bulletNode,
				 int p_iWeapon, Ogre::Real p_iDamage, bool p_bCritic)
{
	if (!mbActive) 
	{
		/*Activate bullet*/
		mbActive = true;		

		/* Initialize class atributes values */
		miWeapon = p_iWeapon;
		miDamage = p_iDamage;
		mbCritic = p_bCritic;
		msbulletNode = bulletNode->createChildSceneNode();

		/*Set bullet's entity depending on the weapons' type*/
		/*Set bullet's speed dependig on the weapon's type*/
		if (p_iWeapon == 0)
		{			
			Ogre::Entity* Weapon1 = sceneMgr->createEntity("sphere.mesh");
			msbulletNode->attachObject(Weapon1);
			msbulletNode->scale(.05,.05,.2);
			miSpeed = 100;			
		}

		if (p_iWeapon == 1)
		{			
			Ogre::Entity* Weapon2 = sceneMgr->createEntity("sphere.mesh");
			msbulletNode->attachObject(Weapon2);
			msbulletNode->scale(.05,.05,.2);
			miSpeed = 150;			
		}			

		/*Set bullets initial orientation*/
		msOrientation = shipNode->_getDerivedOrientation();
		msbulletNode->setOrientation(msOrientation);
		
		/*Set bullets initial position*/
		mvPosition = shipNode->_getDerivedPosition();
		msbulletNode->setPosition(mvPosition);
		msbulletNode->translate(0,0,60,Ogre::Node::TS_LOCAL);	
		

	}


}
void iceBullet::Update(Ogre::Real timeSinceLastFrame)
{	
	/*Translate bullet*/
	if ((mbActive)&&(miCountDown>0))
	{
		/*evt.TimeSinceLastFrame  gives the time in seconds */
		msbulletNode->translate(0,0,timeSinceLastFrame * miSpeed,Ogre::Node::TS_LOCAL);		
		miCountDown = miCountDown - timeSinceLastFrame;		
	}
	if (miCountDown<=0)
	{
		mbActive = false;
	}
	
	/*Update bullet's atributes*/
	mvPosition = msbulletNode->_getDerivedPosition();		// Will refresh bullet's position relative to the root scene node
	msOrientation = msbulletNode->_getDerivedOrientation();	// Idem with bullet's orientation

}