#ifndef ICEANIMATIONMGR_H__
#define ICEANIMATIONMGR_H__

#include <Ogre.h>
#include <boost/enable_shared_from_this.hpp>

struct iceAnimation
{
	Ogre::AnimationState* animation;
	Ogre::Real iddleWeight;
	bool stopAtEnd;
};

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
	void addAnimation(Ogre::AnimationState*  animation, Ogre::Real iddleWeight = 1.0 , bool stopOnEnd = false, bool loop = false);
	
	/**
    *  start an animation
    */
	void startAnimation(Ogre::String name);

	void startIddleAnimation();
	void stopIddleAnimation();

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
	void setIddleAnimation(Ogre::AnimationState* pIddleAnimation);
	void setDyingAnimation(Ogre::AnimationState* pDyingAnimation);
	void startDyingAnimation();
private:
	Ogre::String activeAnimation;
	Ogre::AnimationState* iddleAnimation;
	Ogre::AnimationState* dyingAnimation;
	std::map<Ogre::String,iceAnimation> mAnimations;
	std::map<Ogre::String,iceAnimation>::iterator iter;
};

typedef boost::shared_ptr<iceAnimationMgr> iceAnimationMgrPtr;

#endif