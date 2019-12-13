#pragma once

#include <string>
#include <vector>

#include <winnt.h>

class FolderBrowser
{
public:
	static FolderBrowser& Singleton()
	{
		static FolderBrowser s;
		return s;
	}

	std::string GetApplicationPath();
	std::vector <std::string> GetFolderList(std::string locationPath);
	std::vector<std::string> GetFileList(std::string locationPath, std::string extension);

private:
	HANDLE& PassThroughParentDirectory(std::string const& locationPath);
	bool FoundItemIsFolder(WIN32_FIND_DATAA const& folderData);
	bool ReachedEndOfFolder(std::vector <std::string> const& folderList, WIN32_FIND_DATAA const& folderData);

	bool LocationPathDoesntHaveEndingSlash(std::string const& locationPath);
};