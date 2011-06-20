#ifndef ICEOBJECT_H_
#define ICEOBJECT_H_

#include "icePhysicEntity.h"
#include <OgreEntity.h>

class iceObject :  public icePhysicEntity
{
	public:
		iceObject(Ogre::Entity* entity);
		~iceObject();

	//private:
		
};

#endif