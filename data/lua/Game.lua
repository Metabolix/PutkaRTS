-- DO NOT TOUCH unless you're a developer!
-- This is the Lua interface for the tech tree and scripts.

--- Global methods and methods that interface with the whole game.
Game = {
	--- Table of object types.
	objectTypes = {},

	--- Table of object actions.
	objectActions = {},

	--- Table of objects.
	objects = {},

	--- The counter for getFreeId.
	freeIdCounter = 0,

	--- Generate a new id.
	---
	--- @return A new id different from any previous ones.
	getFreeId = function()
		Game.freeIdCounter = Game.freeIdCounter + 1
		return Game.freeIdCounter
	end,

	--- Run any Lua actions associated with a message.
	handleMessage = function(...)
		local message = Message.fromValues(...)
		local delete = (message.action == "delete")
		for i = 1, #message.actors do
			local object = Game.objects[message.actors[i]]
			if not object then
				-- nothing
			elseif delete then
				Object.delete(object)
			elseif object.objectType[action] then
				object.objectType[action](object)
			end
		end
	end,
}

--- Methods related to the Message class.
Message = {
	--- Make a message from the given values.
	fromValues = function(action, ...)
		return {
			action = action,
			actors = {...},
		}
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

--- Methods related to the ObjectAction class.
ObjectAction = {
	--- Add a new object action.
	new = function(t)
		t.id = t.id or Game.getFreeId()
		luaNewObjectAction(
			t.id,
			t.name or "Unknown",
			nil
		)
		Game.objectActions[t.id] = t
		return t
	end,
}

--- Methods related to the Object class.
Object = {
	--- Create a new object.
	new = function(t)
		if t.objectTypeId then
			t.objectType = Game.objectTypes[t.objectTypeId]
			t.objectTypeId = nil
		end
		-- If t.id is set, the native Object already exists.
		t.id = t.id or luaNewObject(
			t.objectType.id,
			t.playerId,
			t.x,
			t.y,
			nil
		)
		Game.objects[t.id] = t
		if t.objectType.new then
			t.objectType.new(t)
		end
		return t
	end,

	--- Delete an existing object.
	delete = function(t)
		if t.objectType.delete then
			t.objectType.delete(t)
		end
		if t.id then
			luaDeleteObject(t.id)
			Game.objects[t.id] = nil
			t.id = nil
		end
	end,
}
