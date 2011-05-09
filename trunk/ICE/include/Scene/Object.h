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
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include <Ogre.h>

#include "..\Physics\PhysicsInterface.h"
#include "..\Entity\EntityInterface.h"
#include "..\Entity\OBBoxRenderable.h"

namespace WyvernsAssault
{
	/** List of object types */
	enum ObjectTypes
	{
		Default,
		DynamicObject
	};

	/**
	Class used to manage all the enemies
	*/
	class Object : public PhysicsInterface, public EntityInterface
	{
	public:
		Object(Ogre::String name, ObjectTypes type, Ogre::Vector3 physicBox, int life);
		~Object();

		void _attachTo(Ogre::SceneNode* sceneNode);
		void _detach();

		ObjectTypes getObjectType(){return mType;}

		void updateEntity(const float elapsedSeconds);

		// Enable Debug Stuff
		void setDebugEnabled(bool isDebugEnabled);
		bool getDebugEnabled(){return mIsDebugEnabled;}

		void hit(float damage);
		bool isHurt();
		bool isDying();
		bool isDead(){ return dead; }
		bool isBurning(){ return burning; }
		void setBurning(bool status){ burning = status; }

		Real getPoints(){ return points; }

		void setDieMaterialName(Ogre::String material){ mDieMesh->setMaterialName(material); }
		bool hasDieAnimation(){ return mDieAnimation != NULL; }
		bool hasDieMesh(){ return mDieMesh != NULL; }
		void setDieMesh(Ogre::Entity* entity);
		void setDieAnimation(Ogre::AnimationState* dieAnimation);
		void dieSwitch(bool burning = false);

		//
		// Entity Interface
		//
		void initializeEntity(Ogre::Entity* mesh, Ogre::SceneNode* sceneNode, Ogre::SceneManager* sceneManager);
		void finalizeEntity();

	private:
		ObjectTypes mType;

		Real mMaxLife;
		Real life;
		Real points;

		bool burning;
		bool dead;

		Vector3 mPhysicSize;

		Entity*				mDieMesh;
		AnimationState*		mDieAnimation;

		OBBoxRenderable* mOBBoxRenderable;
		bool mIsDebugEnabled;

	private:
		void chase();

	public:
		static ObjectTypes StringToType(Ogre::String typeStr);
	};

	typedef boost::shared_ptr<Object> ObjectPtr;
}

#endif // __OBJECT_H__