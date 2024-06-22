#include "ResourceManager.h"

ResourceManager::ResourceManager(std::string const path) : anchorPath(path) {
}

ResourceManager::~ResourceManager() = default;

int ResourceManager::Init() {
	if (!std::filesystem::exists(anchorPath) || !std::filesystem::is_directory(anchorPath)) {
		Game::S().LOGGER.log("[!] ResourceManager: Directory " + anchorPath.string() + " does not exist");
		initState = -1;
		return initState;
	}
	initState = 0;
	return initState;
}

std::optional<std::string> ResourceManager::ReadFile(std::string const fileName) const
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
	std::stringstream buffer;
	buffer << file.rdbuf();
	return std::move(buffer.str());
}

int ResourceManager::Exit() {
	return initState;
}

void ResourceManager::Event() {

}

void ResourceManager::Render() {

}

void ResourceManager::Update() {

}