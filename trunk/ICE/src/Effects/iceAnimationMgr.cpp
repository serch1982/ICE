#include "Effects\iceAnimationMgr.h"

iceAnimationMgr::iceAnimationMgr(){
	activeAnimation ="";
}

iceAnimationMgr::~iceAnimationMgr(){
	mAnimations.clear();
}

void iceAnimationMgr::addAnimation(Ogre::AnimationState*  animation, Ogre::Real iddleWeight, bool stopOnEnd, bool loop, bool asdefault){
	iceAnimation anim;
	anim.animation = animation;
	anim.animation->setWeight(iddleWeight);
	anim.stopAtEnd = stopOnEnd;
	anim.animation->setLoop(loop);
	anim.asDefault = asdefault;
	mAnimations[animation->getAnimationName()] = anim;
}
		
void iceAnimationMgr::startAnimation(Ogre::String name)
{
	stopAllAnimations();
	mAnimations[name].animation->setEnabled(true);
	activeAnimation = name;
	//iddleAnimation->setWeight(mAnimations[name].iddleWeight);
}

void iceAnimationMgr::stopAllAnimations(){
	for( iter = mAnimations.begin() ; iter!=mAnimations.end(); ++iter)
	{
		((*iter).second).animation->setEnabled(false);
		((*iter).second).animation->setTimePosition(0);
	}
	activeAnimation ="";
	//iddleAnimation->setWeight(1);
}

void iceAnimationMgr::update(Ogre::Real time){
	if(activeAnimation != ""){
		mAnimations[activeAnimation].animation->addTime(time);
		if(mAnimations[activeAnimation].stopAtEnd && mAnimations[activeAnimation].animation->hasEnded())
			stopAllAnimations();
	}
}

bool iceAnimationMgr::hasAnimationEnded(Ogre::String animationName)
{
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
	return activeAnimation;
}

void iceAnimationMgr::startIddleAnimation()
{
	stopAllAnimations();
	for( iter = mAnimations.begin() ; iter!=mAnimations.end(); ++iter)
	{
		if(((*iter).second).asDefault == true){
			((*iter).second).animation->setEnabled(true);
			activeAnimation = (*iter).first;
			return;
		}
	}
}