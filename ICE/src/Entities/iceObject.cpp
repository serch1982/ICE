#include "Entities\iceObject.h"

iceObject::iceObject(Ogre::SceneNode* node,Ogre::Entity* entity){
	mEntity = entity;
	icePhysicEntity::initializePhysics(entity->getName(),entity->getBoundingBox().getSize());
	node->attachObject(getGeometry()->getMovableObject());
	mObjectNode = node;
}

iceObject::~iceObject(){
	icePhysicEntity::finalizePhysics();
}

void iceObject::setDebugMode(bool isDebug){
	setPhysicsDebug(isDebug);
}
		
Ogre::Vector3 iceObject::getWorldPosition(void){
	return mObjectNode->_getDerivedPosition();
}

Ogre::String iceObject::getName(void){
	return mEntity->getName();
}
