#ifndef ICEPARTICLE_H__
#define ICEPARTICLE_H__

#include <Ogre.h>
#include <OgreSingleton.h>
#include <ParticleUniversePlugin.h>

class iceParticle
{
public:
	/**
    *  struct of Parameters for the particles
    */
	struct iceParticleParameters
	{
		Ogre::String script;
		bool isLoop;
		bool isFree;
		Ogre::Vector3 position;
	};

public:
	/**
    *  contructors
    */
	iceParticle(Ogre::SceneNode* sceneNode, ParticleUniverse::ParticleSystem* particleSystem, iceParticleParameters params, Ogre::String id);
	iceParticle(Ogre::Entity* entityNode, Ogre::String boneName, ParticleUniverse::ParticleSystem* particleSystem, iceParticleParameters params, Ogre::String id);
	/**
    *  descontructor
    */
	~iceParticle();
	
	/**
    *  Start the particle
    */
	void start(void);
	/**
    *  start And Stop Fade the particle 
    */
	void startAndStopFade(Ogre::Real stopTime);
	/**
    *  stop the particle
    */
	void stop(void);
	/**
    *  get if the particle is playing
    */
	bool isPlay(void);

	/**
    *  get the ParticleUniverse ParticleSystem
    */
	ParticleUniverse::ParticleSystem* getParticleSystem(){ return mParticleSystem; }
	/**
    *   get the parameter of the instance
    */
	iceParticleParameters getParameters(){ return mParameters; }
	/**
    *  get the name id of the instance
    */
	Ogre::String getId(){ return mId; }

private:
	//private vars
	ParticleUniverse::ParticleSystem*	mParticleSystem;
	iceParticleParameters			mParameters;
	Ogre::String					mId;
	Ogre::SceneNode*			mSceneNode;

};

typedef boost::shared_ptr<iceParticle> iceParticlePtr;

#endif