#include "iceLocomotiveTrajectory.h"


iceLocomotiveTrajectory::iceLocomotiveTrajectory(void)
{
	//iceTrajectory();
		mDuration = 0;
	mCurrentTime = 0;
	mThereIsANodeToLookAt = false;
	mLoop = false;
}


iceLocomotiveTrajectory::~iceLocomotiveTrajectory(void)
{
}



void iceLocomotiveTrajectory::addTime(Ogre::Real p_fTime)
{	
	Ogre::Real fLocomotiveInterpolation = getInterpolationByTime(mCurrentTime + LOCOMOTIVE_ADVANCE);
	Ogre::Vector3 sLocomotivePosition = mTrack.interpolate(fLocomotiveInterpolation);
	mLocomotiveNode->setPosition(sLocomotivePosition);

	iceTrajectory::addTime(p_fTime);

	Ogre::Radian fRollAngle = getInterpolatedRollByTime(mCurrentTime);
	mNode->roll(fRollAngle);
}

void iceLocomotiveTrajectory::init(Ogre::SceneManager* p_spSceneManager, Ogre::SceneNode* p_psNode)
{
	iceTrajectory::init(p_spSceneManager,p_psNode);
	mLocomotiveNode = mNode->getParentSceneNode()->createChildSceneNode();
	setNodeToLookAt(mLocomotiveNode);
}

Ogre::Radian iceLocomotiveTrajectory::getInterpolatedRollByTime(Ogre::Real p_fTime)
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