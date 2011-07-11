#include "Entities\iceObject.h"

iceObject::iceObject(Ogre::SceneNode* node,Ogre::Entity* entity){
	mEntity = entity;
	//icePhysicEntity::initializePhysics(entity->getName(),entity->getBoundingBox().getSize());
}
iceObject::~iceObject(){

}

		

