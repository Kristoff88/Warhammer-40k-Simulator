	#include "GlobalHeaders.h"

	#include "MiniDataManager.h"


	#define MODELS_FOLDER "MiniData\\Models\\"
	#define MINIS_FOLDER  "MiniData\\Minis\\"



	bool MiniDataManager::Init ()
	{
		try
		{
			if( LoadMiniModels() && LoadMinis() )
			{
				return true;
			}
			else
				return false;
		}
		catch(std::string error)
		{
			MessageBox(NULL, error.c_str(), "MiniDataManager initialization error.", MB_OK);
			return false;
		}
	}
//
		bool MiniDataManager::LoadMiniModels ()
		{
			std::vector <std::string> folders, files;
			folders = FolderBrowser::Singleton().GetFolderList(MODELS_FOLDER);


			for(int i = 0; i < folders.size(); i++)
			{
				files = FolderBrowser::Singleton().GetFileList(MODELS_FOLDER + folders[i]+"\\", "*.obj");

				for(int j = 0; j < files.size(); j++)
				{
					try
					{
						std::string locationPath = GetModelsFolderPath();
						locationPath += folders[i];
						locationPath += "\\";


						OBJstruct *newOBJ = OBJ_Loader::Singleton().LoadOBJFile( (locationPath+files[j]).c_str() );
						MiniModel_Data *newModel = new MiniModel_Data( StripName(files[j], false) );

						
						newModel->InsertModelData(newOBJ, locationPath);
						modelArray.push_back(newModel);
					}
					catch(std::string error)
					{
						MessageBox(NULL, error.c_str(), "Loading mini model.", MB_OK);
						return false;
					}
				}
			}

			return true;
		}
//
		bool MiniDataManager::LoadMinis      ()
		{
			std::vector <std::string> files;
			files = FolderBrowser::Singleton().GetFileList(MINIS_FOLDER +std::string("\\"), "*.mini");


			for(int i = 0; i < files.size(); i++)
			{
				try
				{
					Mini *newMini = new Mini( StripName(files[i], false) );

					char locationPath[MAX_PATH];

					strcpy_s( locationPath, MAX_PATH, GetMinisFolderPath().c_str() );
					strcat_s(locationPath, MAX_PATH, files[i].c_str() );

					newMini->Load(locationPath);
					newMini->CopyModelData( GetModelData(newMini->GetModelName()) );

					miniArray.push_back(newMini);
				}
				catch(std::string error)
				{
					MessageBox(NULL, error.c_str(), "Loading mini model.", MB_OK);
					return false;
				}
			}

			return true;
		}

		MiniModel_Data* MiniDataManager::GetModelData (std::string modelName) throw (std::string)
		{
			for(int i = 0; i < modelArray.size(); i++)
			{
				if( modelArray[i]->GetModelName() == modelName )
				{
					return modelArray[i];					
				}
			}


			std::string error;

			error  = "Model \"";
			error += modelName;
			error += "\" hasn't been found in modelArray.";

			throw error;
		}
//
	void MiniDataManager::Close ()
	{
		MiniModel_Data::ClearStaticTextureVector();

		ClearModelArrays();
		ClearMiniArrays();
	}
//
		void MiniDataManager::ClearModelArrays ()
		{
			for(int i = 0; i < modelArray.size(); i++)
			{
				delete modelArray[i];
			}

			modelArray.clear();
		}

		void MiniDataManager::ClearMiniArrays  ()
		{
			for(int i = 0; i < miniArray.size(); i++)
			{
				delete miniArray[i];
			}

			miniArray.clear();
		}
//
/****************************************************************/

	std::string MiniDataManager::GetModelsFolderPath ()
	{
		std::string locationPath;

		locationPath  = FolderBrowser::Singleton().GetApplicationPath();
		locationPath += MODELS_FOLDER;

		return locationPath;
	}

	std::string MiniDataManager::GetMinisFolderPath  ()
	{
		std::string locationPath;

		locationPath  = FolderBrowser::Singleton().GetApplicationPath();
		locationPath += MINIS_FOLDER;

		return locationPath;
	}
//
/****************************************************************/
	
	int MiniDataManager::GetModelCount ()
	{
		return modelArray.size();
	}
 //
	MiniModel_Data* MiniDataManager::GetModel (int const &index)			throw (std::string)
	{
		if( GivenModelIndexIsCorrect(index) )
		{
			return modelArray[index];
		}
		else
			InformAboutIncorrectModelIndex(index);
	}

	MiniModel_Data* MiniDataManager::GetModel (std::string const &modelName) throw (std::string)
	{
		for(int i = 0; i < modelArray.size(); i++)
		{
			if( modelArray[i]->GetModelName() == modelName )
			{
				return modelArray[i];
				break;
			}
		}

		InformAboutIncorrectModelName(modelName);
	}
