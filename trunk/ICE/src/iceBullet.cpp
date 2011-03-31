#include "Bullet.h"


Bullet::Bullet(void)
	:
	mbActive(false),						
	miWeapon(0),						
	miDamage(Ogre::Real(0)),				
	mbCritic(false),						
	mvPosition(Ogre::Vector3(0,0,0)),
	msOrientation(Ogre::Quaternion(0,0,0,0)),
	miCountDown(Ogre::Real(MAX_TIME_ACTIVE))
{
}
Bullet::~Bullet(void)
{

}
void Bullet::Set(Ogre::SceneNode* shipNode, Ogre::SceneNode* bulletNode, int p_iWeapon, Ogre::Real p_iDamage, bool p_bCritic)
{
	if (!mbActive) 
	{
		/*Activate bullet*/
		mbActive = true;

		/* Initialize class atributes values */
		miWeapon = p_iWeapon;
		miDamage = p_iDamage;
		mbCritic = p_bCritic;

		/*Set bullets initial orientation*/
		msOrientation = shipNode->_getDerivedOrientation();
		bulletNode->setOrientation(msOrientation);
		
		/*Set bullets initial position*/
		mvPosition = shipNode->_getDerivedPosition();
		bulletNode->setPosition(mvPosition);
		bulletNode->translate(0,0,60,Ogre::Node::TS_LOCAL);	
		

	}


}
void Bullet::Update(Ogre::SceneNode* bulletNode, const Ogre::FrameEvent& evt)
{	
	/*Translate bullet*/
	if ((mbActive)&&(miCountDown>0))
	{
		bulletNode->translate(0,0,evt.timeSinceLastFrame*SPEED,Ogre::Node::TS_LOCAL);
			/*evt.TimeSinceLastFrame  gives the time in seconds */
		miCountDown = miCountDown - evt.timeSinceLastFrame;		
	}
	if (miCountDown<=0)
	{
		mbActive = false;
	}
	
	/*Update bullet's atributes*/
	mvPosition = bulletNode->_getDerivedPosition();			// Will refresh bullet's position relative to the root scene node
	msOrientation = bulletNode->_getDerivedOrientation();	// Idem with bullet's orientation

}