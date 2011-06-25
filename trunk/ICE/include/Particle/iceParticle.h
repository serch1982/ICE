#ifndef ICEPARTICLE_H__
#define ICEPARTICLE_H__

#include <Ogre.h>
#include <OgreSingleton.h>
#include <ParticleUniversePlugin.h>

class iceParticle
{
public:
	struct iceParticleParameters
	{
		Ogre::String	script;
		int		subtype;
		float	repeatMin;
		float	repeatMax;
	};

public:
	iceParticle(Ogre::SceneNode* sceneNode, ParticleUniverse::ParticleSystem* particleSystem, iceParticleParameters params, Ogre::String id);
	iceParticle(Ogre::Entity* entityNode,Ogre::SceneNode* sceneNode, Ogre::String boneName, ParticleUniverse::ParticleSystem* particleSystem, iceParticleParameters params, Ogre::String id);
	~iceParticle();
		
	void start(void);
	void startAndStopFade(Ogre::Real stopTime);
	void stop(void);
	void update(const float elapsedSeconds);

	// Parameters functions
	ParticleUniverse::ParticleSystem* getParticleSystem(){ return mParticleSystem; }
	iceParticleParameters getParameters(){ return mParameters; }
	Ogre::SceneNode* getSceneNode(){ return mSceneNode; }
	Ogre::String getId(){ return mId; }

	// Time functions
	float getRepeat(){ return mRepeat; }
	float getTimer(){ return mTimer; }
	void setTimer(float time){ mTimer = time; }

private:
	ParticleUniverse::ParticleSystem*	mParticleSystem;
	iceParticleParameters			mParameters;
	Ogre::SceneNode*					mSceneNode;
	Ogre::String								mId;

	float		mRepeat;
	float		mTimer;
};

typedef boost::shared_ptr<iceParticle> iceParticlePtr;

#endif