//
		bool MiniDataManager::GivenModelIndexIsCorrect       (int const &index)
		{
			if( index < 0 || index >= modelArray.size() )
			{
				return false;
			}
			else
				return true;
		}

		void MiniDataManager::InformAboutIncorrectModelIndex (int const &index)
		{
			std::string error;

			error  = "Given model index \"";
			error += index;
			error += "\" is incorrect.";

			throw error;
		}

		void MiniDataManager::InformAboutIncorrectModelName (std::string const &modelName)
		{
			std::string error;

			error  = "Given model name \"";
			error += modelName;
			error += "\" is incorrect.";

			throw error;
		}
//
/****************************************************************/

	int MiniDataManager::GetMiniCount ()
	{
		return miniArray.size();
	}
//
	Mini* MiniDataManager::GetMini (int index)			    throw (std::string)
	{
		if( GivenMiniIndexIsCorrect(index) )
		{
			return miniArray[index];
		}
		else
			InformAboutIncorrectMiniIndex(index);
	}

	Mini* MiniDataManager::GetMini (std::string miniName)   throw (std::string)
	{
		for(int i = 0; i < miniArray.size(); i++)
		{
			if( miniArray[i]->GetMiniName() == miniName )
			{
				return miniArray[i];
			}
		}

		InformAboutIncorrectMiniName(miniName);
	}
//
	void MiniDataManager::AddMini (std::string miniName)    throw (std::string)
	{
		if( GivenMiniNameIsDuplicate(miniName) )
		{
			InformAboutDuplicateName(miniName);
		}
		else
		{
			miniArray.push_back( new Mini(miniName) );
		}
	}
//
	void MiniDataManager::RemoveMini (int index)			throw (std::string)
	{
		if( GivenMiniIndexIsCorrect(index) )
		{
			PerformMiniRemovement( miniArray[index]->GetMiniName() );
		}
		else
			InformAboutIncorrectMiniIndex(index);
	}

	void MiniDataManager::RemoveMini (std::string miniName) throw (std::string)
	{
		for(int i = 0; i < miniArray.size(); i++)
		{
			if( miniArray[i]->GetMiniName() == miniName )
			{
				PerformMiniRemovement(miniName);
				miniArray.erase( miniArray.begin()+i );
				return;
			}
		}
		
		InformAboutIncorrectMiniName(miniName);
	}
//
		void MiniDataManager::PerformMiniRemovement (std::string filename)
		{
			char locationPath[MAX_PATH];

			strcpy_s(locationPath, MAX_PATH, FolderBrowser::Singleton().GetApplicationPath().c_str() );
			strcat_s(locationPath, MAX_PATH, MINIS_FOLDER );
			strcat_s(locationPath, MAX_PATH, "\\" );
			strcat_s(locationPath, MAX_PATH, filename.c_str() );
			strcat_s(locationPath, MAX_PATH, ".mini" );

			remove(locationPath);
		}

		bool MiniDataManager::GivenMiniIndexIsCorrect       (int const &index)
		{
			if( index < 0 || index >= miniArray.size() )
			{
				return false;
			}
			else
				return true;
		}

		void MiniDataManager::InformAboutIncorrectMiniIndex (int const &index)
		{
			std::string error;

			error  = "Given mini index \"";
			error += index;
			error += "\" is incorrect.";

			throw error;
		}

		void MiniDataManager::InformAboutIncorrectMiniName  (std::string const &modelName)
		{
			std::string error;

			error  = "Given mini name \"";
			error += modelName;
			error += "\" is incorrect.";

			throw error;
		}
//
		bool MiniDataManager::GivenMiniNameIsDuplicate (std::string miniName)
		{
			bool FoundDuplicate = false;

			for(int i = 0; i < miniArray.size(); i++)
			{
				if( miniArray[i]->GetMiniName() == miniName )
				{
					FoundDuplicate = true;
					break;
				}
			}

			return FoundDuplicate;
		}

		void MiniDataManager::InformAboutDuplicateName (std::string miniName)
		{
			std::string error;

			error  = "Given mini name \"";
			error += miniName;
			error += "\" is a duplicate.";

			throw error;
		}
//
/****************************************************************/

	MiniDataManager::MiniDataManager  ()
	{}

	MiniDataManager::~MiniDataManager ()
	{}
//
//

	#undef MODELS_FOLDER
	#undef MINIS_FOLDER