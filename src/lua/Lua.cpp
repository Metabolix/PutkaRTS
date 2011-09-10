/*
 * Wrapper for Lua.
 *
 * Copyright 2011 Lauri Kenttä
 *
 * This file is part of PutkaRTS.
 *
 * PutkaRTS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * PutkaRTS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PutkaRTS.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <boost/bind.hpp>

#include "Lua.hpp"

extern "C" {
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

Lua::Lua() {
	state = lua_open();
	bind("include", boost::bind(&Lua::luaInclude, this));
}

Lua::~Lua() {
	lua_close(state);
}

int Lua::luaThunk(lua_State* state) {
	Lua& self = *reinterpret_cast<Lua*>(lua_touserdata(state, lua_upvalueindex(1)));
	Function& func = self.functions.find(lua_tostring(state, lua_upvalueindex(2)))->second;

	try {
		int args = lua_gettop(state);
		func();
		return lua_gettop(state) - args;
	} catch (Lua::Exception& e) {
		lua_pushstring(state, e.what());
		lua_error(state);
		return 0;
	}
}

void Lua::unbind(const std::string& name) {
	if (functions.erase(name)) {
		lua_pushnil(state);
		lua_setglobal(state, name.c_str());
	}
}

void Lua::bind(const std::string& name, Function function) {
	unbind(name);
	functions[name] = function;
	lua_pushlightuserdata(state, this);
	lua_pushlstring(state, name.c_str(), name.size());
	lua_pushcclosure(state, luaThunk, 2);
	lua_setglobal(state, name.c_str());
}

void Lua::luaInclude() {
	String file = get<String>(1);
	for (std::vector<std::string>::const_iterator i = directories.begin(); i != directories.end(); ++i) {
		std::string path = Path::findDataPath(*i, file);
		if (Path::exists(path)) {
			return runFile<void>(path);
		}
	}
	return runFile<void>(Path::findDataPath(file));
}

template <> Lua::Boolean Lua::get<Lua::Boolean>(int index) const {
	if (!lua_isboolean(state, index)) {
		throw Exception("Value is not a number!");
	}
	return lua_toboolean(state, index);
}

template <> Lua::Integer Lua::get<Lua::Integer>(int index) const {
	Number n = get<Number>(index);
	Integer i = lua_tointeger(state, index);
	if (n != i) {
		throw Exception("Value is not an integer!");
	}
	return i;
}

template <> Lua::Number Lua::get<Lua::Number>(int index) const {
	if (!lua_isnumber(state, index)) {
		throw Exception("Value is not a number!");
	}
	return lua_tonumber(state, index);
}

template <> Lua::String Lua::get<Lua::String>(int index) const {
	if (!lua_isstring(state, index)) {
		throw Exception("Value is not a string!");
	}
	return lua_tostring(state, index);
}

template <> boost::any Lua::get<boost::any>(int index) const {
	switch (lua_type(state, index)) {
		case LUA_TNONE:
		case LUA_TNIL:
			return boost::any();
		case LUA_TNUMBER: {
			Integer i = lua_tointeger(state, index);
			double d = lua_tonumber(state, index);
			if (i == d) {
				return i;
			} else {
				return d;
			}
			break;
		}
		case LUA_TBOOLEAN:
			return get<Boolean>(index);
		case LUA_TSTRING:
			return get<String>(index);
		case LUA_TTABLE:
		case LUA_TFUNCTION:
		case LUA_TUSERDATA:
		case LUA_TTHREAD:
		case LUA_TLIGHTUSERDATA:
		default:
			throw Exception("Unsupported value type!");
	}
}

template <> void Lua::push<Lua::Boolean>(const Boolean& value) {
	lua_pushboolean(state, value);
}

template <> void Lua::push<Lua::Integer>(const Integer& value) {
	lua_pushinteger(state, value);
}

template <> void Lua::push<Lua::Number>(const Number& value) {
	lua_pushnumber(state, value);
}

template <> void Lua::push<Lua::String>(const String& value) {
	lua_pushlstring(state, value.c_str(), value.size());
}

template <> boost::any Lua::run<boost::any>(const std::string& code, const std::string& context) {
	int ret;
	std::string str;
	ret = luaL_loadbuffer(state, code.c_str(), code.size(), context.c_str());
	if (ret != 0) {
		str = get<String>(lua_gettop(state));
		lua_pop(state, 1);
		switch (ret) {
			case LUA_ERRSYNTAX:
				throw Exception("LUA_ERRSYNTAX: " + str);
			case LUA_ERRMEM:
				throw Exception("LUA_ERRMEM: " + str);
			default:
				throw Exception(str);
		}
	}

	ret = lua_pcall(state, 0, 1, 0);
	if (ret != 0) {
		str = get<String>(lua_gettop(state));
		lua_pop(state, 1);
		switch (ret) {
			case LUA_ERRRUN:
				throw Exception("LUA_ERRRUN: " + str);
			case LUA_ERRMEM:
				throw Exception("LUA_ERRMEM: " + str);
			case LUA_ERRERR:
				throw Exception("LUA_ERRERR: " + str);
			default:
				throw Exception(str);
		}
	}
	boost::any result;
	try {
		result = get<boost::any>(lua_gettop(state));
	} catch (...) {
		lua_pop(state, 1);
		throw;
	}
	lua_pop(state, 1);
	return result;
}
