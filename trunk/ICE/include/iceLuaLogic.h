#ifndef __iceLuaLogic_h_
#define __iceLuaLogic_h_

#pragma comment(lib,"lua+lib")

extern "C" {
	#include "LUA/lua.h"
	#include "LUA/lualib.h"
	#include "LUA/lauxlib.h"
}


class iceLuaLogic
{
public:
	static iceLuaLogic* instance();
	~iceLuaLogic();
	int testFromOgre();

protected:
	  iceLuaLogic();

private:
	static iceLuaLogic* pinstance;
	void InitLua();
	void Finalize();
	lua_State* L;

	
	static int testtoLua(lua_State *L);
	
};
#endif