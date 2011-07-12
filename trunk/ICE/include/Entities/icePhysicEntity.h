#ifndef ICEPHYSIC_ENTITY_H_
#define ICEPHYSIC_ENTITY_H_

#include <Ogre.h>

#include "Entities\iceGeometry.h"

using namespace Ogre;

typedef std::vector<iceGeometryPtr> iceGeometryList;
typedef std::vector<iceGeometryPtr>::iterator iceGeometryIter;

class icePhysicEntity
{

public:
		
	virtual void initializePhysics(Ogre::String name, Ogre::Vector3 boxDimension)
	{
		mIceGeometryList.push_back( iceGeometryPtr(new iceGeometry(boxDimension, name)) );
	}

	virtual void initializeBossPhysics(Ogre::String name, Ogre::Vector3 boxDimension)
	{
		for(int i = 0; i < 8; i ++)
		{
			// One box for every leg
			char boxName[20];
			sprintf(boxName, "%s_%d", name.c_str(), i);
			iceGeometryPtr geom = iceGeometryPtr(new iceGeometry(boxDimension, boxName));
			mIceGeometryList.push_back( geom );
		}
	}


	virtual void finalizePhysics(){
		mIceGeometryList.clear();
	} 
	virtual void updatePhysics(const float elapsedSeconds){};

	Ogre::Real getSpeed(){return mSpeed;}
	void setSpeed(Ogre::Real speed){mSpeed = speed;}

	virtual iceGeometryPtr getGeometry()
	{
		return mIceGeometryList[0];
	}

	virtual iceGeometryPtr getBossGeometry(int index){ return mIceGeometryList[index]; }

private:
	Ogre::Real		mSpeed;

	// List of physics geometry
	iceGeometryList	mIceGeometryList;

};

typedef boost::shared_ptr<icePhysicEntity> icePhysicEntityPtr;

#endif 