#include "..\include\Particle\ParticleManager.h"

using namespace WyvernsAssault;

// BEGIN SINGLETON
template<> ParticleManager* Ogre::Singleton<ParticleManager>::ms_Singleton = 0;
ParticleManager* ParticleManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
ParticleManager& ParticleManager::getSingleton(void)
{  
    assert( ms_Singleton );  return ( *ms_Singleton );  
}
// END SINGLETON

ParticleManager::ParticleManager(SceneManager* sceneManager)
: mParticleSystem(0)
, mParticleSystemManager(0)
, mId(0)
, mTimer(0.0)
{
	this->mSceneManager = sceneManager;
}

ParticleManager::~ParticleManager()
{
	finalize();
}

/** Initialize the particle manager */
void ParticleManager::initialize()
{
	// Get particle universe particle system manager
	mParticleSystemManager = ParticleUniverse::ParticleSystemManager::getSingletonPtr();

	// Create blood lens particle system
	mParticleSystem = mParticleSystemManager->createParticleSystem("bloodLens", "WyvernsAssault/BloodLens", mSceneManager);
	mSceneManager->getRootSceneNode()->createChildSceneNode("bloodLensNode", Vector3::ZERO)->attachObject(mParticleSystem);
}


/** Finalize the particle manager */
void ParticleManager::finalize()
{
	mParticleSystemManager->destroyAllParticleSystems(mSceneManager);
	mParticleSystemManager->destroyAllAliasses();
	
	mSceneManager->destroySceneNode("bloodLensNode");

	mParticleSystem = NULL;
	mParticleSystemManager = NULL;
	mParticleSystemList.clear();
}

void ParticleManager::update(const float elapsedSeconds)
{
	for(int i = 0; i < mParticleSystemList.size(); i++ )
	{
		// Only check particle systems thar need to be repeated
		if( mParticleSystemList[i]->getRepeat() > 0 )
		{
			mParticleSystemList[i]->setTimer( mParticleSystemList[i]->getTimer() + elapsedSeconds );

			// If total time (mTimer) mod particle system repeat time < 1, need to be restarted
			if( mParticleSystemList[i]->getTimer() > mParticleSystemList[i]->getRepeat() )
			{
				mParticleSystemList[i]->start();
				mParticleSystemList[i]->newRepeat();
				mParticleSystemList[i]->setTimer(0.0);

				// It has camera special effect
				if( mParticleSystemList[i]->getParameters().subtype > -1 )
				{
					// Explosion
					if( mParticleSystemList[i]->getParameters().subtype == 0 )
					{
						SpecialEffectEventPtr evt = SpecialEffectEventPtr(new SpecialEffectEvent(SpecialEffectEvent::EffectType::Explosion, 0.5, 1.0));
						EVENTS_FIRE(evt);
					}
					// Explosion
					else if( mParticleSystemList[i]->getParameters().subtype == 1 )
					{
						SpecialEffectEventPtr evt = SpecialEffectEventPtr(new SpecialEffectEvent(SpecialEffectEvent::EffectType::Explosion, 1.0, 1.0));
						EVENTS_FIRE(evt);
					}
					// Quake
					else if( mParticleSystemList[i]->getParameters().subtype == 2 )
					{
						SpecialEffectEventPtr evt = SpecialEffectEventPtr(new SpecialEffectEvent(SpecialEffectEvent::EffectType::Quake, 2.0, 0.5));
						EVENTS_FIRE(evt);
					}
				}
			}
		}
	}
}

// Create particle system
ParticleUniverse::ParticleSystem* ParticleManager::create(String script)
{
	return mParticleSystemManager->createParticleSystem(this->createUniqueId(), script, mSceneManager);
}

// Create particle system, add to node and start it
void ParticleManager::add(SceneNode* node, WyvernsAssault::ParticleSystem::ParticleSystemParameters params, bool start)
{
	ParticleUniverse::ParticleSystem* ps = this->create(params.script);

	ParticleSystemPtr pSystem = ParticleSystemPtr(new ParticleSystem(node, ps, params, this->createUniqueId()));
	if( start )
		pSystem->start();

	mParticleSystemList.push_back(pSystem);
}

