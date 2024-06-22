#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "LoggingManager.h"
#include <string>
#include <filesystem>
#include <optional>
#include <sstream>
#include <fstream>
#include <memory>

/* This class creates an easy interface to pull json files. Can be extended to load PNG files. */
class ResourceManager {
private:
	/* This should be a directory. */
	std::filesystem::path anchorPath;
	int initState = -1;
public:
	ResourceManager(std::string const path);
	~ResourceManager();

	int Init();
	int Exit();

	void Event();
	void Update();
	void Render();

	/* Reading from a file can fail, so we return an optional. Subject to change? */
	std::optional<std::string> ReadFile(std::string const fileName) const;
};

/*
	Usage:
	ResourceManager mgr("./json");
	mgr.Init() and so on;
	auto file = mgr.ReadFile("meta.json");
	if (file) {
		std::cout << file.value();
	}
*/

#endif
