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
	void addAnimation(Ogre::AnimationState*  animation, bool enable = false, bool loop = false);
	
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
	
	
	/**
    *  the animation has ended
    */
	bool hasAnimationEnded(Ogre::String animationName = "");

	/**
    *  get hte name of the current animation
    */
	Ogre::String getNameCurrentAnimation();
private:
	Ogre::String activeAnimation;
	std::map<Ogre::String,Ogre::AnimationState*> mAnimations;
	std::map<Ogre::String,Ogre::AnimationState*>::iterator iter;
};

typedef boost::shared_ptr<iceAnimationMgr> iceAnimationMgrPtr;

#endif