#pragma once

#include <string>
#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

bool exists(std::string);
bool makeDir(std::string);
std::string combinePath(std::string, std::string);
std::string getFolder(std::string);
std::vector<fs::path> getAllPathsFromFolder(std::string);
std::vector<std::string> getAllPathsFromFolderAsString(std::string);
std::vector<std::string> getThingInFolderWithWildcard(std::string, std::string = "*");
std::string getExtensionFromPath(std::string);
std::string getStemFromPath(std::string);
std::string getParentFromPath(std::string);
std::string getPathWithoutExtension(std::string);
