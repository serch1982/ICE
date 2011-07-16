#include "Particle\iceParticleMgr.h"
#include "iceGame.h"

// BEGIN SINGLETON
template<> iceParticleMgr* Ogre::Singleton<iceParticleMgr>::ms_Singleton = 0;

iceParticleMgr* iceParticleMgr::getSingletonPtr(void)
{
    return ms_Singleton;
}
iceParticleMgr& iceParticleMgr::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

iceParticleMgr::iceParticleMgr():
 mParticleSystemManager(0)
, mId(0)
{
	this->mSceneManager = iceGame::getSceneManager();
}

iceParticleMgr::~iceParticleMgr()
{
	finalize();
}

void iceParticleMgr::initialize()
{
	mParticleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();
} 

void iceParticleMgr::finalize()
{
	mParticleSystemManager->destroyAllParticleSystems(mSceneManager);
	mParticleSystemManager->destroyAllAliasses();

	mParticleSystemManager = NULL;
}


iceParticlePtr iceParticleMgr::createPartAttachToObject(Ogre::SceneNode* node, Ogre::String script, bool start)
{
	iceParticle::iceParticleParameters params = this->defaultParameters(script);
	Ogre::String name = this->createUniqueId() + "_" + script;
	ParticleUniverse::ParticleSystem* ps = mParticleSystemManager->createParticleSystem(name, params.script, mSceneManager);

	iceParticlePtr pSystem = iceParticlePtr(new iceParticle(node, ps, params, name));
	if( start )
		pSystem->start();

	return pSystem;
}

iceParticlePtr iceParticleMgr::createPartAttachToObject(Ogre::SceneNode* node, Ogre::Vector3 position, Ogre::String script, bool start)
{
	iceParticle::iceParticleParameters params = this->defaultParameters(script);
	Ogre::String name = this->createUniqueId() + "_" + script;
	ParticleUniverse::ParticleSystem* ps = mParticleSystemManager->createParticleSystem(name, params.script, mSceneManager);

	iceParticlePtr pSystem = iceParticlePtr(new iceParticle(node->createChildSceneNode(position), ps, params, name));
	if( start )
		pSystem->start();

	return pSystem;
}

iceParticlePtr iceParticleMgr::createPartAttachToBone(Ogre::Entity* entityNode, Ogre::String boneName, Ogre::String script, bool start)
{
	Ogre::String name =script + this->createUniqueId();
	ParticleUniverse::ParticleSystem* ps = mParticleSystemManager->createParticleSystem(name, script, mSceneManager);

	iceParticlePtr pSystem = iceParticlePtr(new iceParticle(entityNode,boneName,ps, this->defaultParameters(script),name));
	if( start )
		pSystem->start();

	return pSystem;
}



bool iceParticleMgr::removeParticle(iceParticlePtr miceParticlePtr)
{
	if( miceParticlePtr == NULL )
		return false;
	
	mParticleSystemManager->getParticleSystem(miceParticlePtr->getId())->stop();
	mParticleSystemManager->destroyParticleSystem(mParticleSystemManager->getParticleSystem(miceParticlePtr->getId()), mSceneManager);
	miceParticlePtr.reset();

	return true;
}

iceParticle::iceParticleParameters iceParticleMgr::defaultParameters(Ogre::String script)
{
	iceParticle::iceParticleParameters params;
	params.script = script;
	params.isLoop = true;

	return params;
}


Ogre::String iceParticleMgr::createUniqueId()
{
	Ogre::StringStream countStrStr;
	countStrStr << mId;

	Ogre::String uniqueId = "Particle_" + countStrStr.str();

	mId++;

	return uniqueId;
}
