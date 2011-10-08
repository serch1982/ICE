#ifndef ICEBULLET_H_
#define ICEBULLET_H_

#include "Entities\icePhysicEntity.h"
#include "Particle\iceParticleMgr.h"

class iceBullet : public icePhysicEntity
{
public: 
		/**
		*  contructor
		*/
		iceBullet();
		/**
		*  descontructor
		*/
		~iceBullet(void);
		
		/**
		*  to create set the parameter from the new bullet instance
		*/
		void createBullet(int bulletType, bool fromPlayer);
		
		void activate(Ogre::Vector3 position, Ogre::Quaternion orientation,Ogre::Radian desviation, unsigned int pDamage, bool pCritic);




		/**
		*  to remove the instance and entity
		*/
		void finalizeEntity();
		/**
		*  set the debug mode
		*/
		void setDebugEnabled(bool isDebugEnabled);
		/**
		*  get current the debug mode
		*/
		bool getDebugEnabled(){return mIsDebugEnabled;};

		/**
		*  set the current time of the bullet
		*/
		void setTimer(float seconds){ mTime = seconds; }
		/**
		*  get the current time of the bullet
		*/
		float getTimer(){ return mTime; }

		/**
		*  set property damage
		*/
		void setDamage(int damage){ mDamage = damage; }
		/**
		*   get property damage
		*/
		int getDamage(){ return mDamage; }

		/**
		*  set property speed
		*/
		void setSpeed(int speed){ mSpeed = speed; }
		/**
		*  get property speed
		*/
		int getSpeed(){ return mSpeed; }

		/**
		*  set property is critic
		*/
		void setCritic(bool critic) { mCritic = critic; }
		/**
		*  get property is critic
		*/
		bool getCritic(void) { return mCritic; }

		/**
		*  to desactivate the bullet
		*/
		void desactivate(void){
			mActive = false;
			mBulletNode->setVisible(false);
			if(mParticlePtr)
				mParticlePtr->stop();

		}

		/**
		*  to desactivate the bullet and create effect when crash an enemy
		*/
		void crashEnemy(void);

		/**
		*  to desactivate the bullet and create effect when crash the player
		*/
		void crashPlayer(void);
		
		/**
		*  get the property is active
		*/
		bool isActive(void){ return mActive; }
		
		/**
		*  get position of the bullet into the world
		*/
		Ogre::Vector3 getPosition(void){ return mBulletNode->_getDerivedPosition(); 	}

		/**
		*  move the bullet 
		*/
		void update(Ogre::Real timeSinceLastFrame,bool isDebugEnabled);

		/**
		*  get property is from the player
		*/
		bool isFromPlayer(void){return mFromPlayer;}
protected:
	//vars
	bool mFromPlayer;
	bool mActive;				
	bool mIsDebugEnabled;
	unsigned int mDamage;
	int mBulletType;		
	bool mCritic;		
	unsigned int mSpeed;	
	float mTime;
	Ogre::SceneNode* mBulletNode;		
	Ogre::Vector3 mPosition;
	Ogre::Quaternion mOrientation;
	//particles
	iceParticlePtr mParticlePtr;
};

typedef boost::shared_ptr<iceBullet> iceBulletPtr;
	
#endif