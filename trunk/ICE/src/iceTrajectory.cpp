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
		//DEBUG {
			Ogre::SceneNode* sDebugNode = sDebugTrajectorySteps->createChildSceneNode(mSteps[i].getPosition());
			char name[100];
			sprintf(name,"key %d",i);
			Ogre::Entity* mesh = mSceneManager->createEntity(name, "razor.mesh");
			sDebugNode->attachObject(mesh);
			//debugNode->scale(40,40,40);
			//sDebugNode->rotate(rotation);
		//} DEBUG
	}
	mDuration = mSteps[mSteps.size()-1].getTime();
	mTrack.recalcTangents();


	//DEBUG	
	
		//Ogre::Quaternion rotation = initialDirection.getRotationTo(steps[i+1].position - steps[i].position);
		//rotation = rotation * Ogre::Quaternion(steps[i].rollAngle,Ogre::Vector3::UNIT_Z);



}

void iceTrajectory::addTime(Ogre::Real p_fTime)
{
	mCurrentTime += p_fTime;
	if (mDuration < mCurrentTime + LOCOMOTIVE_ADVANCE)
		mCurrentTime = 0;
	Ogre::Real fInterpolation = getInterpolationByTime(mCurrentTime);
	Ogre::Real fLocomotiveInterpolation = getInterpolationByTime(mCurrentTime + LOCOMOTIVE_ADVANCE);

	Ogre::Vector3 sNewPosition = mTrack.interpolate(fInterpolation);
	Ogre::Vector3 sLocomotivePosition = mTrack.interpolate(fLocomotiveInterpolation);

	mNode->setPosition(sNewPosition);
	mLocomotiveNode->setPosition(sLocomotivePosition);
	//mNode->lookAt(sLocomotivePosition,Ogre::Node::TransformSpace::TS_WORLD,Ogre::Vector3::UNIT_Z);

	Ogre::Radian fInterpolatedRoll = getInterpolatedRollByTime(mCurrentTime);
	Ogre::Radian fCurrentRoll = mNode->getOrientation().getRoll();

	//mNode->rotate(Ogre::Vector3::UNIT_Z,fInterpolatedRoll-fCurrentRoll,Ogre::Node::TransformSpace::TS_WORLD);

}

void iceTrajectory::init(Ogre::SceneManager* p_spSceneManager, Ogre::SceneNode* p_psNode)
{
	mSceneManager = p_spSceneManager;
	mNode = p_psNode;
	mCurrentTime = 0;
	mLocomotiveNode = mNode->getParentSceneNode()->createChildSceneNode();
	mNode->setAutoTracking(true,mLocomotiveNode,Ogre::Vector3::UNIT_Z);

	//DEBUG {	
	mSceneManager->getRootSceneNode()->createChildSceneNode("DebugTrajectorySteps");
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

	return  mSteps[iCurrentStepIndex].getRollAngle() +
		    ( mSteps[iCurrentStepIndex+1].getRollAngle() - 
			  mSteps[iCurrentStepIndex].getRollAngle()
			)*fCurrentStepInterpolation;

}