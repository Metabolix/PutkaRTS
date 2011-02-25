/*
 * Path utilities.
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

#include "Path.hpp"

#include <boost/filesystem.hpp>
#include <string>

namespace Path {
	/** Global data directory, often /usr/share/PutkaRTS */
	static boost::filesystem::path globalDataDir;

	/** Local data directory, often /home/user/.local/share/PutkaRTS */
	static boost::filesystem::path localDataDir;

	/** Local configuration directory, often /home/user/.config/PutkaRTS */
	static boost::filesystem::path localConfigDir;

	/**
	 * Create a directory. (Helper function for recursive use.)
	 *
	 * @param path The directory to create.
	 * @param recurse Use recursion.
	 * @return True if the directory was created or recursion was enabled and the directory exists.
	 */
	static bool mkdir(const boost::filesystem::path& path, bool recurse);
}

static bool Path::mkdir(const boost::filesystem::path& path, bool recurse)
try {
	if (recurse) {
		boost::filesystem::path parent = path.parent_path();
		if (!boost::filesystem::exists(parent)) {
			mkdir(parent, recurse);
		}
		if (boost::filesystem::is_directory(path)) {
			return true;
		}
	}
	return boost::filesystem::create_directory(path);
} catch (boost::filesystem::basic_filesystem_error<boost::filesystem::path>& e) {
	return false;
}

bool Path::mkdir(const std::string& path) {
	return mkdir(boost::filesystem::path(path), true);
}

void Path::init(const std::string& argv0) {
	// This works in the development environment:
	// argv0 = root/bin/program; chdir to root; everything else is right here as well.
	boost::filesystem::path binaryPath(argv0);
	if (binaryPath.has_parent_path()) {
		boost::filesystem::current_path(binaryPath.parent_path());
	}
	boost::filesystem::current_path("..");

	globalDataDir = boost::filesystem::current_path() / "data";
	localConfigDir = boost::filesystem::current_path() / "local";
	localDataDir = boost::filesystem::current_path() / "local";

	// Override the global data dir with preprocessor.
	#if defined(GLOBAL_DATA_DIR)
		globalDataDir = GLOBAL_DATA_DIR;
	#endif

	#if !defined(USE_SYSTEM_PATHS)
		// Do nothing; use the development paths above.
	#elif defined(_WIN32)
		// TODO: Implement Windows paths!
	#else
		// Others: try some environment variables.
		const char *home = getenv("HOME");
		const char *config = getenv("XDG_CONFIG_HOME");
		const char *data = getenv("XDG_DATA_HOME");

		if (!boost::filesystem::exists(home)) {
			home = 0;
		}
		if (!boost::filesystem::exists(config)) {
			config = 0;
		}
		if (!boost::filesystem::exists(data)) {
			data = 0;
		}

		if (config) {
			localConfigDir = config;
			localConfigDir /= "PutkaRTS";
		} else if (home) {
			localConfigDir = home;
			localConfigDir /= ".config/PutkaRTS";
		}

		if (data) {
			localDataDir = data;
			localDataDir /= "PutkaRTS";
		} else if (home) {
			localDataDir = home;
			localDataDir /= ".local/share/PutkaRTS";
		}
	#endif

	if (!boost::filesystem::exists(globalDataDir)) {
		throw std::runtime_error("Could not find global data dir! Tried: " + globalDataDir.string());
	}
}

std::string Path::getConfigPath(const std::string& name) {
	mkdir(localConfigDir, true);
	return (localConfigDir / name).string();
}

std::string Path::getLocalDataPath(std::string const& name) {
	return (localDataDir / name).string();
}

std::string Path::findDataPath(std::string const& name) {
	boost::filesystem::path tmp;
	if (boost::filesystem::exists(tmp = localDataDir / name)) {
		return tmp.string();
	}
	if (boost::filesystem::exists(tmp = globalDataDir / name)) {
		return tmp.string();
	}
	return name;
}

std::string Path::findDataPath(std::string const& dir, std::string const& name) {
	boost::filesystem::path tmp;
	if (boost::filesystem::exists(tmp = localDataDir / dir / name)) {
		return tmp.string();
	}
	if (boost::filesystem::exists(tmp = globalDataDir / dir / name)) {
		return tmp.string();
	}
	return name;
}

std::string Path::findDataPath(std::string const& prefix1, std::string const& prefix2, std::string const& name) {
	boost::filesystem::path tmp;
	if (boost::filesystem::exists(tmp = localDataDir / prefix1 / name)) {
		return tmp.string();
	}
	if (boost::filesystem::exists(tmp = localDataDir / prefix2 / name)) {
		return tmp.string();
	}
	if (boost::filesystem::exists(tmp = globalDataDir / prefix1 / name)) {
		return tmp.string();
	}
	if (boost::filesystem::exists(tmp = globalDataDir / prefix2 / name)) {
		return tmp.string();
	}
	return name;
}
