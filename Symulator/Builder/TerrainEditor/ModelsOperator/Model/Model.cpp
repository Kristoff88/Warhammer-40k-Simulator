	#include "GlobalHeaders.h"

	#include "Model.h"


/****************************************************************/

	Model::Model  ():  vboIDv(0), vboIDvt(0), vboIDvn(0),
					   R(1.f), G(1.f), B(1.f), A(0.f)
	{}

	Model::~Model ()
	{}
//
/****************************************************************/

	int Model::GetIndexCount ()
	{
		int indexCount = 0;

		for(int i = 0; i < meshVector.size(); i++)
		{
			indexCount += meshVector[i].indexCounter;
		}

		return indexCount;
	}
//