#ifndef ICEBULLETMGR_H__
#define ICEBULLETMGR_H__

#include <Ogre.h>
#include <OgreSingleton.h>

#include <boost/enable_shared_from_this.hpp>

#include "Entities\iceBullet.h"

typedef std::vector<iceBulletPtr> iceBulletList;
typedef std::vector<iceBulletPtr>::iterator iceBulletIter;


class iceBulletMgr	: public Ogre::Singleton<iceBulletMgr>
					, public boost::enable_shared_from_this<iceBulletMgr>
{
public:

		/**
		*  contructor
		*/
		iceBulletMgr();
		/**
		*  descontructor
		*/
		~iceBulletMgr();
		//Ogre::Singleton
		static iceBulletMgr& getSingleton(void);
		static iceBulletMgr* getSingletonPtr(void);

		/**
		*  initialize manager
		*/
		bool initialize();
		/**
		*  finalize manager
		*/
		void finalize();

		/**
		*  to create bullet and throw it into the scene 
		*  @return the new class created iceBulletPtr
		*/
		void createBullet(bool fromPlayer, Ogre::String name,int bulletType, Ogre::Vector3 initPos, Ogre::Quaternion orientation, Ogre::Radian desviation, int damage, bool critic);
		/**
		*  to create bullet into the scene 
		*/
		void update(Ogre::Real timeSinceLastFrame, bool isDebugEnabled);
		/**
		*  @return std::vector<iceBulletPtr> with all the bullets into the scene 
		*/
		std::vector<iceBullet*> getPlayerBullets(){return  mPlayerBullets;}
		std::vector<iceBullet*> getEnemiesBullets(){return  mEnemiesBullets;}
private:
		/**
		*  to create and unique name
		*/
		Ogre::String createUniqueId();

		iceBullet* getPlayerActiveBullet(int bulletType);
		iceBullet* getEnemiesActiveBullet();

private:
		//vars
		std::vector<iceBullet*>				 mPlayerBullets;
		std::vector<iceBullet*>				 mEnemiesBullets;
		int									 mId;

};

typedef boost::shared_ptr<iceBulletMgr> iceBulletMgrPtr;

#endif 