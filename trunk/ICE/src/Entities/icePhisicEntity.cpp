#include "Entities\icePhisicEntity.h"


icePhisicEntity::icePhisicEntity(void)
{
}
icePhisicEntity::~icePhisicEntity(void)
{
}

void icePhisicEntity::initialize(Ogre::MovableObject* p_PhisicEntity)
{
	mPhisicEntity = p_PhisicEntity;
}

Ogre::AxisAlignedBox icePhisicEntity::getBoundingBox(void)
{
	return mPhisicEntity->getWorldBoundingBox();
}