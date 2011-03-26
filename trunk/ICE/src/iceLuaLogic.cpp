
#include "iceLuaLogic.h"

iceLuaLogic* iceLuaLogic::pinstance = 0;

iceLuaLogic* iceLuaLogic::instance(){
	if (pinstance == 0)
          pinstance = new iceLuaLogic;
        return pinstance;
}


iceLuaLogic::iceLuaLogic(void){
	L =0;
	InitLua();
}

iceLuaLogic::~iceLuaLogic(void){
   Finalize();
}

void iceLuaLogic::Finalize()
{
	lua_close(L);
}

void iceLuaLogic::InitLua()
{
	L = lua_open();
	lua_baselibopen(L);
	lua_register(L,"testtoLua",testtoLua);
	lua_dofile(L,"iceLuaLogic.lua");
}


int iceLuaLogic::testtoLua(lua_State *L){
	lua_pushnumber(L,200);
	return 1;

}
int iceLuaLogic::testFromOgre(){
	int new_state;
	int state = 100;

	lua_getglobal(L, "FromLua");
	lua_pushnumber(L, state);
	lua_call(L, 1, 1);
	new_state   = (int)lua_tonumber(L, -1);
	lua_pop(L, 1);

	return new_state;
}