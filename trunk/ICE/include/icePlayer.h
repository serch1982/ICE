#ifndef __ICE_PLAYER_H__
#define __ICE_PLAYER_H__

#include <OgreSceneNode.h>
#include <OgreEntity.h>
#include <OgreSceneManager.h>
#include <OgreParticleSystem.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

class icePlayer
{
	public:
		icePlayer();
		~icePlayer();
		bool initialize(Ogre::SceneManager* sceneManager, Ogre::SceneNode* node);
		void setCamera(Ogre::Camera* camera);
		void processMouseMoved(const OIS::MouseEvent &arg);
		void updateShipPosition(Ogre::Real frameTime);

	protected:
		void finalize();
		void update();
public:
		Ogre::SceneNode *playerNode, *cursorPlaneNode, *shipPlaneNode, *cameraPlaneNode, *cursorNode, *shipNode, *cameraNode;
		Ogre::Real shipMaxVelocity;
		Ogre::Camera* playerCamera;

};

#endif