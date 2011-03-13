#include "iceTrajectory.h"


iceTrajectory::iceTrajectory(void)
{
	duration = 0;
}


iceTrajectory::~iceTrajectory(void)
{
}

void iceTrajectory::addStep(iceTrajectoryStep step)
{
	steps.push_back(step);
	duration = max(duration,step.time);
}

void iceTrajectory::addTime(Ogre::Real time)
{
	animState->addTime(time);
}

void iceTrajectory::loadSteps(Ogre::SceneManager* sceneManager, Ogre::SceneNode* node)
{
	Ogre::Animation* anim = sceneManager->createAnimation("Trajectory", duration);
	anim->setInterpolationMode(Ogre::Animation::IM_SPLINE);
	anim->setRotationInterpolationMode(Ogre::Animation::RIM_SPHERICAL);

    Ogre::NodeAnimationTrack* track = anim->createNodeTrack(0, node);
    // Setup keyframes

	Ogre::Vector3 initialDirection = node->getOrientation() * Ogre::Vector3::UNIT_Z;
	for (unsigned int i=0;i<steps.size()-1;i++)
	{
		Ogre::TransformKeyFrame* key = track->createNodeKeyFrame(steps[i].time);
		key->setTranslate(steps[i].position);


		Ogre::Quaternion rotation = initialDirection.getRotationTo(steps[i+1].position - steps[i].position);
		rotation = rotation * Ogre::Quaternion(steps[i].rollAngle,Ogre::Vector3::UNIT_Z);
		key->setRotation(rotation);
		//key->setRotation(Ogre::Quaternion(Ogre::Degree((-90*i)%360),Ogre::Vector3::UNIT_Y));

		//DEBUG
		Ogre::SceneNode* debugNode = sceneManager->getRootSceneNode()->createChildSceneNode(steps[i].position);
		char name[100];
		sprintf(name,"key %d",i);
		Ogre::Entity* mesh = sceneManager->createEntity(name, "fish.mesh");
		debugNode->attachObject(mesh);
		debugNode->scale(40,40,40);
		debugNode->rotate(Ogre::Quaternion(Ogre::Degree(90),Ogre::Vector3::UNIT_Y) * rotation);
	}

	track->setUseShortestRotationPath(true);

    // Create a new animation state to track this
    animState = sceneManager->createAnimationState("Trajectory");
	animState->setEnabled(true);
}
