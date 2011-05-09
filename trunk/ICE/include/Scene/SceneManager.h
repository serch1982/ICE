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
#ifndef __SCENARIO_MANAGER_H_
#define __SCENARIO_MANAGER_H_

#define SCENARIO_NODE_NAME "ScenarioNode"

#include <Ogre.h>
#include <OgreSingleton.h>

#include <boost/enable_shared_from_this.hpp>

#include "..\Debug\Debug.h"

#include "..\Utils\Utils.h"
#include "..\Events\EventsInterface.h"
#include "..\Lua\LuaInterface.h"

#include "Object.h"

using namespace Ogre;

namespace WyvernsAssault 
{
	typedef std::vector<ObjectPtr> ObjectList;
	typedef std::vector<ObjectPtr>::iterator ObjectListIterator;

	typedef std::map<int, ObjectList> ObjectMapList;
	typedef std::map<int, ObjectList>::iterator ObjectMapListIterator;

	/**
	Class used to manage all the scenarios
	*/
	class ScenarioManager	: public Ogre::Singleton<ScenarioManager>
							, public boost::enable_shared_from_this<ScenarioManager>
							, public EventsInterface
							, public LuaInterface
	{
	public:
		ScenarioManager(SceneManager* sceneManager);
		~ScenarioManager();
		static ScenarioManager& getSingleton(void);
		static ScenarioManager* getSingletonPtr(void);

		void initialize();
		void load(Ogre::String file);

		void update(const float elpasedSeconds);

		ObjectPtr createObject(WyvernsAssault::ObjectTypes type, Ogre::String name, Ogre::String meshFile);
		ObjectPtr createObject(WyvernsAssault::ObjectTypes type, Ogre::String name, Ogre::Entity* entity, Ogre::SceneNode* sceneNode, int gameArea, Ogre::Vector3 physicBox, String dieMesh, String dieAnimation, bool dieMaterial, int life);

		int getCount();
		int getCount(int gameArea);

		ObjectPtr getObject(int index);
		ObjectPtr getObject(int index, int gameArea);
		ObjectPtr getObject(Ogre::String name);
		ObjectPtr getObject(Ogre::String name, int gameArea);

		bool removeObject(Ogre::String name);

		// Enable Debug Stuff
		void setDebugEnabled(bool isDebugEnabled);
		bool getDebugEnabled(){return mIsDebugEnabled;};

		Ogre::SceneNode* _getSceneNode() const { return mScenarioNode; }

	public:
		// ----------------
		// Events interface
		// ----------------
		EVENTS_INTERFACE()

		EVENTS_HANDLER(Collision)
		EVENTS_HANDLER(ObjectHit)
		EVENTS_HANDLER(ObjectKilled)
		EVENTS_HANDLER(ObjectRemove)
		EVENTS_HANDLER(ObjectCustom)
		EVENTS_HANDLER(GameAreaChanged)

	private:
		Ogre::SceneManager*		mSceneManager;
		Ogre::SceneNode*		mScenarioNode;

		// Where items are stored and sorted by game area
		ObjectMapList			mObjectMapList;
		int						mCurrentGameArea;

		bool					mIsDebugEnabled;

	private:
		bool mInitialized;

		// --------------------------------
		// BEGIN Lua Interface Declarations
		// --------------------------------
	public:
		LUA_INTERFACE();

		//Scenario Lib (exported to Lua)
		LUA_LIBRARY("Scenario",scenariolib);

		LUA_FUNCTION(createObject);
		LUA_FUNCTION(removeObject);
		// ------------------------------
		// END Lua Interface Declarations
		// ------------------------------
	};

	typedef boost::shared_ptr<ScenarioManager> ScenarioManagerPtr;
}

#endif //__SCENARIO_MANAGER_H_