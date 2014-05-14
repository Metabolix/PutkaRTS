#include "ProgramInfo.hpp"

#ifndef PUTKARTS_VERSION
	#define PUTKARTS_VERSION ""
#endif

namespace ProgramInfo {
	const std::string name = "PutkaRTS";
	const std::string version = PUTKARTS_VERSION[0] ? PUTKARTS_VERSION : "PutkaRTS-devel-unknown";
}
