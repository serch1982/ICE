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
		
void iceAnimationMgr::startAnimation(Ogre::String name){
	for(iter = mAnimations.begin() ; iter!=mAnimations.end(); ++iter)
	{
		if(((*iter).first) == name){ 
			((*iter).second)->setEnabled(true);
			activeAnimation = name;
		}else{
			((*iter).second)->setEnabled(false);
			if(!((*iter).second)->getLoop()) ((*iter).second)->setTimePosition(0);
		}
	}
}

void iceAnimationMgr::stopAllAnimations(){
	for( iter = mAnimations.begin() ; iter!=mAnimations.end(); ++iter)
	{
		((*iter).second)->setEnabled(false);
	}
	activeAnimation ="";
}

void iceAnimationMgr::update(Ogre::Real time){
	if(activeAnimation != ""){
		((*mAnimations.find(activeAnimation)).second)->addTime(time); 
	}
}