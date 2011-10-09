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
	isload = false;
	_log = iceGame::getGameLog();
	L = lua_open();	
	luaL_openlibs(L);
	luabind::open(L);
}

iceLogicLua::~iceLogicLua(void){
	if(L) lua_close(L);
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

int iceLogicLua::runAllFiles()
{
	if( isload) return 0;
	int error;
	std::vector<Ogre::String> luaFiles;

	luaFiles.push_back("./media/scripts/lua/mini.lua");
	luaFiles.push_back("./media/scripts/lua/smart.lua");
	luaFiles.push_back("./media/scripts/lua/kamikaze.lua");
	luaFiles.push_back("./media/scripts/lua/volcano.lua");
	luaFiles.push_back("./media/scripts/lua/StartLevel1.lua");
	luaFiles.push_back("./media/scripts/lua/EndLevel2.lua");

	for(unsigned int i=0;i<luaFiles.size();i++)
	{
		if( error = luaL_dofile(L,luaFiles[i].c_str()) )
		{
			errorHandler();
			break;
		}
		else
		{
			_log->logMessage("The lua file " + luaFiles[i] + " has been loaded");
		}
	}

	if(!error)
	{
		isload = true;
		bindLuaObjects();
	}
	return error;
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
	//enemy class definition
	luabind::module(L)
	[
		luabind::class_<iceEnemy>("enemy")
			.enum_("ENEMYSTATE")
			[
				luabind::value("STOPPED",0),
				luabind::value("FOLLOWING_TRAJECTORY",1),
				luabind::value("ATTACK",2),
				luabind::value("START_DYING",3),
				luabind::value("DYING",4),
				luabind::value("DEAD",5),
				luabind::value("INACTIVE",6)
			]
		    .def("getState", &iceEnemy::getState )
		    .def("setState", &iceEnemy::setState )
			.def("isActive", &iceEnemy::isActive )
			.def("isAlive", &iceEnemy::isAlive )
			.def("changeDirection", &iceEnemy::changeDirection )
			.def("isAnimDyingEnded", &iceEnemy::isAnimDyingEnded )
			.def("setAnimDyingEnded", &iceEnemy::setAnimDyingEnded )
			//.def("checkActivationTime", (bool( iceEnemy::*)(Ogre::Real)) &iceEnemy::checkActivationTime)
			.def("isVisiblePlayerCam", (bool( iceEnemy::*)(void)) &iceEnemy::isVisiblePlayerCam)
			.def("isVisibleWideCam", (bool( iceEnemy::*)(void)) &iceEnemy::isVisibleWideCam)
			.def("rangeAttack", (float( iceEnemy::*)(void)) &iceEnemy::rangeAttack),

			luabind::class_<iceCutScene>("cutscene")
			.enum_("CUTSCENEENTITYTYPE")
			[
				luabind::value("SHIP",0),
				luabind::value("MINI",1),
				luabind::value("SMART",2),
				luabind::value("KAMIKACE",3),
				luabind::value("VOLCANO",4),
				luabind::value("MAGMATON",5),
				luabind::value("PIGSHEEP",6),
				luabind::value("DUMMY",7)
			]
			.def("setActivationTime", &iceCutScene::setActivationTime )
		    .def("addEntity", &iceCutScene::addEntity )
		    .def("initializeCameraEntity", &iceCutScene::initializeCameraEntity )
			.def("getStep", &iceCutScene::getStep )
			.def("setStep", &iceCutScene::setStep )
			.def("isEntityTrajectoryEnded", &iceCutScene::isEntityTrajectoryEnded )
			.def("nextStep", &iceCutScene::nextStep )
			.def("stop", &iceCutScene::stop )
			.def("startEntityAnimation", &iceCutScene::startEntityAnimation )
			.def("stopEntityAnimations", &iceCutScene::stopEntityAnimations )
			.def("hasEntityAnimationEnded", &iceCutScene::hasEntityAnimationEnded )
			.def("playSound", &iceCutScene::playSound )
			.def("stopSound", &iceCutScene::stopSound )
			.def("stopAllSounds", &iceCutScene::stopAllSounds )
			.def("getCurrentTime", &iceCutScene::getCurrentTime )
			.def("setEntityLookAt", &iceCutScene::setEntityLookAt )
			.def("startEntity", &iceCutScene::startEntity )
			.def("stopEntity", &iceCutScene::stopEntity )
			.def("showBlur", &iceCutScene::showBlur )
			.def("hideBlur", &iceCutScene::hideBlur )
			.def("showToBeContinued", &iceCutScene::showToBeContinued )
			.def("hideToBeContinued", &iceCutScene::hideToBeContinued )
	];
}

//call the method of lua with the enemy logic and change his ENEMYSTATE 
void iceLogicLua::getEnemyLogicState(iceEnemy *enemy, Ogre::Real p_timeSinceLastFrame){
	// try catch for luabind
	try{
		unsigned int ret = -1;
		// we need the name of the function
		std::string funcName;
		funcName = enemy->getFunctionStr();
		//Check if the function exists
		if( FuncExist( funcName.c_str() )){
			//call Lua Function
			luabind::call_function<void>(L, funcName.c_str(), enemy );
			// log
			//std::stringstream ss;
			//ss << enemy->getState();
			//_log->logMessage( ss.str() );
		}else{
			std::stringstream ss;
			ss << "IA: function " << funcName << " does NOT exist";
			_log->logMessage( ss.str() );
		}
	}catch(const luabind::error& err){
			//Catch and log the message
			std::string errString = "LUA Function call failed: ";
			errString.append(err.what()).append(" - ");
			errString.append(lua_tostring(err.state(),-1));
			_log->logMessage(errString);
	}
}

//call the method of lua with the enemy logic and change his ENEMYSTATE 
void iceLogicLua::initCutScene(iceCutScene* pCutscene)
{
	// try catch for luabind
	try{
		unsigned int ret = -1;
		// we need the name of the function
		Ogre:: String funcName = pCutscene->getInitFunctionStr();
		//Check if the function exists
		if( FuncExist( funcName.c_str() )){
			//call Lua Function
			luabind::call_function<void>(L, funcName.c_str(), pCutscene );
			// log
			std::stringstream ss;
			ss << pCutscene->getStep();
			_log->logMessage( ss.str() );
		}else{
			std::stringstream ss;
			ss << "IA: function " << funcName << " does NOT exist";
			_log->logMessage( ss.str() );
		}
	}catch(const luabind::error& err){
			//Catch and log the message
			std::string errString = "LUA Function call failed: ";
			errString.append(err.what()).append(" - ");
			errString.append(lua_tostring(err.state(),-1));
			_log->logMessage(errString);
	}
}

void iceLogicLua::updateCutScene(iceCutScene* pCutscene, Ogre::Real p_timeSinceLastFrame)
{
		// try catch for luabind
	try{
		unsigned int ret = -1;
		// we need the name of the function
		Ogre:: String funcName = pCutscene->getUpdateFunctionStr();
		//Check if the function exists
		if( FuncExist( funcName.c_str() )){
			//call Lua Function
			luabind::call_function<void>(L, funcName.c_str(), pCutscene );
			// log
			std::stringstream ss;
			ss << pCutscene->getStep();
			_log->logMessage( ss.str() );
		}else{
			std::stringstream ss;
			ss << "IA: function " << funcName << " does NOT exist";
			_log->logMessage( ss.str() );
		}
	}catch(const luabind::error& err){
			//Catch and log the message
			std::string errString = "LUA Function call failed: ";
			errString.append(err.what()).append(" - ");
			errString.append(lua_tostring(err.state(),-1));
			_log->logMessage(errString);
	}
}