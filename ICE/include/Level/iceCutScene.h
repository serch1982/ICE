#ifndef ICE_CUT_SCENE_H_
#define ICE_CUT_SCENE_H_

#include <Ogre.h>
#include "Entities/iceCutSceneEntity.h"


class iceCutScene
{
	public:
		iceCutScene();
		~iceCutScene();

		bool initialize(Ogre::String pInitFunctionStr,Ogre::String pUpdateFunctionStr, std::vector<iceTrajectory*>* pTrajectories);
		void filalize(void);
		void setActivationTime(Ogre::Real pActivationTime);

		void update(Ogre::Real p_TimeSinceLastFrame);
		
		void play(void);
		void stop(void);
		void rollback(void);

		Ogre::Real getActivationTime(void);
		bool hasEnded(void);

		Ogre::String getInitFunctionStr();
		Ogre::String getUpdateFunctionStr();
		int getStep();
		void setStep(int pStep);

		void nextStep();

		//pTrajectoryIndex == -1 => no trajectory
		void addEntity(iceCutSceneEntity::TYPE pEntityType, int pTrajectoryIndex = -1);
		//pTrajectoryIndex == -1 => no trajectory
		void initializeCameraEntity(int pTrajectoryIndex = -1);
		//Index == -1 => entity = camera
		void setEntityLookAt(int pEntityIndexLooking, int pEntityIndexLooked);

		//pEntityIndex == -1 => entity = camera
		bool isEntityTrajectoryEnded(int pEntityIndex = -1);

		void startEntityAnimation(int pEntityIndex, Ogre::String name);
		void stopEntityAnimations(int pEntityIndex);
		bool hasEntityAnimationEnded(int pEntityIndex, Ogre::String name);

		Ogre::Real getCurrentTime();

		void playSound(int i, float volume=1);
		void stopSound(int i);
		void stopAllSounds();

		void startEntity(int pEntityIndex);
		void stopEntity(int pEntityIndex);

		void showBlur();
		void hideBlur();

		void showToBeContinued();
		void hideToBeContinued();

	protected:
		//static Ogre::NameGenerator mNameGenerator;
		iceTrajectoryFollower* mCameraEntity;
		std::vector<iceCutSceneEntity*> mEntities;
		std::vector<iceTrajectory*>* mTrajectories;

		bool mIsPlaying;
		Ogre::Real mActivationTime;

		Ogre::SceneNode* mLastCameraNode;
		Ogre::String mInitFunctionStr;
		Ogre::String mUpdateFunctionStr;
		int mStep;

		void initializeEntities();
		void putBands(void);
		void removeBands(void);

		Ogre::Real mCurrentTime;
		bool cameraStarted;
};

#endif