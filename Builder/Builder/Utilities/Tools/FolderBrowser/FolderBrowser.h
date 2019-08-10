#pragma once



	class FolderBrowser
	{
		public:

			static FolderBrowser& Singleton()
			{
				static FolderBrowser s;
				return s;
			}
			//

			std::string GetApplicationPath ();


			std::vector <std::string> GetFolderList (std::string locationPath);
				private:
				inline HANDLE& PassThroughParentDirectory (std::string const &locationPath);

				inline bool FoundItemIsFolder  (WIN32_FIND_DATAA const &folderData);
				inline bool ReachedEndOfFolder (std::vector <std::string> const &folderList, WIN32_FIND_DATAA const &folderData);
			public:


			std::vector <std::string> GetFileList (std::string locationPath, std::string extension);


			private:

				inline bool LocationPathDoesntHaveEndingSlash (std::string const &locationPath);
	};