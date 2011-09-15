#ifndef ICEANIMATIONMGR_H__
#define ICEANIMATIONMGR_H__

#include <Ogre.h>
#include <boost/enable_shared_from_this.hpp>

class iceAnimationMgr 
{
public:
	/**
    *  contructor
    */
	iceAnimationMgr();

	/**
    *  descontructor
    */
	~iceAnimationMgr();

	/**
    *  add new Animation
    */
	void addAnimation(Ogre::AnimationState*  animation, bool enable, bool loop);
	
	/**
    *  start an animation
    */
	void startAnimation(Ogre::String name);

	/**
    *  stop all animations
    */
	void stopAllAnimations();

	/**
    *  update current animation 
    */
	void update(Ogre::Real time);

private:
	Ogre::String activeAnimation;
	std::map<Ogre::String,Ogre::AnimationState*> mAnimations;
	std::map<Ogre::String,Ogre::AnimationState*>::iterator iter;
};

typedef boost::shared_ptr<iceAnimationMgr> iceAnimationMgrPtr;

#endif