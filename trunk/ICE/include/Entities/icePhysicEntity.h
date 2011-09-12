#ifndef ICEPHYSIC_ENTITY_H_
#define ICEPHYSIC_ENTITY_H_

#include <Ogre.h>
#include "Entities\iceGeometry.h"

typedef std::vector<iceGeometryPtr> iceGeometryList;
typedef std::vector<iceGeometryPtr>::iterator iceGeometryIter;

class icePhysicEntity
{

public:
		
	virtual void initializePhysics(Ogre::String name, Ogre::Vector3 boxDimension)
	{
		mIceGeometryList.push_back( iceGeometryPtr(new iceGeometry(boxDimension, name)) );
	}

	virtual void finalizePhysics(){
		mIceGeometryList.clear();
	}

	virtual iceGeometryPtr getGeometry()
	{
		return mIceGeometryList[0];
	}

	virtual iceGeometryPtr getGeometry(int index)
	{ 
		return mIceGeometryList[index]; 
	}

	virtual int getNumBoxes()
	{
		return mIceGeometryList.size();
	}

	virtual void setPhysicsDebug(bool bdebug)
	{
		for(unsigned i = 0; i <  mIceGeometryList.size(); i++){
			mIceGeometryList[i]->getMovableObject()->setVisible(bdebug);
		}
	}

private:
	// List of physics geometry
	iceGeometryList	mIceGeometryList;

};

typedef boost::shared_ptr<icePhysicEntity> icePhysicEntityPtr;

#endif 