void ParticleManager::add(SceneNode* node, Vector3 position, WyvernsAssault::ParticleSystem::ParticleSystemParameters params, bool start)
{
	ParticleUniverse::ParticleSystem* ps = this->create(params.script);

	ParticleSystemPtr pSystem = ParticleSystemPtr(new ParticleSystem(node->createChildSceneNode(position), ps, params, this->createUniqueId()));
	if( start )
		pSystem->start();

	mParticleSystemList.push_back(pSystem);
}

void ParticleManager::add(SceneNode* node, String id, WyvernsAssault::ParticleSystem::ParticleSystemParameters params)
{
	ParticleUniverse::ParticleSystem* ps = mParticleSystemManager->createParticleSystem(id + "_particle", params.script, mSceneManager);

	ParticleSystemPtr pSystem = ParticleSystemPtr(new ParticleSystem(node, ps, params, id + "_particle"));
	pSystem->start();

	mParticleSystemList.push_back(pSystem);
}

// Remove particle system from node and stop it (only usable with non-unique ID particle systems)
bool ParticleManager::remove(String id)
{
	ParticleSystemPtr pSystem = this->getParticleSystem(id + "_particle");

	if( pSystem == NULL )
		return false;
	
	ParticleSystemListIterator it = find(mParticleSystemList.begin(), mParticleSystemList.end(), pSystem);
	
	if( it != mParticleSystemList.end() )
	{
		mParticleSystemList.erase(it);

		// Plugin special destroy
		mParticleSystemManager->getParticleSystem(id + "_particle")->stop();
		pSystem->getSceneNode()->detachObject(mParticleSystemManager->getParticleSystem(id + "_particle"));
		mParticleSystemManager->destroyParticleSystem(mParticleSystemManager->getParticleSystem(id + "_particle"), mSceneManager);

		return true;
	}

	return false;
}

ParticleSystemPtr ParticleManager::getParticleSystem(String id)
{
	for(int i = 0; i < mParticleSystemList.size(); i++ )
	{
		if( mParticleSystemList[i]->getId() == id )
			return mParticleSystemList[i];
	}

	ParticleSystemPtr pSystem;

	return pSystem;
}

WyvernsAssault::ParticleSystem::ParticleSystemParameters ParticleManager::defaultParameters(String script)
{
	WyvernsAssault::ParticleSystem::ParticleSystemParameters params;
	params.script = script;
	params.subtype = -1;
	params.repeatMax = -1;
	params.repeatMin = -1;

	return params;
}

// Blood on camera lens
void ParticleManager::bloodLens()
{
	Vector3 position = mSceneManager->getCamera(GAME_CAMERA)->getParentSceneNode()->getPosition();
	Vector3 direction = mSceneManager->getCamera(GAME_CAMERA)->getDirection();
	SceneNode* node = mSceneManager->getSceneNode("bloodLensNode");
	node->setPosition(position + direction * 170);	
	ParticleUniverse::ParticleSystem* particles = mParticleSystemManager->getParticleSystem("bloodLens");
	particles->startAndStopFade(5);
}

// Wizard magic bolt
void ParticleManager::magicBolt(SceneNode* node)
{
	String name = node->getName();
	ParticleUniverse::ParticleSystem* particles;
	if( mParticleSystemManager->getParticleSystem(name + "_magicBolt") == NULL)
	{
		particles = mParticleSystemManager->createParticleSystem( name + "_magicBolt", "WyvernsAssault/MagicBolt", mSceneManager);
		node->attachObject( particles );
	}
	else
	{
		particles = mParticleSystemManager->getParticleSystem( name + "_magicBolt");
	}	
	particles->start();
}

Ogre::String ParticleManager::createUniqueId()
{
	Ogre::StringStream countStrStr;
	countStrStr << mId;

	Ogre::String uniqueId = "Particle_" + countStrStr.str();

	mId++;

	return uniqueId;
}

