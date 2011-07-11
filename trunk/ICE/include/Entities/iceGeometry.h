#ifndef ICEGEOMETRY_H__
#define ICEGEOMETRY_H__

#include <Ogre.h>

class PhysicsMeshInfo{
public:

    PhysicsMeshInfo(){};
    ~PhysicsMeshInfo(){};

public:

    size_t vertex_count;
    size_t index_count;
    unsigned long* indices;
    Ogre::Vector3* vertices;

};
   
class iceGeometry
{
public:
	iceGeometry(Ogre::Entity* physicsMesh);
	iceGeometry(Ogre::Vector3 boxDimension, Ogre::String name);
    ~iceGeometry();

	void initializeMeshInformation(Ogre::Vector3 position, Ogre::Quaternion orient, Ogre::Vector3 scale);

	Ogre::AxisAlignedBox getWorldBoundingBox(Ogre::Vector3 position);

    const PhysicsMeshInfo& getPhysicsMeshInfo()
    {
            return mPhysicsMesh;
    }

	Ogre::MovableObject* getMovableObject(){return mDebuggGeom;}

private:
	Ogre::ManualObject* debuggGeometry(Ogre::String name);
   
private:

    Ogre::Entity* mPhysicEntity;
	Ogre::Vector3 boundingBoxDimension;
    PhysicsMeshInfo mPhysicsMesh;
	Ogre::MovableObject* mDebuggGeom;

};

typedef boost::shared_ptr<iceGeometry> iceGeometryPtr;

#endif 