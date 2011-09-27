#include "Particle\iceParticle.h"

iceParticle::iceParticle(Ogre::SceneNode* sceneNode, ParticleUniverse::ParticleSystem* particleSystem, iceParticleParameters params, Ogre::String id)
{
	mParticleSystem = particleSystem;
	mParameters = params;
	mId = id;
	sceneNode->attachObject( mParticleSystem );
	mSceneNode = sceneNode;
}

iceParticle::iceParticle(Ogre::Entity* entityNode,Ogre::String boneName, ParticleUniverse::ParticleSystem* particleSystem, iceParticleParameters params, Ogre::String id){
	mParticleSystem = particleSystem;
	mParameters = params;
	mId = id;
	entityNode->attachObjectToBone( boneName, mParticleSystem, Ogre::Quaternion::IDENTITY, mParameters.position );
}

iceParticle::~iceParticle()
{
	if (mParameters.isFree){
		if(mSceneNode){
			mSceneNode->detachAllObjects();
			mSceneNode->removeAndDestroyAllChildren();
		}
	}
}
		
void iceParticle::start(void)
{
	mParticleSystem->start();
}

void iceParticle::startAndStopFade(Ogre::Real stopTime)
{
	mParticleSystem->startAndStopFade(stopTime);
}

void iceParticle::stop(void)
{
	mParticleSystem->stop();
}

bool iceParticle::isPlay(void)
{
	if((mParticleSystem->getState() == ParticleUniverse::ParticleSystem::PSS_STOPPED) 
		||(mParticleSystem->getState() == ParticleUniverse::ParticleSystem::PSS_PAUSED) )
		return false;
	else
		return true;
}
