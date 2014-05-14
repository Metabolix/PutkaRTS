#include <string>
#include <fstream>
#include <boost/filesystem.hpp>

#include "Path.hpp"

#if defined(_WIN32)
	#define UNICODE
	#include <windows.h>
	#include <shlobj.h>
#endif

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
		if (path.has_parent_path()) {
			boost::filesystem::path parent = path.parent_path();
			if (!boost::filesystem::exists(parent)) {
				mkdir(parent, recurse);
			}
		}
		if (boost::filesystem::is_directory(path)) {
			return true;
		}
	}
	return boost::filesystem::create_directory(path);
} catch (boost::filesystem::filesystem_error& e) {
	return false;
}

bool Path::mkdir(const std::string& path) {
	return mkdir(boost::filesystem::path(path), true);
}

bool Path::mkdirForFile(const std::string& pathStr) {
	boost::filesystem::path path(pathStr);
	if (!path.has_parent_path()) {
		// Assume that root paths and current path always exist.
		return true;
	}
	return mkdir(path.parent_path(), true);
}

bool Path::exists(const std::string& path) {
	return boost::filesystem::exists(path);
}

std::string Path::readFile(const std::string& path) {
	std::ifstream ifs(path.c_str(), std::ios::binary);
	if (!ifs) {
		throw std::runtime_error("File not found: " + path);
	}
	char buf[4096];
	std::string result;
	while (ifs.peek() != EOF) {
		ifs.read(buf, sizeof(buf));
		result += std::string(buf, buf + ifs.gcount());
	}
	return result;
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
		// Windows: use Application Data for local stuff.

		/* The program is not made for the UTF-16 used in Windows,
		 * but to be nice, we'll use a small hack, as follows:
		 *
		 * The installation dir is like C:\Program Files\PutkaRTS.
		 * The path is likely to contain only 8-bit characters ("ANSI codepage").
		 * The user data dir is like C:\Users\Jørgen-äijä\Application Data.
		 * The path may contain just anything, depending on the login name.
		 *
		 * So the trick is to chdir to Application Data here with
		 * any Unicode characters and then use relative paths for
		 * the local files. This way the rest of the program doesn't
		 * need to care about UTF-16 at all.
		 */
		TCHAR w32ApplicationData[MAX_PATH];
		if (SHGetFolderPath(0, CSIDL_APPDATA, 0, 0, w32ApplicationData) == S_OK) {
			if (SetCurrentDirectory(w32ApplicationData)) {
				localDataDir = localConfigDir = "PutkaRTS";
			}
		}
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
