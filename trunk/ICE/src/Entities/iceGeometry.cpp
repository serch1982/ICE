#include "Entities\iceGeometry.h"

iceGeometry::iceGeometry(Ogre::Entity* physicMesh)
{
	mPhysicEntity = physicMesh;
}

iceGeometry::~iceGeometry()
{

}

iceGeometry::iceGeometry(Ogre::Vector3 boxDimension, Ogre::String name)
{
	boundingBoxDimension = boxDimension; 
	mDebuggGeom = debuggGeometry(name);
}

void iceGeometry::initializeMeshInformation(Ogre::Vector3 position, Ogre::Quaternion orient, Ogre::Vector3 scale){

	size_t &vertex_count = mPhysicsMesh.vertex_count;
	Ogre::Vector3* &vertices = mPhysicsMesh.vertices;
	size_t &index_count = mPhysicsMesh.index_count;
	unsigned long* &indices = mPhysicsMesh.indices;

	Ogre::MeshPtr mesh = mPhysicEntity->getMesh();

    bool added_shared = false;
    size_t current_offset = 0;
    size_t shared_offset = 0;
    size_t next_offset = 0;
    size_t index_offset = 0;

    vertex_count = index_count = 0;

    // Calculate how many vertices and indices we're going to need
    for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh( i );

        // We only need to add the shared vertices once
        if(submesh->useSharedVertices)
        {
            if( !added_shared )
            {
                vertex_count += mesh->sharedVertexData->vertexCount;
                added_shared = true;
            }
        }
        else
        {
            vertex_count += submesh->vertexData->vertexCount;
        }

        // Add the indices
        index_count += submesh->indexData->indexCount;
    }


    // Allocate space for the vertices and indices
    vertices = new Ogre::Vector3[vertex_count];
    indices = new unsigned long[index_count];

    added_shared = false;

    // Run through the submeshes again, adding the data into the arrays
    for ( unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
    {
        Ogre::SubMesh* submesh = mesh->getSubMesh(i);

        Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;

        if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
        {
            if(submesh->useSharedVertices)
            {
                added_shared = true;
                shared_offset = current_offset;
            }

            const Ogre::VertexElement* posElem =
                vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

            Ogre::HardwareVertexBufferSharedPtr vbuf =
                vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

            unsigned char* vertex =
                static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

            float* pReal;

            for( size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
            {
                posElem->baseVertexPointerToElement(vertex, &pReal);

                Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

                vertices[current_offset + j] = (orient * (pt * scale)) + position;
            }

            vbuf->unlock();
            next_offset += vertex_data->vertexCount;
        }


        Ogre::IndexData* index_data = submesh->indexData;
        size_t numTris = index_data->indexCount / 3;
        Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

        bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

        unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


        size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;

        if ( use32bitindexes )
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset);
            }
        }
        else
        {
            for ( size_t k = 0; k < numTris*3; ++k)
            {
                indices[index_offset++] = static_cast<unsigned long>(pShort[k]) +
                    static_cast<unsigned long>(offset);
            }
        }

        ibuf->unlock();
        current_offset = next_offset;
    }
}

Ogre::AxisAlignedBox iceGeometry::getWorldBoundingBox(Ogre::Vector3 position)
{
	Ogre::Vector3 min = position - boundingBoxDimension/2;
	Ogre::Vector3 max = position + boundingBoxDimension/2;
	Ogre::AxisAlignedBox boundingBox(min, max);
	return boundingBox;
}

Ogre::ManualObject* iceGeometry::debuggGeometry(Ogre::String name)
{
	
	Ogre::Vector3 box = boundingBoxDimension;

	float x = box.x/2;
	float y = box.y/2;
	float z = box.z/2;

	Ogre::ManualObject* m_pManObj = new Ogre::ManualObject(name+"manObject");

	// tell OGRE we use the OT_TRIANGLE_STRIP to draw
	m_pManObj->begin("",Ogre::RenderOperation::OT_LINE_STRIP);

	m_pManObj->position(x,-y,z);
	m_pManObj->position(x,y,z);
	m_pManObj->position(-x,y,z);
	m_pManObj->position(-x,-y,z);
	m_pManObj->position(x,-y,z);
	m_pManObj->position(x,-y,-z); 
	m_pManObj->position(-x,-y,-z);
	m_pManObj->position(-x,-y,z);
	m_pManObj->position(-x,y,z);
	m_pManObj->position(-x,y,-z); 
	m_pManObj->position(x,y,-z);
	m_pManObj->position(x,y,z);
	m_pManObj->position(x,y,-z);
	m_pManObj->position(x,-y,-z);
	m_pManObj->position(-x,-y,-z);
	m_pManObj->position(-x,y,-z);

	m_pManObj->end(); // end of a TRIANGLE_STRIP

	m_pManObj->setVisible(false);

	return m_pManObj;

}



