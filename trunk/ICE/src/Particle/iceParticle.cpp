#include "Particle\iceParticle.h"

iceParticle::iceParticle(Ogre::SceneNode* sceneNode, ParticleUniverse::ParticleSystem* particleSystem, iceParticleParameters params, Ogre::String id): mTimer(0.0)
{
	mSceneNode = sceneNode;
	mParticleSystem = particleSystem;
	mParameters = params;
	mId = id;

	mSceneNode->attachObject( mParticleSystem );
}

iceParticle::iceParticle(Ogre::Entity* entityNode,Ogre::SceneNode* sceneNode, Ogre::String boneName, ParticleUniverse::ParticleSystem* particleSystem, iceParticleParameters params, Ogre::String id): mTimer(0.0){
	mSceneNode = sceneNode;
	mParticleSystem = particleSystem;
	mParameters = params;
	mId = id;

	entityNode->attachObjectToBone( boneName, mParticleSystem );
}

iceParticle::~iceParticle()
{
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

void iceParticle::update(const float elapsedSeconds)
{
}
