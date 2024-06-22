#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "LoggingManager.h"
#include "Manager.h"
#include <nlohmann/json.hpp>
#include "Game.h"
#include <string>
#include <filesystem>
#include <optional>
#include <sstream>
#include <fstream>
#include <memory>

using json = nlohmann::json;

/* This class creates an easy interface to pull json files */
class ResourceManager : public Manager {
private:
	/* This should be a directory containing the json files. */
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

	/* Reading from a file or parsing json can fail, so we return an optional. Error is logged. */
	std::optional<json> LoadJson(std::string const fileName) const;
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
