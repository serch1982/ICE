#ifndef ICEANIMATIONMGR_H__
#define ICEANIMATIONMGR_H__

#include <Ogre.h>
#include <boost/enable_shared_from_this.hpp>

struct iceAnimation
{
	Ogre::AnimationState* animation;
	Ogre::Real iddleWeight;
	bool stopAtEnd;
	bool asDefault;
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
	void addAnimation(Ogre::AnimationState*  animation, Ogre::Real iddleWeight = 1.0 , 
					 bool stopOnEnd = false, bool loop = false, bool asdefault = false);
	
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
    *  get the name of the current animation
    */
	Ogre::String getNameCurrentAnimation();


	/**
    *  active the default animation 
    */
	void startIddleAnimation();
private:
	Ogre::String activeAnimation;
	std::map<Ogre::String,iceAnimation> mAnimations;
	std::map<Ogre::String,iceAnimation>::iterator iter;
};

typedef boost::shared_ptr<iceAnimationMgr> iceAnimationMgrPtr;

#endif