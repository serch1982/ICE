#include "Logic\iceLogicLua.h"
#include "iceGame.h" 

// Some helpful macros for defining constants  (sort of) in Lua. Similar to this code: 
// object g = globals(L);
// object table = g["class"];
// table["constant"] = class::constant; 
#define LUA_CONST_START( class ) { luabind::object g = luabind::globals(L); luabind::object table = g[#class];
#define LUA_CONST( class, name ) table[#name] = class::name
#define LUA_CONST_END }

iceLogicLua* iceLogicLua::pinstance = 0;

iceLogicLua* iceLogicLua::getInstance(){
	if (pinstance == 0)
		pinstance = new iceLogicLua;
	return pinstance;
}

iceLogicLua::iceLogicLua(void){
	_log = iceGame::getGameLog();
	L = lua_open();	
	luaL_openlibs(L);
	luabind::open(L);
}

iceLogicLua::~iceLogicLua(void){
	if(L)
		lua_close(L);
}


int iceLogicLua::RunFile( const char *fname )
{
	int ret;
	if( ret = luaL_dofile(L,fname) ){
	//if( ret = luaL_loadfile(L, fname) || lua_pcall( L, 0 ,0 ,0 )){
		errorHandler();
	}else{
		_log->logMessage("The lua file has been loaded");
		bindLuaObjects();
	}

	return ret;
}

int iceLogicLua::RunSource( const char *source )
{
	int ret;
	if( ret = luaL_dostring(L, source) ){
		errorHandler(); 
	}else{
		bindLuaObjects();
	}

	return ret;
}

int iceLogicLua::errorHandler()
{
	luabind::object error_msg(luabind::from_stack(L, -1));
	std::stringstream ss; 
		
	ss << error_msg << std::endl;

	_log->logMessage("Lua exception:"  + std::string(ss.str()) );
	return 0;
}

void iceLogicLua::ForceGarbageCollect()
{
	lua_gc(L, LUA_GCCOLLECT, 0);
}

bool iceLogicLua::FuncExist(const char *name )
{
	luabind::object g = luabind::globals(L);
	luabind::object func = g[name];

	if( func )
	{
		if( luabind::type(func) == LUA_TFUNCTION ){
			return true;
		}else{
			_log->logMessage("the Function doesnt exists:"  + std::string(name) );
		}
	}

	return false;
}



