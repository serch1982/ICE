#include "iceTrajectory.h"


iceTrajectory::iceTrajectory(void)
{
	mDuration = 0;
	mCurrentTime = 0;
}


iceTrajectory::~iceTrajectory(void)
{
}

void iceTrajectory::loadSteps(vector<iceStep> p_vSteps)
{
	mTrack.setAutoCalculate(false);
	mSteps = p_vSteps;

	//DEBUG {
		Ogre::SceneNode* sDebugTrajectorySteps = mSceneManager->getSceneNode("DebugTrajectorySteps");
	//} DEBUG
	for (unsigned int i=0; i<mSteps.size(); i++)
	{
		mTrack.addPoint(mSteps[i].getPosition());
	}
	mDuration = mSteps[mSteps.size()-1].getTime();
	mTrack.recalcTangents();

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
	if (mDuration < mCurrentTime + LOCOMOTIVE_ADVANCE)
		mCurrentTime = 0;
	Ogre::Real fInterpolation = getInterpolationByTime(mCurrentTime);
	Ogre::Real fLocomotiveInterpolation = getInterpolationByTime(mCurrentTime + LOCOMOTIVE_ADVANCE);

	Ogre::Vector3 sNewPosition = mTrack.interpolate(fInterpolation);
	Ogre::Vector3 sLocomotivePosition = mTrack.interpolate(fLocomotiveInterpolation);

	mNode->setPosition(sNewPosition);
	mLocomotiveNode->setPosition(sLocomotivePosition);
	mNode->resetOrientation();

	Ogre::Vector3 sDiference = sLocomotivePosition - sNewPosition;
	Ogre::Real fDxz = sqrt(sDiference.x*sDiference.x + sDiference.z*sDiference.z);

	Ogre::Radian fYawAngle = Ogre::Radian(atan2(sDiference.x,sDiference.z));
	Ogre::Radian fPitchAngle = Ogre::Radian(atan(-sDiference.y/fDxz));

	mNode->yaw(fYawAngle);
	mNode->pitch(fPitchAngle);

	Ogre::Radian fRollAngle = getInterpolatedRollByTime(mCurrentTime);
	mNode->roll(fRollAngle);
}

void iceTrajectory::init(Ogre::SceneManager* p_spSceneManager, Ogre::SceneNode* p_psNode)
{
	mSceneManager = p_spSceneManager;
	mNode = p_psNode;
	mCurrentTime = 0;
	mLocomotiveNode = mNode->getParentSceneNode()->createChildSceneNode();
	//mNode->setAutoTracking(true,mLocomotiveNode,Ogre::Vector3::UNIT_Z);

	//DEBUG {	
	mSceneManager->getRootSceneNode()->createChildSceneNode("DebugTrajectorySteps");
	Ogre::Entity* mesh = mSceneManager->createEntity("locomotive", "sphere.mesh");
	mLocomotiveNode->attachObject(mesh);
	mLocomotiveNode->scale(0.01,0.01,0.01);

	Ogre::SceneNode* playerCenter = mNode->createChildSceneNode();
	Ogre::Entity* mesh2 = mSceneManager->createEntity("player_center", "razor.mesh");
	playerCenter->attachObject(mesh2);
	playerCenter->scale(0.03,0.03,0.03);
	//} DEBUG
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

Ogre::Radian iceTrajectory::getInterpolatedRollByTime(Ogre::Real p_fTime)
{
	unsigned int iCurrentStepIndex = getCurrentStepIndexByTime(p_fTime);

	Ogre::Real fTimeToNextStep = mSteps[iCurrentStepIndex+1].getTime() - mSteps[iCurrentStepIndex].getTime();
	Ogre::Real fTimeElapsedFromCurrentStep = p_fTime - mSteps[iCurrentStepIndex].getTime();
	Ogre::Real fCurrentStepInterpolation = fTimeElapsedFromCurrentStep/fTimeToNextStep;

	Ogre::Radian fStartAngle = mSteps[iCurrentStepIndex].getRollAngle();
	Ogre::Radian fEndAngle = mSteps[iCurrentStepIndex+1].getRollAngle();
	Ogre::Radian fDiference = fEndAngle - fStartAngle;

	if (fDiference.valueDegrees() > 180)
	{
		fDiference -= Ogre::Degree(360);
	}
	else if (fDiference.valueDegrees() < -180)
	{
		fDiference += Ogre::Degree(360);
	}

	return  fStartAngle + fDiference*fCurrentStepInterpolation;
}