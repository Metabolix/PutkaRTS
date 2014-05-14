#ifndef PUTKARTS_Path_HPP
#define PUTKARTS_Path_HPP

#include <string>

/**
 * Functions for locating game files and handling filesystem.
 */
namespace Path {
	/**
	 * Create a directory. Recursively create parent directories as well.
	 *
	 * @param path The directory path to create.
	 * @return True if the directory exists or was created.
	 */
	extern bool mkdir(const std::string& path);

	/**
	 * Create the parent directory for a file.
	 *
	 * For example, for the path "config/my.conf" create directory "config".
	 *
	 * @param path The file name.
	 * @return True if the directory exists or was created.
	 */
	extern bool mkdirForFile(const std::string& path);

	/**
	 * Check if a path exists.
	 *
	 * @param path The path.
	 * @return True if the path exists.
	 */
	extern bool exists(const std::string& path);

	/**
	 * Read a whole file.
	 *
	 * @param path The path.
	 * @return Contents of the file.
	 */
	extern std::string readFile(const std::string& path);

	/**
	 * Initialize the data and configuration paths.
	 *
	 * @param argv0 The first command line argument (program name).
	 */
	extern void init(const std::string& argv0);

	/**
	 * Get the path of a configuration file.
	 *
	 * As a side effect, create the configuration directory.
	 *
	 * @param name Configuration file name, for example PutkaRTS.conf
	 * @return Configuration file path.
	 */
	extern std::string getConfigPath(const std::string& name);

	/**
	 * Get the local data path for storing a file.
	 *
	 * @param name The file name, for example maps/newmap/data.txt.
	 * @return The whole path.
	 */
	extern std::string getLocalDataPath(std::string const& name);

	/**
	 * Find a data file in either local or global data directory.
	 *
	 * @param name The file name to look for, for example maps/somemap/data.txt.
	 * @return The whole path, if the file exists; otherwise, the given name.
	 */
	extern std::string findDataPath(std::string const& name);

	/**
	 * Find a data file (dir/name) in either local or global data directory.
	 *
	 * @param dir The directory part to prepend to the name, for example maps/somemap.
	 * @param name The file name to look for, for example data.txt.
	 * @return The whole path, if the file exists; otherwise, the given dir/name.
	 */
	extern std::string findDataPath(std::string const& dir, std::string const& name);

	/**
	 * Find a data file with either of the given prefices in either local
	 * or global data directory.
	 *
	 * This function can be used to resolve, for example, map tile paths.
	 * With prefix1 = "maps/mapname", prefix2 = "" and name = "tiles/x.png",
	 * it considers the following paths and returns the first that exists:
	 *
	 * - localDataDir/maps/mapname/tiles/x.png
	 * - localDataDir/tiles/x.png
	 * - globalDataDir/maps/mapname/tiles/x.png
	 * - globalDataDir/tiles/x.png
	 *
	 * @param prefix1 The first subdirectory to try, for example "maps/mapname".
	 * @param prefix2 The second subdirectory to try, for example "" (empty).
	 * @param name The file name to look for, for example "tiles/x.png".
	 * @return The whole path, if the file exists; otherwise, the given name.
	 */
	extern std::string findDataPath(std::string const& prefix1, std::string const& prefix2, std::string const& name);
}

#endif
