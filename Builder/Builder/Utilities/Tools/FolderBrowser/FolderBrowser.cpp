#include <fileapi.h>
#include <handleapi.h>
#include <minwinbase.h>
#include <minwindef.h>
#include <processenv.h>
#include <shellapi.h>

#include <GlobalHeaders.h>
#include "FolderBrowser.h"

std::string FolderBrowser::GetApplicationPath()
{
	int argCount;	// < -- This is just a filler. This function doesn't require argument count.
	LPWSTR* szArgList = CommandLineToArgvW((LPCWSTR)GetCommandLineA(), &argCount);

	std::string applicationPath;
	applicationPath.insert(0, (const char*)szArgList[0], 0, MAX_PATH);

	applicationPath.erase(applicationPath.rfind('\\') + 1, std::string::npos);  // usuniecie nazwy aplikacji
	applicationPath.erase(applicationPath.begin());

	return applicationPath;
}

std::vector <std::string> FolderBrowser::GetFolderList(std::string locationPath)
{
	std::vector <std::string> folderList;
	WIN32_FIND_DATAA folderData;

	if (LocationPathDoesntHaveEndingSlash(locationPath))
	{
		locationPath.push_back('\\');
	}

	HANDLE directory = PassThroughParentDirectory(locationPath);

	bool FoundFolder = false;

	do
	{
		FindNextFileA(directory, &folderData);

		if (FoundItemIsFolder(folderData))
		{
			if (ReachedEndOfFolder(folderList, folderData))
			{
				FoundFolder = false;
			}
			else
			{
				FoundFolder = true;
				folderList.push_back(folderData.cFileName);
			}
		}
		else
			FoundFolder = false;
	} while (FoundFolder);

	return folderList;
}

HANDLE FolderBrowser::PassThroughParentDirectory(std::string const& locationPath)
{
	HANDLE directory;
	WIN32_FIND_DATAA folderData;

	std::string firstFile;
	firstFile = locationPath;
	firstFile += "*.*";

	// Passing through "." i ".." (current and parent directory)
	directory = FindFirstFileA(firstFile.c_str(), &folderData);
	FindNextFileA(directory, &folderData);

	return directory;
}

bool FolderBrowser::FoundItemIsFolder(WIN32_FIND_DATAA const& folderData)
{
	return folderData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
}

bool FolderBrowser::ReachedEndOfFolder(std::vector <std::string> const& folderList, WIN32_FIND_DATAA const& folderData)
{
	return !folderList.empty() && strcmp(folderData.cFileName, folderList.back().c_str()) == 0;
}

std::vector <std::string> FolderBrowser::GetFileList(std::string locationPath, std::string extension)
{
	std::vector <std::string> fileList;
	HANDLE file;
	WIN32_FIND_DATAA fileData;

	if (LocationPathDoesntHaveEndingSlash(locationPath))
	{
		locationPath.push_back('\\');
	}

	file = FindFirstFileA((locationPath + extension).c_str(), &fileData);

	if (file != INVALID_HANDLE_VALUE)
	{
		std::string newFile;

		newFile.append(fileData.cFileName);
		fileList.push_back(newFile);
		newFile.clear();

		bool FoundFile = false;

		do
		{
			FoundFile = FindNextFileA(file, &fileData);

			if (FoundFile)
			{
				newFile.append(fileData.cFileName);
				fileList.push_back(newFile);
				newFile.clear();
			}
		} while (FoundFile);
	}

	return fileList;
}

bool FolderBrowser::LocationPathDoesntHaveEndingSlash(std::string const& locationPath)
{
	char lastCharacter = locationPath[locationPath.size() - 1];

	if (lastCharacter != '/' || lastCharacter != '\\')
	{
		return true;
	}
	else
		return false;
}