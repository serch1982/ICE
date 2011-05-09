/*
-----------------------------------------------------------------------------
Wyverns' Assault 3D Videgame.
Copyright (C) 2010  Giorgio Tino, Javier Soto Huesa, Jordi Carreras Ribot, 
					Marc Serena, Elm Oliver Torres

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

-----------------------------------------------------------------------------
*/
#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include <Ogre.h>

using namespace Ogre;

namespace WyvernsAssault
{
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
   
    class Geometry
    {
	public:
	
		//Ground geometry contructor
		Geometry(Ogre::Entity* physicsMesh);
		//Object geometry contructor
		Geometry(Ogre::Vector3 boxDimension, Ogre::String name, Ogre::String material);
        ~Geometry();

		void initializeMeshInformation(Ogre::Vector3 position, Ogre::Quaternion orient, Ogre::Vector3 scale);

		Ogre::AxisAlignedBox& getWorldBoundingBox(Ogre::Vector3 position);

        const PhysicsMeshInfo& getPhysicsMeshInfo()
        {
                return mPhysicsMesh;
        }

		Ogre::MovableObject* getMovableObject(){return mDebuggGeom;}

	private:
		Ogre::ManualObject* debuggGeometry(Ogre::String name, Ogre::String material);
   
	private:

        Ogre::Entity* mPhysicEntity;
		Ogre::Vector3 boundingBoxDimension;
        PhysicsMeshInfo mPhysicsMesh;
		Ogre::MovableObject* mDebuggGeom;

    };

	typedef boost::shared_ptr<Geometry> GeometryPtr;
}

#endif //__PHYSICS_H__