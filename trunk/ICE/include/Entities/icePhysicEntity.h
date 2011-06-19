#ifndef ICEPHYSIC_ENTITY_H_
#define ICEPHYSIC_ENTITY_H_

#include <OgreMovableObject.h>
#include <OgreSceneNode.h>

class icePhysicEntity
{
public:
	icePhysicEntity(void);	
	virtual ~icePhysicEntity(void);

	void initialize(Ogre::MovableObject* p_PhisicEntity);
	Ogre::AxisAlignedBox getBoundingBox(void);
	virtual void showBoundingBox(void);
	virtual void hideBoundingBox(void);

protected:
	Ogre::MovableObject* mPhisicEntity;
};
#endif