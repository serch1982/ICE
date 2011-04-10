
#include "iceLogicLua.h"

#define LUA_CONST_START( class ) { luabind::object g = luabind::globals(L); luabind::object table = g[#class];
#define LUA_CONST( class, name ) table[#name] = class::name
#define LUA_CONST_END }

iceLogicLua* iceLogicLua::pinstance = 0;

iceLogicLua* iceLogicLua::instance(){
	if (pinstance == 0)
          pinstance = new iceLogicLua;
        return pinstance;
}

iceLogicLua::iceLogicLua(void){
	mGameLog = Ogre::LogManager::getSingleton().createLog("iceLog.log", false, false, false );
	L = lua_open();	
	luaL_openlibs(L);
	//lua_baselibopen(L);
	luabind::open(L);
}

iceLogicLua::~iceLogicLua(void){
   lua_close(L);
}

int iceLogicLua::RunFile( const char *fname )
{
	int ret;
	if( ret = luaL_loadfile(L, fname) || lua_pcall( L, 0 ,0 ,0 )){
		errorHandler();
	}else{
		mGameLog->logMessage("The lua file has been loaded");
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

	mGameLog->logMessage("Lua exception:"  + std::string(ss.str()) );
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
			mGameLog->logMessage("the Function doesnt exists:"  + std::string(name) );
		}
	}

	return false;
}

int iceLogicLua::testfunctionlua(int num){
	if(FuncExist("test")){
		 return luabind::call_function<int>(L, "test",num);
	}else{
		return 0;
	}
}

void iceLogicLua::bindLuaObjects(){
	luabind::module(L)
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
		//.def("ptr", &Vector3::ptr )
		.def("randomDeviant", &Vector3::randomDeviant )
		.def("reflect", &Vector3::reflect )
		.def("squaredDistance", &Vector3::squaredDistance )
		.def("squaredLength", &Vector3::squaredLength )

		// Operators

		.def( luabind::self + luabind::other<Vector3>() )
		.def( luabind::self - luabind::other<Vector3>() )
		.def( luabind::self * luabind::other<Vector3>() )
		.def( luabind::self * Real() )
	];

	LUA_CONST_START( Vector3 )
		LUA_CONST( Vector3, ZERO);
		LUA_CONST( Vector3, UNIT_X );
		LUA_CONST( Vector3, UNIT_X);
		LUA_CONST( Vector3, UNIT_Y);
		LUA_CONST( Vector3, UNIT_Z);
		LUA_CONST( Vector3, NEGATIVE_UNIT_X);
		LUA_CONST( Vector3, NEGATIVE_UNIT_Y);
		LUA_CONST( Vector3, NEGATIVE_UNIT_Z);
		LUA_CONST( Vector3, UNIT_SCALE);
	LUA_CONST_END;


	luabind::module(L) [
		luabind::def("availableForLua", &iceLogicLua::availableForLua)
	];

}

Vector3 iceLogicLua::availableForLua(Vector3 &myvec) {
	myvec.x = 2000;
	return myvec; 
}