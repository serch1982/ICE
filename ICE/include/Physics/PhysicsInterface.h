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
#ifndef __PHYSICS_INTERFACE_H__
#define __PHYSICS_INTERFACE_H__

#include <Ogre.h>

#include "Physics.h"

using namespace Ogre;

namespace WyvernsAssault
{
	typedef std::vector<GeometryPtr> GeometryList;
	typedef std::vector<GeometryPtr>::iterator GeometryListIterator;

	enum PhysicsTypes
	{
		Ground,
		Border,
		Static,
		Dynamic,
		Fake
	};

	enum PhysicBoxType
	{
		body = 0,
		attack = 1,
		specialAttack = 2
	};

	/**
	Interface implemented by objects with physics
	*/
	class PhysicsInterface
	{

	public:
		
		virtual void initializePhysics(Ogre::String name, Ogre::Vector3 boxDimension, Ogre::String material)
		{
			mGeometryList.push_back( GeometryPtr(new Geometry(boxDimension, name, material)) );
		}

		virtual void initializeBossPhysics(Ogre::String name, Ogre::Vector3 boxDimension, Ogre::String material)
		{
			for(int i = 0; i < 8; i ++)
			{
				// One box for every leg
				char boxName[20];
				sprintf(boxName, "%s_%d", name.c_str(), i);
				GeometryPtr geom = GeometryPtr(new Geometry(boxDimension, boxName, material));
				mGeometryList.push_back( geom );
			}
		}

		virtual void addAttackGeometry(Ogre::String name, Ogre::Vector3 boxDimension, Ogre::String material)
		{
			mGeometryAttack = GeometryPtr(new Geometry(boxDimension, "attack"+name, material));
		}

		virtual void finalizePhysics(){return;} 
		virtual void updatePhysics(const float elapsedSeconds){};

		Ogre::Real getSpeed(){return mSpeed;}
		void setSpeed(Ogre::Real speed){mSpeed = speed;}

		virtual GeometryPtr getGeometry(PhysicsInterface::PhysicBoxType type)
		{
			if( type == PhysicBoxType::body )
				return mGeometryList[0];
			else if( type == PhysicBoxType::attack )
				return mGeometryAttack;
			return mGeometryList[0];
		}

		virtual GeometryPtr getBossGeometry(int index){ return mGeometryList[index]; }

	private:
		Ogre::Real		mSpeed;

		// List of physics geometry
		GeometryList	mGeometryList;
		
		// Attack geometry
		GeometryPtr		mGeometryAttack;

	};

	typedef boost::shared_ptr<PhysicsInterface> PhysicsInterfacePtr;
}

#endif // __PHYSICS_INTERFACE_H__