// --------------
// Event handlers
// --------------
EVENTS_BEGIN_REGISTER_HANDLERS(ParticleManager)
	EVENTS_REGISTER_HANDLER(ParticleManager,PlayerHit)
	EVENTS_REGISTER_HANDLER(ParticleManager,EnemyHit)
	EVENTS_REGISTER_HANDLER(ParticleManager,EnemyKilled)
	EVENTS_REGISTER_HANDLER(ParticleManager,EnemyCustom)
	EVENTS_REGISTER_HANDLER(ParticleManager,ItemCreation)
	EVENTS_REGISTER_HANDLER(ParticleManager,ItemCatch)
	EVENTS_REGISTER_HANDLER(ParticleManager,ProjectileUpdate)
	EVENTS_REGISTER_HANDLER(ParticleManager,ProjectileHit)
	EVENTS_REGISTER_HANDLER(ParticleManager,ObjectHit)
	EVENTS_REGISTER_HANDLER(ParticleManager,ObjectKilled)
EVENTS_END_REGISTER_HANDLERS()

EVENTS_BEGIN_UNREGISTER_HANDLERS(ParticleManager)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,PlayerHit)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,EnemyHit)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,EnemyKilled)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,EnemyCustom)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,ItemCreation)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,ItemCatch)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,ProjectileUpdate)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,ProjectileHit)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,ObjectHit)
	EVENTS_UNREGISTER_HANDLER(ParticleManager,ObjectKilled)
EVENTS_END_REGISTER_HANDLERS()


EVENTS_DEFINE_HANDLER(ParticleManager, PlayerHit)
{
	PlayerPtr player = evt->getPlayer();

	SceneNode* sceneNode = player->_getSceneNode();

	this->add(sceneNode, this->defaultParameters("WyvernsAssault/Impact"));
}

EVENTS_DEFINE_HANDLER(ParticleManager, EnemyHit)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();	

	if( player->isSpecial() )
	{
		if( enemy->getEnemyType() == Enemy::EnemyTypes::Boss )
			this->add(enemy->getPhysicsNode(enemy->getPhysicPositionIndex()), this->defaultParameters("WyvernsAssault/FireHit"));
		else
			this->add(enemy->_getSceneNode(), this->defaultParameters("WyvernsAssault/FireHit"));
	}
	else
	{
		if( enemy->getEnemyType() == Enemy::EnemyTypes::BatteringRam )
			this->add(enemy->_getSceneNode(), this->defaultParameters("WyvernsAssault/Hit"));
		else if( enemy->getEnemyType() == Enemy::EnemyTypes::Boss )
			this->add(enemy->getPhysicsNode(enemy->getPhysicPositionIndex()), this->defaultParameters("WyvernsAssault/Hit"));
		else
		{
			this->add(enemy->_getSceneNode(), this->defaultParameters("WyvernsAssault/BloodHit"));
			this->add(enemy->_getSceneNode(), this->defaultParameters("WyvernsAssault/Hit"));	
		}
	}
}

EVENTS_DEFINE_HANDLER(ParticleManager, EnemyKilled)
{
	EnemyPtr enemy = evt->getEnemy();
	PlayerPtr player = evt->getPlayer();	

	if( enemy->getEnemyType() == Enemy::EnemyTypes::Boss )
	{			
		this->add(enemy->_getSceneNode(), this->defaultParameters("WyvernsAssault/BossExplosion"));
	}
	else if( !enemy->isBurning() )
	{		
		if( enemy->getEnemyType() == Enemy::EnemyTypes::Chicken )
			this->add(enemy->_getSceneNode(), this->defaultParameters("WyvernsAssault/ChickenKill"));

		else if( enemy->getEnemyType() == Enemy::EnemyTypes::Cow )
			this->add(enemy->_getSceneNode(), this->defaultParameters("WyvernsAssault/CowKill"));

		else if( enemy->isFlying() )
		{
			this->add(enemy->_getSceneNode(), this->defaultParameters("WyvernsAssault/BloodHit"));
			this->bloodLens();
		}
		else
		{
			if( enemy->getEnemyType() == Enemy::EnemyTypes::BatteringRam )
				this->add(enemy->_getSceneNode(), this->defaultParameters("WyvernsAssault/Hit"));
			else
				this->add(enemy->_getSceneNode(), this->defaultParameters("WyvernsAssault/BloodKill"));
		}
	}
}

