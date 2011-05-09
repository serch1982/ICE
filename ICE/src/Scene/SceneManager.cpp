#include "..\include\Scenario\ScenarioManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> ScenarioManager* Ogre::Singleton<ScenarioManager>::ms_Singleton = 0;
ScenarioManager* ScenarioManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
ScenarioManager& ScenarioManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

ScenarioManager::ScenarioManager(SceneManager* sceneManager)
: mInitialized(false)
, mSceneManager(0)
, mScenarioNode(0)
, mIsDebugEnabled(false)
, mCurrentGameArea(-1)
{
	mSceneManager = sceneManager;
}

ScenarioManager::~ScenarioManager()
{
	if(mInitialized)
	{
		//
		// Destroy all the objects (destructors will take care of deleting them)
		//
		mObjectMapList.clear();

		Utils::Destroy(mSceneManager, SCENARIO_NODE_NAME);
		mScenarioNode = NULL;

		mInitialized = false;
	}
}

void ScenarioManager::initialize()
{
	//
	// Create the base scenario node...
	//
	mScenarioNode = mSceneManager->getRootSceneNode()->createChildSceneNode(SCENARIO_NODE_NAME, Vector3::ZERO);

	// Now it is initialized!
	mInitialized = true;
}

void ScenarioManager::load(Ogre::String file)
{
	assert(mInitialized);
}

ObjectPtr ScenarioManager::createObject(WyvernsAssault::ObjectTypes type, Ogre::String name, Ogre::String meshFile)
{
	Ogre::Entity* pEntity = mSceneManager->createEntity(name, meshFile);

	Ogre::SceneNode* pSceneNode = mSceneManager->getRootSceneNode()->createChildSceneNode("Object_"+name+"_Node");

	return createObject(type, name, pEntity, pSceneNode, mCurrentGameArea, Ogre::Vector3::ZERO, "", "", false, 0);
}

ObjectPtr ScenarioManager::createObject(WyvernsAssault::ObjectTypes type, Ogre::String name, Ogre::Entity* entity, Ogre::SceneNode* sceneNode, int gameArea, Ogre::Vector3 physicBox, String dieMesh, String dieAnimation, bool dieMaterial, int life)
{
	ObjectPtr object = ObjectPtr(new Object(name, type, physicBox, life));

	object->initializeEntity(entity, sceneNode, mSceneManager);

	sceneNode->attachObject(entity);

	// Die mesh and animation
	if(type == WyvernsAssault::ObjectTypes::DynamicObject && dieMesh != "") 
	{
		Ogre::Entity* objDieMesh = NULL;
		Ogre::AnimationState* objDieAnimation = NULL;
		objDieMesh = mSceneManager->createEntity(name + "Die", dieMesh);
		if(!dieMaterial)
			objDieMesh->setMaterialName(entity->getSubEntity(0)->getMaterialName()); 
		sceneNode->attachObject(objDieMesh);			
		object->setDieMesh(objDieMesh);
		if(dieAnimation != "")
			objDieAnimation = objDieMesh->getAnimationState(dieAnimation);
		object->setDieAnimation(objDieAnimation);
	}

	mObjectMapList[gameArea].push_back(object);

	return object;
}

int ScenarioManager::getCount()
{
	return mObjectMapList[mCurrentGameArea].size();
}

int ScenarioManager::getCount(int gameArea)
{
	return mObjectMapList[gameArea].size();
}

ObjectPtr ScenarioManager::getObject(int index)
{
	return mObjectMapList[mCurrentGameArea][index];
}

ObjectPtr ScenarioManager::getObject(int index, int gameArea)
{
	return mObjectMapList[gameArea][index];
}


ObjectPtr ScenarioManager::getObject(Ogre::String name)
{
	// Search item in current game area enemies list
	for( int i = 0; i < mObjectMapList[mCurrentGameArea].size(); i++ )
	{
		ObjectPtr object = mObjectMapList[mCurrentGameArea][i];

		if( object->getName() == name )
			return mObjectMapList[mCurrentGameArea][i];
	}

	// Search in others game areas lists
	for( ObjectMapListIterator it = mObjectMapList.begin(); it != mObjectMapList.end(); ++it )
	{
		ObjectList list = it->second;

		for( int i = 0; i < list.size(); i++ )
		{
			ObjectPtr object = list[i];

			if( object->getName() == name )
				return list[i];
		}
	}
}

ObjectPtr ScenarioManager::getObject(Ogre::String name, int gameArea)
{
	for( int i = 0; i < mObjectMapList[gameArea].size(); i++ )
	{
		ObjectPtr object = mObjectMapList[gameArea][i];

		if( object->getName() == name )
			return mObjectMapList[gameArea][i];
	}
}

