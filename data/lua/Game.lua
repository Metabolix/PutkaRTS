-- DO NOT TOUCH! This is the Lua interface for the tech tree and scripts.
--
-- Copyright 2011 Lauri Kenttä
--
-- This file is part of PutkaRTS.
--
-- PutkaRTS is free software: you can redistribute it and/or modify
-- it under the terms of the GNU General Public License as published by
-- the Free Software Foundation, either version 3 of the License, or
-- (at your option) any later version.
--
-- PutkaRTS is distributed in the hope that it will be useful,
-- but WITHOUT ANY WARRANTY; without even the implied warranty of
-- MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
-- GNU General Public License for more details.
--
-- You should have received a copy of the GNU General Public License
-- along with PutkaRTS.  If not, see <http://www.gnu.org/licenses/>.

--- Global methods and methods that interface with the whole game.
Game = {
	--- Table of object types.
	objectTypes = {},

	--- The counter for getFreeId.
	freeIdCounter = 0,

	--- Generate a new id.
	---
	--- @return A new id different from any previous ones.
	getFreeId = function()
		Game.freeIdCounter = Game.freeIdCounter + 1
		return Game.freeIdCounter
	end,
}

--- Methods related to the ObjectType class.
ObjectType = {
	--- Add a new object type.
	new = function(t)
		t.id = t.id or Game.getFreeId()
		luaNewObjectType(
			t.id,
			t.name or "Unknown",
			t.immutable or false,
			t.radius or 0.5,
			t.maxVelocity or 0,
			t.lineOfSight or 10,
			t.maxHitPoints or 0,
			nil
		)
		Game.objectTypes[t.id] = t
		return t
	end,
}
