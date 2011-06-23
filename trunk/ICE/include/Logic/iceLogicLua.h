#ifndef ICELOGICLUA_H_
#define ICELOGICLUA_H_

#pragma once

extern "C" {
	#include "LUA51\lua.h"
	#include "LUA51\lualib.h"
	#include "LUA51\lauxlib.h"
}

#include "Enemies\iceEnemy.h"
#include "luabind\luabind.hpp"
#include "luabind\operator.hpp"
#include <Ogre.h>
#include <sstream>
#include <string>



class iceLogicLua
{
public:
	static iceLogicLua* getInstance();
	~iceLogicLua();

	int RunFile( const char *fname );
	void getEnemyLogicState(iceEnemy *enemy, Ogre::Real p_timeSinceLastFrame);
	int RunSource( const char *source );
protected:
	  iceLogicLua();

private:
	Ogre::Log* _log;
	static iceLogicLua* pinstance;
	lua_State* L;

	void iceLogicLua::bindLuaObjects();
	int errorHandler();
	void ForceGarbageCollect();
	bool FuncExist(const char *name );

};
#endif