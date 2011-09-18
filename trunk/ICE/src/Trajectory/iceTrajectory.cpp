#include "Trajectory\iceTrajectory.h"
#include <algorithm>
#include "iceGame.h"

Ogre::NameGenerator iceTrajectory::mNameGenerator("Trajectory_");

iceTrajectory::iceTrajectory(void)
{
	mDuration = 0;
	mCurrentTime = 0;
	mThereIsANodeToLookAt = false;
	mLoop = false;
	mLog = iceGame::getGameLog();
	mNode = NULL;
}

iceTrajectory::iceTrajectory(std::vector<iceStep> p_vSteps, const bool p_bIsLoop)
{
	iceTrajectory::iceTrajectory();
	mDuration = 0;
	mCurrentTime = 0;
	mThereIsANodeToLookAt = false;
	mLoop = false;
	mLog = iceGame::getGameLog();
	mNode = NULL;
	loadSteps(p_vSteps,p_bIsLoop);
}


iceTrajectory::~iceTrajectory(void)
{
}

void iceTrajectory::loadSteps(std::vector<iceStep> p_vSteps, const bool p_bIsLoop)
{
	mTrack.setAutoCalculate(false);
	mTrack.clear();
	mSteps = p_vSteps;
	std::sort(mSteps.begin(),mSteps.end());

	//DEBUG {
		//Ogre::SceneNode* sDebugTrajectorySteps = mSceneManager->getSceneNode("DebugTrajectorySteps");
	//} DEBUG
	for (unsigned int i=0; i<mSteps.size(); i++)
	{
		mTrack.addPoint(mSteps[i].getPosition());
	}

	mDuration = mSteps[mSteps.size()-1].getTime();
	mTrack.recalcTangents();
	mLoop = p_bIsLoop;
	if(mNode)
		mNode->setPosition(mSteps[0].getPosition());
	//DEBUG {
	//int numSpheres = 3000;

	//if(!Ogre::ResourceGroupManager::getSingleton().resourceGroupExists("debugger"))
	//	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("debugger");
	//Ogre::ManualObject* myManualObject =  iceGame::getSceneManager()->createManualObject();
	//Ogre::SceneNode* sDebugNode = iceGame::getSceneManager()->getRootSceneNode()->createChildSceneNode();
	//Ogre::StringStream materialName;
	//materialName << "Material_"  << mNameGenerator.generate();
	//Ogre::MaterialPtr myManualObjectMaterial = Ogre::MaterialManager::getSingleton().create(materialName.str(),"debugger");

	//myManualObjectMaterial->setReceiveShadows(false); 
	//myManualObjectMaterial->getTechnique(0)->setLightingEnabled(true); 
	//myManualObjectMaterial->getTechnique(0)->getPass(0)->setDiffuse(0,0,1,0); 
	//myManualObjectMaterial->getTechnique(0)->getPass(0)->setAmbient(0,0,1); 
	//myManualObjectMaterial->getTechnique(0)->getPass(0)->setSelfIllumination(0,0,1); 
	////myManualObjectMaterial->dispose();  // dispose pointer, not the material


	//myManualObject->begin(materialName.str(), Ogre::RenderOperation::OT_LINE_LIST); 
	//for (int i=0;i<numSpheres;i++)
	//{
	//		myManualObject->position(mTrack.interpolate((float)i/numSpheres));
	//}

	//myManualObject->end(); 
 //
	//sDebugNode->attachObject(myManualObject);

	//for (unsigned int i=0;i<mSteps.size(); i++)
	//{
	//	Ogre::SceneNode* sStepNode = sDebugNode->createChildSceneNode(mSteps[i].getPosition());
	//	Ogre::Entity* mesh = iceGame::getSceneManager()->createEntity("sphere.mesh");
	//	sStepNode->attachObject(mesh);
	//	sStepNode->scale(0.03,0.03,0.03);
	//}
	//} DEBUG
}

void iceTrajectory::addTime(Ogre::Real p_fTime)
{
	
	if(mLoop && mCurrentTime == mDuration)
		mCurrentTime = 0;

	mCurrentTime += p_fTime;

	if (mCurrentTime > mDuration)
		mCurrentTime = mDuration;

	Ogre::Real fInterpolation = getInterpolationByTime(mCurrentTime);

	Ogre::Vector3 sNewPosition = mTrack.interpolate(fInterpolation);
	//new optional
	Ogre::Vector3  npos = sNewPosition - mNode->_getDerivedPosition();
	mNode->translate(npos);
	//mNode->setPosition(sNewPosition);
	
	if (mThereIsANodeToLookAt)
		lookAt();
}

void iceTrajectory::init(Ogre::SceneManager* p_spSceneManager, Ogre::SceneNode* p_psNode)
{
	mSceneManager = p_spSceneManager;
	mNode = p_psNode;
	mCurrentTime = 0;
	if(mSteps.size()>0)
		mNode->setPosition(mSteps[0].getPosition());

	//DEBUG {
	//if(!mSceneManager->hasSceneNode("DebugTrajectorySteps"))
	//	mSceneManager->getRootSceneNode()->createChildSceneNode("DebugTrajectorySteps");

	//Ogre::SceneNode* playerCenter = mNode->createChildSceneNode();
	//Ogre::Entity* mesh2 = mSceneManager->createEntity(mNameGenerator.generate(), "razor.mesh");
	//playerCenter->attachObject(mesh2);
	//playerCenter->scale(0.03,0.03,0.03);
	//} DEBUG
}

void iceTrajectory::setNodeToLookAt(Ogre::SceneNode* p_psNodeToLookAt)
{
	mNodeToLookAt = p_psNodeToLookAt;
	if(mNodeToLookAt)
	{
		mThereIsANodeToLookAt = true;
		if(mNode)
			mNode->setInheritOrientation(false); //Si vamos a controlar a donde mira el nodo, mejor que no herede la orientacion del padre
	}
	else
	{
		mThereIsANodeToLookAt = false;
	}
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

	Ogre::Real interpolation = 0;
	if(mLoop)
		interpolation = (iCurrentStepIndex + fCurrentStepInterpolation)/(mSteps.size()-1);
	else
		interpolation = (iCurrentStepIndex + fCurrentStepInterpolation)/mSteps.size();

	return interpolation;
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

void iceTrajectory::goToLastStep(void)
{
	mNode->_setDerivedPosition(mSteps.back().getPosition());
	mCurrentTime = mSteps.back().getTime() + 1;
}