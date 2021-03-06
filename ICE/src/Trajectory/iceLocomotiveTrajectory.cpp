#include "Trajectory\iceLocomotiveTrajectory.h"


iceLocomotiveTrajectory::iceLocomotiveTrajectory(void)
{
	//iceTrajectory();
	mDuration = 0;
	mCurrentTime = 0;
	mThereIsANodeToLookAt = false;
	mLoop = false;
}

iceLocomotiveTrajectory::iceLocomotiveTrajectory(std::vector<iceStep> p_vSteps, const bool p_bIsLoop)
{
	iceLocomotiveTrajectory();
	loadSteps(p_vSteps,p_bIsLoop);
}

iceLocomotiveTrajectory::~iceLocomotiveTrajectory(void)
{
}



void iceLocomotiveTrajectory::addTime(Ogre::Real p_fTime)
{	
	Ogre::Real fLocomotiveTime = mCurrentTime + LOCOMOTIVE_ADVANCE;
	if (mLoop && fLocomotiveTime > mDuration)
		fLocomotiveTime -= mDuration;
	Ogre::Real fLocomotiveInterpolation = getInterpolationByTime(fLocomotiveTime);
	Ogre::Vector3 sLocomotivePosition = mTrack.interpolate(fLocomotiveInterpolation);
	mLocomotiveNode->setPosition(sLocomotivePosition);

	iceTrajectory::addTime(p_fTime);

	//Ogre::Radian fRollAngle = getInterpolatedRollByTime(mCurrentTime);
	//mNode->roll(fRollAngle);
}

void iceLocomotiveTrajectory::init(Ogre::SceneManager* p_spSceneManager, Ogre::SceneNode* p_psNode)
{
	iceTrajectory::init(p_spSceneManager,p_psNode);
	mLocomotiveNode = mNode->getParentSceneNode()->createChildSceneNode();
	////DEBUG {
	//Ogre::Entity* sp = mSceneManager->createEntity("sphere.mesh");
	//mLocomotiveNode->attachObject(sp);
	//mLocomotiveNode->scale(0.01,0.01,0.01);
	////} DEBUG
	setNodeToLookAt(mLocomotiveNode);
}

//Ogre::Radian iceLocomotiveTrajectory::getInterpolatedRollByTime(Ogre::Real p_fTime)
//{
	//unsigned int iCurrentStepIndex = getCurrentStepIndexByTime(p_fTime);

	//Ogre::Real fTimeToNextStep = mSteps[iCurrentStepIndex+1].getTime() - mSteps[iCurrentStepIndex].getTime();
	//Ogre::Real fTimeElapsedFromCurrentStep = p_fTime - mSteps[iCurrentStepIndex].getTime();
	//Ogre::Real fCurrentStepInterpolation = fTimeElapsedFromCurrentStep/fTimeToNextStep;

	//Ogre::Radian fStartAngle = mSteps[iCurrentStepIndex].getRollAngle();
	//Ogre::Radian fEndAngle = mSteps[iCurrentStepIndex+1].getRollAngle();
	//Ogre::Radian fDiference = fEndAngle - fStartAngle;

	//if (fDiference.valueDegrees() > 180)
	//{
	//	fDiference -= Ogre::Degree(360);
	//}
	//else if (fDiference.valueDegrees() < -180)
	//{
	//	fDiference += Ogre::Degree(360);
	//}

	//return  fStartAngle + fDiference*fCurrentStepInterpolation;
//}