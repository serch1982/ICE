#ifndef ICEPHYSIC_ENTITY_H_
#define ICEPHYSIC_ENTITY_H_

#include <OgreMovableObject.h>

class icePhysicEntity
{
public:
	icePhysicEntity(void);	
	virtual ~icePhysicEntity(void);

	void initialize(Ogre::MovableObject* p_PhisicEntity);
	Ogre::AxisAlignedBox getBoundingBox(void);

protected:
	Ogre::MovableObject* mPhisicEntity;
};
#endif