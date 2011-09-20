#ifndef ICEOBJECT_H_
#define ICEOBJECT_H_

#include "icePhysicEntity.h"
#include <OgreEntity.h>
#include <OgreSceneNode.h>

class iceObject :  public icePhysicEntity
{
	public:
		iceObject(Ogre::SceneNode* node, Ogre::Entity* entity);
		~iceObject();

		Ogre::String getName(void);
		void setDebugMode(bool isDebug);
		Ogre::Vector3 getWorldPosition(void);
	private:
		Ogre::Entity* mEntity;
		Ogre::SceneNode* mObjectNode;
};

#endif