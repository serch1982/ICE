#include "Entities\iceObject.h"

iceObject::iceObject(Ogre::SceneNode* node,Ogre::Entity* entity){
	mEntity = entity;
	mObjectNode = node;
	//icePhysicEntity::initializePhysics(entity->getName(),entity->getBoundingBox().getSize());
}

iceObject::~iceObject(){

}



void iceObject::setDebugMode(bool isDebug){
	mObjectNode->showBoundingBox(isDebug);
}
		

