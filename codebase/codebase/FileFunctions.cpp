#include "FileFunctions.h"

bool exists(std::string folderPath)
{
	return fs::exists(folderPath);
}

bool makeDir(std::string dirPathString)
{
	bool retVal = false;
	fs::path dirPath = dirPathString;
	retVal = fs::create_directory(dirPath);
	return retVal;
}

std::string combinePath(std::string front, std::string back)
{
	fs::path frontPath = front;
	fs::path combinedPath = frontPath / back;
	return combinedPath.string();
}

std::string getFolder(std::string folderPath)
{
	// because exe dir is top dir we don't need to do this stupid next line
	//fs::path relativeFolderPath = fs::current_path().parent_path().parent_path() / folderPath;
	fs::path relativeFolderPath = fs::current_path() / folderPath;
	std::string relativeFolderPathString = relativeFolderPath.string();
	return relativeFolderPathString;
}

std::vector<fs::path> getAllPathsFromFolder(std::string folderPath)
{
	std::vector<fs::path> thingsInFolder = {};
	if (exists(folderPath))
	{
		for (auto thingInFolder : fs::directory_iterator(folderPath))
		{
			thingsInFolder.push_back(thingInFolder.path());
		}
	}

	return thingsInFolder;
}

std::vector<std::string> getAllPathsFromFolderAsString(std::string folderPath)
{
	std::vector<std::string> thingsInFolder = {};

	for (auto thingInFolder : getAllPathsFromFolder(folderPath))
	{
		thingsInFolder.push_back(thingInFolder.string());
	}

	return thingsInFolder;
}

std::vector<std::string> getThingInFolderWithWildcard(std::string folderPath, std::string filter)
{
	std::vector<std::string> thingsInFolder = {};

	for (auto thingInFolder : getAllPathsFromFolder(folderPath))
	{
		if (filter == "*" || thingInFolder.extension().string() == filter)
		{
			thingsInFolder.push_back(thingInFolder.string());
		}
	}

	return thingsInFolder;
}

std::string getExtensionFromPath(std::string thingPath)
{
	fs::path filePath = thingPath;
	return filePath.extension().string();
}

std::string getStemFromPath(std::string thingPath)
{
	fs::path filePath = thingPath;
	return filePath.stem().string();
}

std::string getParentFromPath(std::string thingPath)
{
	fs::path filePath = thingPath;
	return filePath.parent_path().string();
}

std::string getPathWithoutExtension(std::string thingPath)
{
	std::string Path = getParentFromPath(thingPath);
	std::string Name = getStemFromPath(thingPath);
	std::string withoutExtension = combinePath(Path, Name);
	return withoutExtension;
}
