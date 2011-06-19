#include "Entities\icePhysicEntity.h"


icePhysicEntity::icePhysicEntity(void)
{
}
icePhysicEntity::~icePhysicEntity(void)
{
}

void icePhysicEntity::initialize(Ogre::MovableObject* p_PhisicEntity)
{
	mPhisicEntity = p_PhisicEntity;
}

Ogre::AxisAlignedBox icePhysicEntity::getBoundingBox(void)
{
	return mPhisicEntity->getWorldBoundingBox();
}

void icePhysicEntity::showBoundingBox(void)
{
	mPhisicEntity->getParentSceneNode()->showBoundingBox(true);
}

void icePhysicEntity::hideBoundingBox(void)
{
	mPhisicEntity->getParentSceneNode()->showBoundingBox(false);
}