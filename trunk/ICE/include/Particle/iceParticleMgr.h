#ifndef ICEPARTICLEMGR_H__
#define ICEPARTICLEMGR_H__

#include <Ogre.h>
#include <OgreSingleton.h>
#include <ParticleUniversePlugin.h>
#include <boost/enable_shared_from_this.hpp>
#include "Particle\iceParticle.h"

class iceParticleMgr	: public Ogre::Singleton<iceParticleMgr>
						, public boost::enable_shared_from_this<iceParticleMgr>
{				
public:
	/**
    *  contructor
    */
	iceParticleMgr();
	/**
    *  descontructor
    */
	~iceParticleMgr();
	/**
    *  singleton instance
    */
	static iceParticleMgr& getSingleton(void);
	static iceParticleMgr* getSingletonPtr(void);

	/**
    *  initialize particle manager
    */
	void initialize();
	/**
    *  finalize particle manager
    */
	void finalize();

	/**
    *  to create a particle attached to a scene node 
	*  @return iceParticlePtr : instance iceParticle
    */
	iceParticlePtr createPartAttachToObject(Ogre::SceneNode* node, Ogre::String script, bool start);
	/**
    *  to create a particle attached to a scene node in a specific position of the sceneNode  
	*  @return iceParticlePtr : instance iceParticle
    */
	iceParticlePtr createPartAttachToObject(Ogre::SceneNode* node, Ogre::Vector3 position, Ogre::String script, bool start);
	/**
    *  to create a particle attached to a bone from his body
	*  @return iceParticlePtr : instance iceParticle
    */
	iceParticlePtr createPartAttachToBone(Ogre::Entity* entityNode, Ogre::String boneName, Ogre::String script, bool start);
	/**
    *  to remove a particle to the world, deleting the instance
	*  @return bool
    */
	bool removeParticle(iceParticlePtr miceParticlePtr);

private:
	/**
    *  to create a default parameter for a instance iceparticle 
	*  @return iceParticle::iceParticleParameters
    */
	iceParticle::iceParticleParameters defaultParameters(Ogre::String script);
	/**
	*  to create and unique name
	*/
	Ogre::String createUniqueId();

	//private vars 
	ParticleUniverse::ParticleSystemManager* mParticleSystemManager;
	int	mId;
	Ogre::SceneManager*	mSceneManager;
};

typedef boost::shared_ptr<iceParticleMgr> iceParticleMgrPtr;

	
#endif
