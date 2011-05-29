#ifndef ICEPHISIC_ENTITY_H_
#define ICEPHISIC_ENTITY_H_

#include <OgreMovableObject.h>

class icePhisicEntity
{
public:
	icePhisicEntity(void);	
	virtual ~icePhisicEntity(void);

	void initialize(Ogre::MovableObject* p_PhisicEntity);
	Ogre::AxisAlignedBox getBoundingBox(void);

protected:
	Ogre::MovableObject* mPhisicEntity;
};
#endif