#include "iceTrajectory.h"


iceTrajectory::iceTrajectory(void)
{
	mDuration = 0;
	mCurrentTime = 0;
	mThereIsANodeToLookAt = false;
	mLoop = false;
}


iceTrajectory::~iceTrajectory(void)
{
}

void iceTrajectory::loadSteps(std::vector<iceStep> p_vSteps, const bool p_bIsLoop)
{
	mTrack.setAutoCalculate(false);
	mTrack.clear();
	mSteps = p_vSteps;

	//DEBUG {
		Ogre::SceneNode* sDebugTrajectorySteps = mSceneManager->getSceneNode("DebugTrajectorySteps");
	//} DEBUG
	for (unsigned int i=0; i<mSteps.size(); i++)
	{
		mTrack.addPoint(mSteps[i].getPosition());
	}
	mNode->setPosition(mSteps[0].getPosition());
	mDuration = mSteps[mSteps.size()-1].getTime();
	mTrack.recalcTangents();
	mLoop = p_bIsLoop;

	//DEBUG {
	int numSpheres = 100;

	for (int i=0;i<numSpheres;i++)
	{
			Ogre::SceneNode* sDebugNode = sDebugTrajectorySteps->createChildSceneNode(mTrack.interpolate((float)i/numSpheres));
			char name[50];
			sprintf(name,"key %d",i);
			Ogre::Entity* mesh = mSceneManager->createEntity(name, "sphere.mesh");
			sDebugNode->attachObject(mesh);
			sDebugNode->scale(0.005,0.005,0.005);
	}
	//} DEBUG
}

void iceTrajectory::addTime(Ogre::Real p_fTime)
{
	mLog = Ogre::LogManager::getSingleton().getLog( "iceLog.log" );
	mCurrentTime += p_fTime;

	if (mLoop && mCurrentTime >= mDuration)
		mCurrentTime -= mDuration;

	if (mCurrentTime < mDuration)
	{
		Ogre::Real fInterpolation = getInterpolationByTime(mCurrentTime);

		Ogre::Vector3 sNewPosition = mTrack.interpolate(fInterpolation);

		mNode->setPosition(sNewPosition);

		if (mThereIsANodeToLookAt)
			lookAt();
	}
}

void iceTrajectory::init(Ogre::SceneManager* p_spSceneManager, Ogre::SceneNode* p_psNode)
{
	mSceneManager = p_spSceneManager;
	mNode = p_psNode;
	mCurrentTime = 0;

	//DEBUG {
	if(!mSceneManager->hasSceneNode("DebugTrajectorySteps"))
		mSceneManager->getRootSceneNode()->createChildSceneNode("DebugTrajectorySteps");

	Ogre::SceneNode* playerCenter = mNode->createChildSceneNode();
	Ogre::Entity* mesh2 = mSceneManager->createEntity("player_center", "razor.mesh");
	playerCenter->attachObject(mesh2);
	playerCenter->scale(0.03,0.03,0.03);
	//} DEBUG
}

void iceTrajectory::setNodeToLookAt(Ogre::SceneNode* p_psNodeToLookAt)
{
	mNodeToLookAt = p_psNodeToLookAt;
	mThereIsANodeToLookAt = true;
}

void iceTrajectory::reset(void)
{
	mCurrentTime = 0;
}

void iceTrajectory::setLoop(bool p_bLoop)
{
	mLoop = p_bLoop;
}

bool iceTrajectory::hasEnded(void)
{
	return !mLoop && mCurrentTime >= mDuration;
}

unsigned int iceTrajectory::getCurrentStepIndexByTime(Ogre::Real p_fTime)
{
	unsigned int iCurrentStepIndex = 0;
	for(unsigned int i=0;i<mSteps.size();i++)
	{
		if (mSteps[i].getTime() > p_fTime)
		{
			iCurrentStepIndex = i-1;
			break;
		}
	}

	return iCurrentStepIndex;
}

Ogre::Real iceTrajectory::getInterpolationByTime(Ogre::Real p_fTime)
{
	unsigned int iCurrentStepIndex = getCurrentStepIndexByTime(p_fTime);

	Ogre::Real fTimeToNextStep = mSteps[iCurrentStepIndex+1].getTime() - mSteps[iCurrentStepIndex].getTime();
	Ogre::Real fTimeElapsedFromCurrentStep = p_fTime - mSteps[iCurrentStepIndex].getTime();
	Ogre::Real fCurrentStepInterpolation = fTimeElapsedFromCurrentStep/fTimeToNextStep;

	return (Ogre::Real)(iCurrentStepIndex + fCurrentStepInterpolation)/mSteps.size();
}

void iceTrajectory::lookAt(void)
{
	mNode->resetOrientation();

	Ogre::Vector3 sDiference = mNodeToLookAt->_getDerivedPosition() - mNode->_getDerivedPosition();
	Ogre::Real fDxz = sqrt(sDiference.x*sDiference.x + sDiference.z*sDiference.z);

	Ogre::Radian fYawAngle = Ogre::Radian(atan2(sDiference.x,sDiference.z));
	Ogre::Radian fPitchAngle = Ogre::Radian(atan(-sDiference.y/fDxz));

	mNode->yaw(fYawAngle);
	mNode->pitch(fPitchAngle);
}