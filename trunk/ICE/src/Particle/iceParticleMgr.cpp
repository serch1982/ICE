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

iceParticlePtr iceParticleMgr::createPartAttachToObject(Ogre::SceneNode* node, Ogre::Vector3 position, Ogre::String script, bool start, Ogre::Vector3 scale)
{
	iceParticle::iceParticleParameters params = this->defaultParameters(script);
	Ogre::String name = this->createUniqueId() + "_" + script;
	ParticleUniverse::ParticleSystem* ps = mParticleSystemManager->createParticleSystem(name, params.script, mSceneManager);
	ps->setScale(scale);
	iceParticlePtr pSystem = iceParticlePtr(new iceParticle(node->createChildSceneNode(position), ps, params, name));
	if( start )
		pSystem->start();

	return pSystem;
}

iceParticlePtr iceParticleMgr::createPartAttachToBone(Ogre::Entity* entityNode, Ogre::String boneName, Ogre::String script, bool start, Ogre::Vector3 scale, Ogre::Vector3 position)
{
	iceParticle::iceParticleParameters params = this->defaultParameters(script,false,false, position);
	Ogre::String name =script + this->createUniqueId();
	ParticleUniverse::ParticleSystem* ps = mParticleSystemManager->createParticleSystem(name, script, mSceneManager);
	ps->setScale(scale);
	iceParticlePtr pSystem = iceParticlePtr(new iceParticle(entityNode,boneName,ps, params,name));
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

iceParticle::iceParticleParameters iceParticleMgr::defaultParameters(Ogre::String script, bool isFree, bool isLoop, Ogre::Vector3 position )
{
	iceParticle::iceParticleParameters params;
	params.script = script;
	params.isLoop = isLoop;
	params.isFree = isFree;
	params.position = position;
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

void iceParticleMgr::createParticle(Ogre::SceneNode* node, Ogre::String script, Ogre::Vector3 scale)
{
	iceParticle::iceParticleParameters params = this->defaultParameters(script, true, false);
	Ogre::String name = this->createUniqueId() + "_" + script;
	ParticleUniverse::ParticleSystem* ps = mParticleSystemManager->createParticleSystem(name, params.script, mSceneManager);

	iceParticlePtr pSystem = iceParticlePtr(new iceParticle(node, ps, params, name));
	pSystem->start();
	ps->setScale(scale);
	mParticlesList.push_back(pSystem);
}

void iceParticleMgr::update(Ogre::Real timeSinceLastFrame)
{
	iceParticlesIter it = mParticlesList.begin();
	while (it != mParticlesList.end()) {
		if (!(*it)->isPlay())  {
			mParticleSystemManager->destroyParticleSystem(mParticleSystemManager->getParticleSystem((*it)->getId()), mSceneManager);
			it = mParticlesList.erase(it);
		}
		else{
			//(*it)->update();
			++it;
		}
		
	}

}