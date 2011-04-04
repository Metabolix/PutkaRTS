/*
 * Tech tree class definition.
 *
 * Copyright 2011 Petri Österman
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

#ifndef PUTKARTS_World_TechTree_HPP
#define PUTKARTS_World_TechTree_HPP

#include "ObjectAction.hpp"
#include "ObjectType.hpp"

#include <map>
#include <stdexcept>
#include <boost/shared_ptr.hpp>

namespace World {
	class TechTree;
}

/**
 * This class describes tech tree.
 */
class World::TechTree {
	/** All object actions in the tech tree. */
	std::map<std::string, boost::shared_ptr<World::ObjectAction> > objectActions;

	/** All object types in the tech tree. */
	std::map<std::string, boost::shared_ptr<World::ObjectType> > objectTypes;
public:
	/**
	 * Default constructor.
	 */
	TechTree() {
		// Nothing here.
	}

	/**
	 * Constructor that also loads the tech tree.
	 *
	 * @param directory The tech tree file directory.
	 */
	TechTree(const std::string& directory) {
		load(directory);
	}

	/**
	 * Load tech tree from file.
	 *
	 * @param directory The tech tree file directory.
	 */
	void load(const std::string& directory);

	/**
	 * Get object action.
	 *
	 * @param action Object action's name.
	 * @return Object action.
	 * @return std::logic_error, if object action was not found.
	 */
	boost::shared_ptr<const World::ObjectAction> getObjectAction(const std::string& action) const {
		std::map<std::string, boost::shared_ptr<World::ObjectAction> >::const_iterator iterator = objectActions.find(action);
		if (iterator == objectActions.end()) {
			throw std::logic_error("World::TechTree::getObjectAction: action not found!");
		}
		return iterator->second;
	}

	/**
	 * Get object type.
	 *
	 * @param type Object type's name.
	 * @return Object type.
	 * @throw std::logic_error, if object type was not found.
	 */
	boost::shared_ptr<const World::ObjectType> getObjectType(const std::string& type) const {
		std::map<std::string, boost::shared_ptr<World::ObjectType> >::const_iterator iterator = objectTypes.find(type);
		if (iterator == objectTypes.end()) {
			throw std::logic_error("World::TechTree::getObjectType: type not found!");
		}
		return iterator->second;
	}

	/**
	 * Add object type to tech tree.
	 *
	 * If tech tree has object type with same name, existing object type will be overwritten.
	 *
	 * @param name Object type's name.
	 * @param objectType Object type to add.
	 */
	void insertObjectType(const std::string& name, const boost::shared_ptr<World::ObjectType>& objectType) {
		objectTypes[name] = objectType;
	}

	/**
	 * Add object action to tech tree.
	 *
	 * If tech tree has object action with same name, existing object action will be overwritten.
	 *
	 * @param name Object action's name.
	 * @param objectAction Object action to add.
	 */
	void insertObjectAction(const std::string& name, const boost::shared_ptr<World::ObjectAction>& objectAction) {
		objectActions[name] = objectAction;
	}

	/**
	 * Delete object type from tech tree.
	 *
	 * @param name Object type's name.
	 */
	void eraseObjectType(const std::string& name) {
		std::map<std::string, boost::shared_ptr<World::ObjectType> >::iterator iterator = objectTypes.find(name);
		if (iterator != objectTypes.end()) {
			objectTypes.erase(iterator);
		}
	}

	/**
	 * Delete object action from tech tree.
	 *
	 * @param name Object action's name.
	 */
	void eraseObjectAction(const std::string& name) {
		std::map<std::string, boost::shared_ptr<World::ObjectAction> >::iterator iterator = objectActions.find(name);
		if (iterator != objectActions.end()) {
			objectActions.erase(iterator);
		}
	}
};

#endif
