#ifndef ICEPARTICLEMGR_H__
#define ICEPARTICLEMGR_H__

#include <Ogre.h>
#include <OgreSingleton.h>
#include <ParticleUniversePlugin.h>
#include <boost/enable_shared_from_this.hpp>

#include "Particle\iceParticle.h"


typedef std::vector<iceParticlePtr> iceParticleList;
typedef std::vector<iceParticlePtr>::iterator iceParticleListIterator;


class iceParticleMgr	: public Ogre::Singleton<iceParticleMgr>
						, public boost::enable_shared_from_this<iceParticleMgr>
{				
public:
	iceParticleMgr();
	~iceParticleMgr();
	static iceParticleMgr& getSingleton(void);
	static iceParticleMgr* getSingletonPtr(void);

	void initialize();
	void finalize();

	void update(const float elapsedSeconds);

	// Particle system function	
	ParticleUniverse::ParticleSystem* create(Ogre::String script);

	iceParticlePtr addParticle(Ogre::SceneNode* node, Ogre::String script, bool start);
	void add(Ogre::SceneNode* node, iceParticle::iceParticleParameters params, bool start = true);
	void add(Ogre::SceneNode* node, Ogre::Vector3 position, iceParticle::iceParticleParameters params, bool start = true);
	void add(Ogre::SceneNode* node, Ogre::String id, iceParticle::iceParticleParameters params, bool start);
	void add(Ogre::Entity* entityNode, Ogre::SceneNode* node,  Ogre::String boneName, Ogre::String script);
	bool remove(Ogre::String id);

	iceParticlePtr getParticleSystem(Ogre::String id);

	iceParticle::iceParticleParameters defaultParameters(Ogre::String script);


private:
	Ogre::String createUniqueId();

	ParticleUniverse::ParticleSystemManager* mParticleSystemManager;

	iceParticleList	mIceParticleList;

	int				mId;
	Ogre::Real			mTimer;
	Ogre::SceneManager*	mSceneManager;
};

typedef boost::shared_ptr<iceParticleMgr> iceParticleMgrPtr;

	
#endif