EVENTS_DEFINE_HANDLER(ParticleManager, EnemyCustom)
{
	EnemyPtr enemy = evt->getEnemy();

	if( enemy->isBurning() && enemy->getEnemyType() != Enemy::EnemyTypes::Boss )
	{
		if( enemy->getEnemyType() == Enemy::EnemyTypes::BatteringRam )
			this->add(enemy->_getSceneNode(), this->defaultParameters("WyvernsAssault/FireKillObject"));
		else
			this->add(enemy->_getSceneNode(), this->defaultParameters("WyvernsAssault/FireKill"));
	}
}

EVENTS_DEFINE_HANDLER(ParticleManager, ItemCreation)
{
	ItemPtr item = evt->getItem();

	this->add(item->_getSceneNode(), item->getName() + "_glow", this->defaultParameters("WyvernsAssault/ItemGlow"));

	if ( item->getType() == Item::ItemTypes::Life )
		this->add(item->_getSceneNode(), item->getName(), this->defaultParameters("WyvernsAssault/Heart"));
	else if ( item->getType() == Item::ItemTypes::Drunk )
		this->add(item->_getSceneNode(), item->getName(), this->defaultParameters("WyvernsAssault/Skull"));
	else if ( item->getType() == Item::ItemTypes::Points )
		this->add(item->_getSceneNode(), item->getName(), this->defaultParameters("WyvernsAssault/Money"));
}

EVENTS_DEFINE_HANDLER(ParticleManager, ItemCatch)
{
	ItemPtr item = evt->getItem();

	SceneNode* sceneNode = item->_getSceneNode();

	this->remove(item->getName() + "_glow");
	this->remove(item->getName());
	this->add(sceneNode, this->defaultParameters("WyvernsAssault/GlowShort"));
}

EVENTS_DEFINE_HANDLER(ParticleManager, ProjectileUpdate)
{
	ProjectilePtr projectile = evt->getProjectile();

	// Add particles to projectile
	if( evt->getType() == Enemy::EnemyTypes::Wizard )
		this->add(projectile->_getSceneNode(), projectile->getName(), this->defaultParameters("WyvernsAssault/MagicBolt"));
}

EVENTS_DEFINE_HANDLER(ParticleManager, ProjectileHit)
{
	PlayerPtr player = evt->getPlayer();

	SceneNode* sceneNode = player->_getSceneNode();

	this->add(sceneNode, this->defaultParameters("WyvernsAssault/Impact"));
}

EVENTS_DEFINE_HANDLER(ParticleManager, ObjectHit)
{
	ObjectPtr object = evt->getObject();
	PlayerPtr player = evt->getPlayer();	

	if( player->isSpecial() )
		this->add(object->_getSceneNode(), this->defaultParameters("WyvernsAssault/FireHitObject"));
	else
		this->add(object->_getSceneNode(), this->defaultParameters("WyvernsAssault/Hit"));
}

EVENTS_DEFINE_HANDLER(ParticleManager, ObjectKilled)
{
	ObjectPtr object = evt->getObject();
	PlayerPtr player = evt->getPlayer();	

	if( player->isSpecial() )		
		this->add(object->_getSceneNode(), this->defaultParameters("WyvernsAssault/FireKillObject"));
	else
		this->add(object->_getSceneNode(), this->defaultParameters("WyvernsAssault/Hit"));
}

// --------------------------------
// Lua Particle Lib
// --------------------------------
LUA_BEGIN_BINDING(ParticleManager, particlelib)
LUA_BIND(ParticleManager, createSystem)
LUA_BIND(ParticleManager, removeSystem)
LUA_END_BINDING()

//
// Load lua scripts that will be used by this manager
//
LUA_BEGIN_LOAD_SCRIPTS(ParticleManager)
// 
// TODO : Load scripts if needed
//
LUA_END_LOAD_SCRIPTS()

LUA_DEFINE_FUNCTION(ParticleManager, createSystem)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}

LUA_DEFINE_FUNCTION(ParticleManager, removeSystem)
{
	/* get number of arguments */
	int n = lua_gettop(L);

	/* return the number of results */
	return 1;
}