void iceLogicLua::bindLuaObjects(){
	//using namespace Ogre;
	/*luabind::module(L)
	[
		luabind::class_<Vector3>( "Vector3" )
		.def(tostring(luabind::self))
		.def_readwrite( "x", &Vector3::x )
		.def_readwrite( "y", &Vector3::y )
		.def_readwrite( "z", &Vector3::z )
		.def(luabind::constructor<>())
        .def(luabind::constructor<Vector3&>())
		.def(luabind::constructor<Real, Real, Real>())
		.def("absDotProduct", &Vector3::absDotProduct)
		.def("crossProduct", &Vector3::crossProduct )
		.def("directionEquals", &Vector3::directionEquals )
		.def("distance", &Vector3::distance )
		.def("dotProduct", &Vector3::dotProduct )
		.def("getRotationTo", &Vector3::getRotationTo )
		.def("isZeroLength", &Vector3::isZeroLength )
		.def("length", &Vector3::length )
		.def("makeCeil", &Vector3::makeCeil )
		.def("makeFloor", &Vector3::makeFloor )
		.def("midPoint", &Vector3::midPoint )
		.def("normalise", &Vector3::normalise )
		.def("nornaliseCopy", &Vector3::normalisedCopy )
		.def("perpendicular", &Vector3::perpendicular )
		.def("positionCloses", &Vector3::positionCloses )
		.def("positionEquals", &Vector3::positionEquals )
		.def("randomDeviant", &Vector3::randomDeviant )
		.def("reflect", &Vector3::reflect )
		.def("squaredDistance", &Vector3::squaredDistance )
		.def("squaredLength", &Vector3::squaredLength )

		// Operators

		.def( luabind::self + luabind::other<Vector3>() )
		.def( luabind::self - luabind::other<Vector3>() )
		.def( luabind::self * luabind::other<Vector3>() )
		.def( luabind::self * Real() )
	];*/

	/*LUA_CONST_START( Vector3 )
		LUA_CONST( Vector3, ZERO);
		LUA_CONST( Vector3, UNIT_X );
		LUA_CONST( Vector3, UNIT_X);
		LUA_CONST( Vector3, UNIT_Y);
		LUA_CONST( Vector3, UNIT_Z);
		LUA_CONST( Vector3, NEGATIVE_UNIT_X);
		LUA_CONST( Vector3, NEGATIVE_UNIT_Y);
		LUA_CONST( Vector3, NEGATIVE_UNIT_Z);
		LUA_CONST( Vector3, UNIT_SCALE);
	LUA_CONST_END;*/


	/*luabind::module(L)
	[
		luabind::class_<Real>( "Real" )
		.def(tostring(luabind::self))
		.def(luabind::constructor<>())
	];*/

	//enemy class definition
	luabind::module(L)
	[
		luabind::class_<iceEnemy>("enemy")
			.enum_("ENEMYSTATE")
			[
				luabind::value("STOPPED",0),
				luabind::value("FOLLOWING_TRAJECTORY",1),
				luabind::value("ATTACK",2),
				luabind::value("DYING",3),
				luabind::value("DEAD",4),
				luabind::value("INACTIVE",5)
			]
		    .def("getState", &iceEnemy::getState )
		    .def("setState", &iceEnemy::setState )
			.def("isActive", &iceEnemy::isActive )
			.def("isAlive", &iceEnemy::isAlive )
			.def("isAnimDyingEnded", &iceEnemy::isAnimDyingEnded )
			//.def("checkActivationTime", (bool( iceEnemy::*)(Ogre::Real)) &iceEnemy::checkActivationTime)
			//.def("isVisiblePlayerCam", (bool( iceEnemy::*)(void)) &iceEnemy::isVisiblePlayerCam)
			//.def("isVisibleWideCam", (bool( iceEnemy::*)(void)) &iceEnemy::isVisibleWideCam)
			//.def("rangeAttack", (float( iceEnemy::*)(void)) &iceEnemy::rangeAttack)
	];
}

//call the method of lua with the enemy logic and change his ENEMYSTATE 
void iceLogicLua::getEnemyLogicState(iceEnemy *enemy, Ogre::Real p_timeSinceLastFrame){
	/*try{
		unsigned int ret = -1;
		if( enemy->getType() == enemy->KAMIKAZE ){
			if(FuncExist("KamikazeLogic")){
				luabind::call_function<void>(L, "KamikazeLogic", enemy);
					std::stringstream ss; 
					ss << enemy->getState();
				_log->logMessage( ss.str() );
				//ret = luabind::call_function<int>(L, "KamikazeLogic", enemy);
			}else
				_log->logMessage("IA: function KamikazeLogic doesn't exist" );
			if( ret == enemy->INACTIVE )
				enemy->setAnimDyingEnded(0);
		}
	}catch(const luabind::error& err)
		{
			std::string errString = "LUA Function call failed: ";
			errString.append(err.what()).append(" - ");
			errString.append(lua_tostring(err.state(),-1));
			_log->logMessage(errString);
		}
	/*else if( enemy->getType() == enemy->MAGMATON ){
		if(FuncExist("MagmatonLogic")){
			ret = luabind::call_function<int>(L, "MagmatonLogic", enemy->getState());
		}else
			_log->logMessage("IA: function MagmatonLogic doesn't exist" );

	}else if( enemy->getType() == enemy->MINIMAGMATON ){
		if(FuncExist("MinimagmatonLogic")){
			ret = luabind::call_function<int>(L, "MinimagmatonLogic", enemy->getState());
		}else
			_log->logMessage("IA: function MinimagmatonLogic doesn't exist" );

	}else if( enemy->getType() == enemy->SMART ){
		if(FuncExist("SmartLogic")){
			ret = luabind::call_function<int>(L, "SmartLogic", enemy->getState());
		}else
			_log->logMessage("IA: function SmartLogic doesn't exist" );

	}else if( enemy->getType() == enemy->VOLCANO ){
		if(FuncExist("VolcanoLogic")){
			ret = luabind::call_function<int>(L, "VolcanoLogic", enemy->getState());
		}else
			_log->logMessage("IA: function VolcanoLogic doesn't exist" );
	}*/
//	enemy->setState( ret );
}