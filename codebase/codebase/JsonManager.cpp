#include "JsonManager.h"
//using namespace std::string_literals;

JsonManager::JsonManager(std::string const path) : anchorPath(path) {
}

JsonManager::~JsonManager() = default;

int JsonManager::Init() {
	if (!std::filesystem::exists(anchorPath) || !std::filesystem::is_directory(anchorPath)) {
		Game::S().LOGGER.log("[!] ResourceManager: Directory " + anchorPath.string() + " does not exist");
		initState = -1;
		return initState;
	}
	initState = 0;
	return initState;
}

std::optional<json> JsonManager::LoadJson(std::string const fileName) const
{
	if (initState != 0) {
		return std::nullopt;
	}
	std::filesystem::path const filePath = anchorPath / fileName;
	if (!std::filesystem::exists(filePath)) {
		Game::S().LOGGER.log("[!] ResourceManager: File " + filePath.string() + " does not exist");
		return std::nullopt;
	}
	std::ifstream file(filePath);
	if (!file) {
		Game::S().LOGGER.log("[!] ResourceManager: Failed to open " + filePath.string());
		return std::nullopt;
	}
	std::optional<json> data = std::nullopt;
	try {
		data = json::parse(file);
	}
	catch (json::exception& e) {
		Game::S().LOGGER.log("Error while parsing JSON file at " + filePath.string() + "\n" + e.what());
	}
	return data;
}

int JsonManager::Exit() {
	return initState;
}

void JsonManager::Event() {

}

void JsonManager::Render() {

}

void JsonManager::Update() {

}