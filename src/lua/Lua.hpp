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

#ifndef PUTKARTS_Lua_HPP
#define PUTKARTS_Lua_HPP

#include <cstddef>
#include <stdexcept>
#include <vector>
#include <boost/unordered_map.hpp>
#include <boost/function.hpp>
#include <boost/any.hpp>
#include "util/Path.hpp"

extern "C" {
	struct lua_State;
}

/**
 * Wrapper for Lua.
 */
class Lua {
public:
	/** Boolean type. */
	typedef bool Boolean;

	/** Integer type. */
	typedef ptrdiff_t Integer;

	/** Number type. */
	typedef double Number;

	/** String type. */
	typedef std::string String;

	/** Exception type. */
	typedef std::runtime_error Exception;

protected:
	/** Function type. */
	typedef boost::function<void()> Function;

private:
	/** The Lua VM state. */
	lua_State* state;

	/** Allocated functions. */
	boost::unordered_map<std::string, Function> functions;

protected:
	/** Include directories. */
	std::vector<std::string> directories;

	/**
	 * Read one value from Lua.
	 *
	 * The template parameter must be one of the following:
	 * Boolean, Integer, Number, String, Table, boost::any.
	 *
	 * @param index Index of the value in the Lua stack.
	 * @return The value.
	 * @throw Exception Thrown if the value doesn't match.
	 */
	template <typename T> T get(int index) const {
		return get<String>(index);
	}

	/**
	 * Push one value to Lua.
	 *
	 * The template parameter must be one of the following:
	 * Boolean, Integer, Number, String.
	 *
	 * @param value The value to push.
	 */
	template <typename T> void push(const T& value) {
		return push<Lua::String>(value);
	}

	/**
	 * Pop one value from the Lua stack.
	 *
	 * The template parameter must be one of the following:
	 * Boolean, Integer, Number, String, boost::any, void.
	 *
	 * @param count The number of values to pop.
	 */
	template <typename T> T pop() {
		return boost::any_cast<T>(pop<boost::any>());
	}

	/**
	 * Bind a function.
	 *
	 * @param name A name for the function.
	 * @param function The function.
	 */
	void bind(const std::string& name, Function function);

	/**
	 * Remove a function binding.
	 *
	 * @param name The name to remove.
	 */
	void unbind(const std::string& name);

	/**
	 * Load some Lua code on the stack.
	 *
	 * @param code The Lua code.
	 * @param context The indentifier used in error reporting.
	 * @throw Exception Thrown if anything goes wrong.
	 */
	void load(const std::string& code, const std::string& context = "evaluated code");

	/**
	 * Call a Lua function that is already in the stack.
	 *
	 * @param nargs The number of arguments on the stack.
	 * @param nresults The number of results.
	 * @throw Exception Thrown if anything goes wrong.
	 */
	void call(int nargs, int nresults);

private:
	/**
	 * This function forwards calls from Lua to the actual Function object.
	 *
	 * @param state The Lua state.
	 * @return How many values does this function return to Lua?
	 */
	static int luaThunk(lua_State* state);

	/**
	 * Lua callback: include another file.
	 */
	void luaInclude();

public:
	/**
	 * Constructor.
	 */
	Lua();

	/**
	 * Destructor.
	 */
	virtual ~Lua();

	/**
	 * Run some Lua code.
	 *
	 * The template parameter must be one of the following:
	 * Boolean, Integer, Number, String, boost::any, void.
	 *
	 * @param code The Lua code.
	 * @param context The indentifier used in error reporting.
	 * @return The return value of the Lua code.
	 * @throw Exception Thrown if anything goes wrong.
	 */
	template <typename T> T run(const std::string& code, const std::string& context) {
		load(code, context);
		call(0, 1);
		return pop<T>();
	}

	/**
	 * Run some Lua code.
	 *
	 * The template parameter must be one of the following:
	 * Boolean, Integer, Number, String, boost::any, void.
	 *
	 * @param code The Lua code.
	 * @return The return value of the Lua code.
	 * @throw Exception Thrown if anything goes wrong.
	 */
	template <typename T> T run(const std::string& code) {
		return run<T>(code, "evaluated code");
	}

	/**
	 * Run a file.
	 *
	 * The template parameter must be one of the following:
	 * Boolean, Integer, Number, String, boost::any, void.
	 *
	 * @param file The file.
	 * @return The return value of the Lua code.
	 * @throw Exception Thrown if anything goes wrong.
	 */
	template <typename T> T runFile(const std::string& file) {
		return run<T>(Path::readFile(file), file);
	}
};

/** @cond */
template <> Lua::Boolean Lua::get<Lua::Boolean>(int index) const;
template <> Lua::Integer Lua::get<Lua::Integer>(int index) const;
template <> Lua::Number Lua::get<Lua::Number>(int index) const;
template <> Lua::String Lua::get<Lua::String>(int index) const;
template <> boost::any Lua::get<boost::any>(int index) const;

template <> void Lua::push<Lua::Boolean>(const Boolean& value);
template <> void Lua::push<Lua::Integer>(const Integer& value);
template <> void Lua::push<Lua::Number>(const Number& value);
template <> void Lua::push<Lua::String>(const String& value);

template <> boost::any Lua::pop<boost::any>();
template <> void Lua::pop<void>();

template <> inline Lua::Number Lua::pop<Lua::Number>() {
	boost::any result(Lua::pop<boost::any>());
	if (boost::any_cast<Lua::Integer>(&result)) {
		return boost::any_cast<Lua::Integer>(result);
	}
	return boost::any_cast<Lua::Number>(result);
}
/** @endcond */

#endif
