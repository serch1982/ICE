#ifndef ICE_CUT_SCENE_H_
#define ICE_CUT_SCENE_H_

#include <Ogre.h>
#include "Entities/iceCutSceneEntity.h"


class iceCutScene
{
	public:
		iceCutScene();
		~iceCutScene();

		bool initialize(Ogre::String luaScript, std::vector<iceTrajectory*>& trajectories, Ogre::Real pActivationTime);
		void filalize(void);

		void update(Ogre::Real p_TimeSinceLastFrame);
		
		void play(void);
		void rollback(void);

		Ogre::Real getActivationTime(void);
		bool hasEnded(void);

	protected:
		//static Ogre::NameGenerator mNameGenerator;
		iceTrajectoryFollower* cameraEntity;
		std::vector<iceCutSceneEntity*> entities;

		bool mIsPlaying;
		Ogre::Real mActivationTime;

		Ogre::SceneNode* lastCameraNode;

		void initializeEntities(std::vector<iceTrajectory*>& trajectories);
		void initializeCameraEntity(std::vector<iceTrajectory*>& trajectories);
		void putBands(void);
		void removeBands(void);
};

#endif