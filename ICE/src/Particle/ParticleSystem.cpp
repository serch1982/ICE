#include "..\include\Particle\ParticleSystem.h"

using namespace WyvernsAssault;

WyvernsAssault::ParticleSystem::ParticleSystem(SceneNode* sceneNode, ParticleUniverse::ParticleSystem* particleSystem, ParticleSystemParameters params, String id)
: mTimer(0.0)
{
	mSceneNode = sceneNode;
	mParticleSystem = particleSystem;
	mParameters = params;
	mId = id;

	mRepeat = fmodf(rand(), ( mParameters.repeatMax - mParameters.repeatMin )) + mParameters.repeatMin;

	mSceneNode->attachObject( mParticleSystem );
}

WyvernsAssault::ParticleSystem::~ParticleSystem()
{
}
		
void WyvernsAssault::ParticleSystem::start(void)
{
	mParticleSystem->start();
}

void WyvernsAssault::ParticleSystem::startAndStopFade(Ogre::Real stopTime)
{
	mParticleSystem->startAndStopFade(stopTime);
}

void WyvernsAssault::ParticleSystem::stop(void)
{
	mParticleSystem->stop();
}

void WyvernsAssault::ParticleSystem::update(const float elapsedSeconds)
{
}
	
void WyvernsAssault::ParticleSystem::attachTo(SceneNode* node)
{
}

void WyvernsAssault::ParticleSystem::detachFrom(SceneNode* node)
{
}
