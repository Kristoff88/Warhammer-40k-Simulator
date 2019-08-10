#pragma once

	#include "Mini//Mini.h"

	
	class MiniDataManager
	{
		public:

			static MiniDataManager &Singleton()
			{
				static MiniDataManager s;
				return s;
			}
			//

			bool Init ();
				private:
				inline bool LoadMiniModels ();

				inline bool LoadMinis      ();
				inline MiniModel_Data* GetModelData (std::string modelName) throw (std::string);
			public:

			void Close ();
				private:
				inline void ClearModelArrays ();
				inline void ClearMiniArrays  ();
			public:

	/****************************************************************/

			std::string GetModelsFolderPath ();
			std::string GetMinisFolderPath  ();

	/****************************************************************/

			int GetModelCount ();
 
			MiniModel_Data* GetModel (int const &index)			    throw (std::string);
			MiniModel_Data* GetModel (std::string const &modelName) throw (std::string);
				private:
				inline bool GivenModelIndexIsCorrect       (int const &index);
				inline void InformAboutIncorrectModelIndex (int const &index);
				inline void InformAboutIncorrectModelName  (std::string const &modelName);
			public:

	/****************************************************************/

			int GetMiniCount ();

			Mini* GetMini (int index)			   throw (std::string);
			Mini* GetMini (std::string miniName)   throw (std::string);

			void AddMini (std::string miniName)    throw (std::string);

			void RemoveMini (int index)			   throw (std::string);
			void RemoveMini (std::string miniName) throw (std::string);
				private:
				inline void PerformMiniRemovement (std::string filename);

				inline bool GivenMiniIndexIsCorrect       (int const &index);
				inline void InformAboutIncorrectMiniIndex (int const &index);
				inline void InformAboutIncorrectMiniName  (std::string const &modelName);

				inline bool GivenMiniNameIsDuplicate (std::string miniName);
				inline void InformAboutDuplicateName (std::string miniName);
			public:

		private:

			std::vector <MiniModel_Data*> modelArray;
			std::vector <Mini*> miniArray;


			MiniDataManager  ();
			~MiniDataManager ();
	};