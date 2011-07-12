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
		iceBulletPtr createBullet(bool fromPlayer, Ogre::String name,int bulletType, Ogre::Vector3 initPos, Ogre::Quaternion orientation, Ogre::Radian desviation, int damage, bool critic);
		/**
		*  to create bullet into the scene 
		*/
		void update(Ogre::Real timeSinceLastFrame, bool isDebugEnabled);
		/**
		*  @return std::vector<iceBulletPtr> with all the bullets into the scene 
		*/
		iceBulletList getAllBullets(){return  mIceBulletList;}

private:
		/**
		*  to create and unique name
		*/
		Ogre::String createUniqueId();

private:
		//vars
		iceBulletList			mIceBulletList;
		int						mId;

};

typedef boost::shared_ptr<iceBulletMgr> iceBulletMgrPtr;

#endif 