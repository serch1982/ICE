#ifndef ICEOBJECT_H_
#define ICEOBJECT_H_

//#include "icePhysicEntity.h"
#include <OgreEntity.h>
#include <OgreSceneNode.h>

class iceObject //:  public icePhysicEntity
{
	public:
		iceObject(Ogre::SceneNode* node, Ogre::Entity* entity);
		~iceObject();


		void setDebugMode(bool isDebug);

		Ogre::AxisAlignedBox getBox(void) {return mEntity->getBoundingBox();}

	private:
		Ogre::Entity* mEntity;
		Ogre::SceneNode* mObjectNode;
};

#endif