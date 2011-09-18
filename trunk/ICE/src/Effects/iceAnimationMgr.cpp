#include "Effects\iceAnimationMgr.h"

iceAnimationMgr::iceAnimationMgr(){
	activeAnimation ="";
}

iceAnimationMgr::~iceAnimationMgr(){
	mAnimations.clear();
}

void iceAnimationMgr::addAnimation(Ogre::AnimationState*  animation, bool enable, bool loop){
	mAnimations[animation->getAnimationName()] = animation;
	mAnimations[animation->getAnimationName()]->setEnabled(enable);
	mAnimations[animation->getAnimationName()]->setLoop(loop);
	if(enable) activeAnimation = animation->getAnimationName();
}
		
void iceAnimationMgr::startAnimation(Ogre::String name)
{
	stopAllAnimations();
	mAnimations[name]->setEnabled(true);
	activeAnimation = name;
}

void iceAnimationMgr::stopAllAnimations(){
	for( iter = mAnimations.begin() ; iter!=mAnimations.end(); ++iter)
	{
		((*iter).second)->setEnabled(false);
		((*iter).second)->setTimePosition(0);
	}
	activeAnimation ="";
}

void iceAnimationMgr::update(Ogre::Real time){
	if(activeAnimation != ""){
		((*mAnimations.find(activeAnimation)).second)->addTime(time); 
	}
}

void iceAnimationMgr::animationGoToStart(Ogre::String animationName)
{
	if(animationName == "")
	{
		if(activeAnimation == "")
		{
			return;
		}
		else
		{
			animationName = activeAnimation;
		}
	}
	mAnimations[animationName]->setTimePosition(0);
}

void iceAnimationMgr::animationGoToEnd(Ogre::String animationName)
{
	if(animationName == "")
	{
		if(activeAnimation == "")
		{
			return;
		}
		else
		{
			animationName = activeAnimation;
		}
	}
	mAnimations[animationName]->setTimePosition(mAnimations[animationName]->getLength());
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
	return mAnimations[animationName]->hasEnded();
}