bool ScenarioManager::removeObject(Ogre::String name)
{
	ObjectPtr objectToErase = getObject(name);
	
	ObjectListIterator it = find(mObjectMapList[mCurrentGameArea].begin(), mObjectMapList[mCurrentGameArea].end(), objectToErase);
	
	if( it != mObjectMapList[mCurrentGameArea].end() )
	{
		mObjectMapList[mCurrentGameArea].erase(it);
		return true;
	}

	return false;
}

void ScenarioManager::setDebugEnabled(bool isDebugEnabled)
{
	if(mIsDebugEnabled != isDebugEnabled)
	{
		mIsDebugEnabled = isDebugEnabled;

	}
}

void ScenarioManager::update(const float elapsedSeconds)
{
	for(int i = 0; i < mObjectMapList[mCurrentGameArea].size() ; i++)
	{
		ObjectPtr obj =  mObjectMapList[mCurrentGameArea][i];
		obj->updateEntity(elapsedSeconds); // this updates animations too!		

		// Physic debugg control
		obj->setDebugEnabled(mIsDebugEnabled);
	}
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(ScenarioManager)
	EVENTS_REGISTER_HANDLER(ScenarioManager, Collision)
	EVENTS_REGISTER_HANDLER(ScenarioManager, ObjectHit)
	EVENTS_REGISTER_HANDLER(ScenarioManager, ObjectKilled)
	EVENTS_REGISTER_HANDLER(ScenarioManager, ObjectRemove)
	EVENTS_REGISTER_HANDLER(ScenarioManager, ObjectCustom)
	EVENTS_REGISTER_HANDLER(ScenarioManager, GameAreaChanged)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(ScenarioManager)
	EVENTS_UNREGISTER_HANDLER(ScenarioManager, Collision)
	EVENTS_UNREGISTER_HANDLER(ScenarioManager, ObjectHit)
	EVENTS_UNREGISTER_HANDLER(ScenarioManager, ObjectKilled)
	EVENTS_UNREGISTER_HANDLER(ScenarioManager, ObjectRemove)
	EVENTS_UNREGISTER_HANDLER(ScenarioManager, ObjectCustom)
	EVENTS_UNREGISTER_HANDLER(ScenarioManager, GameAreaChanged)
EVENTS_END_UNREGISTER_HANDLERS()

EVENTS_DEFINE_HANDLER(ScenarioManager, Collision)
{
// TODO
}

EVENTS_DEFINE_HANDLER(ScenarioManager, ObjectHit)
{
	Debug::Out("ScenarioManager : handleObjectHitEvent");

	ObjectPtr obj = evt->getObject();
	PlayerPtr player = evt->getPlayer();

	obj->hit(player->getHitDamage());

	if(obj->isDying())
	{
		ObjectKilledEventPtr oKill = ObjectKilledEventPtr(new ObjectKilledEvent(obj, player));
 		EVENTS_FIRE(oKill);
	}
}

EVENTS_DEFINE_HANDLER(ScenarioManager, ObjectKilled)
{
	Debug::Out("ScenarioManager : handleObjectKilledEvent");

	ObjectPtr obj = evt->getObject();
	PlayerPtr player = evt->getPlayer();

	if( player->isSpecial() )
	{	
		obj->setBurning(true);
		obj->setMaterialName("Skin/Black");
		ObjectCustomEventPtr oCustom = ObjectCustomEventPtr(new ObjectCustomEvent(obj));
		EVENTS_FIRE_AFTER(oCustom, 1.0f);
	}
	else if( obj->hasDieMesh() )
	{
		obj->dieSwitch();
	}
}

EVENTS_DEFINE_HANDLER(ScenarioManager, ObjectRemove)
{
	Debug::Out("ScenarioManager : handleObjectRemoveEvent");

	ObjectPtr o = evt->getObject();
	removeObject(o->getName());
}

EVENTS_DEFINE_HANDLER(ScenarioManager, ObjectCustom)
{
	Debug::Out("ScenarioManager : handleObjectCustomEvent");

	ObjectPtr object = evt->getObject();

	if( object->isBurning())
		object->dieSwitch(true);
}

EVENTS_DEFINE_HANDLER(ScenarioManager, GameAreaChanged)
{
	Debug::Out("ScenarioManager : handleGameAreaChangedEvent");

	mCurrentGameArea = evt->getActualArea();
}

// --------------------------------
// Lua Scenario Lib
// --------------------------------
LUA_BEGIN_BINDING(ScenarioManager, scenariolib)
LUA_BIND(ScenarioManager, createObject)
LUA_BIND(ScenarioManager, removeObject)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(ScenarioManager)
// 
// TODO : Load scripts if needed
//
LUA_END_LOAD_SCRIPTS()

LUA_DEFINE_FUNCTION(ScenarioManager, createObject)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(ScenarioManager, removeObject)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}