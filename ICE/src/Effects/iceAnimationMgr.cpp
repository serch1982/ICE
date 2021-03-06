#include "Effects\iceAnimationMgr.h"

iceAnimationMgr::iceAnimationMgr(){
	activeAnimation ="";
	iddleAnimation = NULL;
	dyingAnimation = NULL;
}

iceAnimationMgr::~iceAnimationMgr(){
	mAnimations.clear();
}

void iceAnimationMgr::addAnimation(Ogre::AnimationState*  animation, Ogre::Real iddleWeight, bool stopOnEnd, bool loop){
	iceAnimation anim;
	anim.animation = animation;
	anim.iddleWeight = iddleWeight;
	anim.stopAtEnd = stopOnEnd;
	anim.animation->setLoop(loop);
	mAnimations[animation->getAnimationName()] = anim;
}
		
void iceAnimationMgr::startAnimation(Ogre::String name)
{
	stopAllAnimations();
	mAnimations[name].animation->setEnabled(true);
	activeAnimation = name;
	if(iddleAnimation)
		iddleAnimation->setWeight(mAnimations[name].iddleWeight);
}

void iceAnimationMgr::stopAllAnimations(){
	for( iter = mAnimations.begin() ; iter!=mAnimations.end(); ++iter)
	{
		((*iter).second).animation->setEnabled(false);
		((*iter).second).animation->setTimePosition(0);
	}
	activeAnimation ="";
	if(iddleAnimation)
		iddleAnimation->setWeight(1);
}

void iceAnimationMgr::update(Ogre::Real time){
	if(activeAnimation != ""){
		mAnimations[activeAnimation].animation->addTime(time);
		if(mAnimations[activeAnimation].stopAtEnd && mAnimations[activeAnimation].animation->hasEnded())
			stopAllAnimations();
	}
	if(iddleAnimation)
		iddleAnimation->addTime(time);
	if(dyingAnimation)
		dyingAnimation->addTime(time);
}

bool iceAnimationMgr::hasAnimationEnded(Ogre::String animationName)
{
	if(dyingAnimation && dyingAnimation->getEnabled())
	{
		return dyingAnimation->hasEnded();
	}

	if(animationName == "")
	{
		if(activeAnimation == "")
		{
			return true;
		}
		else
		{
			animationName = activeAnimation;
		}
	}
	return mAnimations[animationName].animation->hasEnded();
}


Ogre::String iceAnimationMgr::getNameCurrentAnimation()
{
	return activeAnimation == "" ? iddleAnimation->getAnimationName() : activeAnimation;
}

void iceAnimationMgr::setIddleAnimation(Ogre::AnimationState* pIddleAnimation)
{
	iddleAnimation = pIddleAnimation;
	iddleAnimation->setLoop(true);
	iddleAnimation->setEnabled(true);
}

void iceAnimationMgr::startIddleAnimation()
{
	iddleAnimation->setEnabled(true);
}

void iceAnimationMgr::stopIddleAnimation()
{
	iddleAnimation->setEnabled(false);
	iddleAnimation->setTimePosition(0);
}

void iceAnimationMgr::setDyingAnimation(Ogre::AnimationState* pDyingAnimation)
{
	dyingAnimation = pDyingAnimation;
	dyingAnimation->setEnabled(false);
	dyingAnimation->setLoop(false);
}

void iceAnimationMgr::startDyingAnimation()
{
	stopAllAnimations();
	dyingAnimation->setTimePosition(0);
	dyingAnimation->setEnabled(true);
	if(iddleAnimation)
		iddleAnimation->setWeight(0);
}
