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
, mTimer(0.0)
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


/** Finalize the particle manager */
void iceParticleMgr::finalize()
{
	mParticleSystemManager->destroyAllParticleSystems(mSceneManager);
	mParticleSystemManager->destroyAllAliasses();

	mParticleSystemManager = NULL;
	mIceParticleList.clear();
}

void iceParticleMgr::update(const float elapsedSeconds)
{
	for(unsigned int i = 0; i < mIceParticleList.size(); i++ )
	{
		// Only check particle systems thar need to be repeated
		if( mIceParticleList[i]->getRepeat() > 0 )
		{
			mIceParticleList[i]->setTimer( mIceParticleList[i]->getTimer() + elapsedSeconds );

			// If total time (mTimer) mod particle system repeat time < 1, need to be restarted
			if( mIceParticleList[i]->getTimer() > mIceParticleList[i]->getRepeat() )
			{
				mIceParticleList[i]->start();
				mIceParticleList[i]->setTimer(0.0);
			}
		}
	}
}

// Create particle system
ParticleUniverse::ParticleSystem* iceParticleMgr::create(Ogre::String script)
{
	return mParticleSystemManager->createParticleSystem(this->createUniqueId(), script, mSceneManager);
}


void iceParticleMgr::add(Ogre::SceneNode* node, iceParticle::iceParticleParameters params, bool start)
{
	ParticleUniverse::ParticleSystem* ps = this->create(params.script);

	iceParticlePtr pSystem = iceParticlePtr(new iceParticle(node, ps, params, this->createUniqueId()));
	if( start )
		pSystem->start();

	mIceParticleList.push_back(pSystem);
}

void iceParticleMgr::add(Ogre::SceneNode* node, Ogre::Vector3 position, iceParticle::iceParticleParameters params, bool start)
{
	ParticleUniverse::ParticleSystem* ps = this->create(params.script);

	iceParticlePtr pSystem = iceParticlePtr(new iceParticle(node->createChildSceneNode(position), ps, params, this->createUniqueId()));
	if( start )
		pSystem->start();

	mIceParticleList.push_back(pSystem);
}

void iceParticleMgr::add(Ogre::SceneNode* node, Ogre::String id, iceParticle::iceParticleParameters params, bool start)
{
	ParticleUniverse::ParticleSystem* ps = mParticleSystemManager->createParticleSystem(id + "_particle", params.script, mSceneManager);

	iceParticlePtr pSystem = iceParticlePtr(new iceParticle(node, ps, params, id + "_particle"));
	if( start )
		pSystem->start();

	mIceParticleList.push_back(pSystem);
}

void iceParticleMgr::add(Ogre::Entity* entityNode, Ogre::SceneNode* node,  Ogre::String boneName, Ogre::String script)
{
	Ogre::String name =script + this->createUniqueId();
	ParticleUniverse::ParticleSystem* ps = mParticleSystemManager->createParticleSystem(name, script, mSceneManager);

	iceParticlePtr pSystem = iceParticlePtr(new iceParticle(entityNode,node, boneName,ps, this->defaultParameters(script),name));
	pSystem->start();

	mIceParticleList.push_back(pSystem);
}


iceParticlePtr iceParticleMgr::addParticle(Ogre::SceneNode* node, Ogre::String script, bool start)
{
	iceParticle::iceParticleParameters params = this->defaultParameters(script);
	Ogre::String name = this->createUniqueId() + "_" + script;
	ParticleUniverse::ParticleSystem* ps = mParticleSystemManager->createParticleSystem(name, params.script, mSceneManager);

	iceParticlePtr pSystem = iceParticlePtr(new iceParticle(node, ps, params, name));
	if( start )
		pSystem->start();

	return pSystem;
}

// Remove particle system from node and stop it (only usable with non-unique ID particle systems)
bool iceParticleMgr::remove(Ogre::String id)
{
	iceParticlePtr pSystem = this->getParticleSystem(id + "_particle");

	if( pSystem == NULL )
		return false;
	
	iceParticleListIterator it = find(mIceParticleList.begin(), mIceParticleList.end(), pSystem);
	
	if( it != mIceParticleList.end() )
	{
		mIceParticleList.erase(it);

		// Plugin special destroy
		mParticleSystemManager->getParticleSystem(id + "_particle")->stop();
		pSystem->getSceneNode()->detachObject(mParticleSystemManager->getParticleSystem(id + "_particle"));
		mParticleSystemManager->destroyParticleSystem(mParticleSystemManager->getParticleSystem(id + "_particle"), mSceneManager);

		return true;
	}

	return false;
}

iceParticlePtr iceParticleMgr::getParticleSystem(Ogre::String id)
{
	for(unsigned int i = 0; i < mIceParticleList.size(); i++ )
	{
		if( mIceParticleList[i]->getId() == id )
			return mIceParticleList[i];
	}

	iceParticlePtr pSystem;

	return pSystem;
}

iceParticle::iceParticleParameters iceParticleMgr::defaultParameters(Ogre::String script)
{
	iceParticle::iceParticleParameters params;
	params.script = script;
	params.subtype = -1;
	params.repeatMax = -1;
	params.repeatMin = -1;

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
