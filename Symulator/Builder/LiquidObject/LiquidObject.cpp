	#include "GlobalHeaders.h"

	#include "LiquidObject.h"


	extern  PFNGLGENBUFFERSARBPROC	         glGenBuffersARB;           // VBO Name Generation Procedure
	extern  PFNGLBINDBUFFERARBPROC			 glBindBufferARB;           // VBO Bind Procedure
	extern  PFNGLBUFFERDATAARBPROC			 glBufferDataARB;           // VBO Data Loading Procedure
	extern  PFNGLBUFFERSUBDATAARBPROC		 glBufferSubDataARB;        // VBO Sub Data Loading Procedure
	extern  PFNGLDELETEBUFFERSARBPROC		 glDeleteBuffersARB;        // VBO Deletion Procedure
	extern  PFNGLGETBUFFERPARAMETERIVARBPROC glGetBufferParameterivARB; // return various parameters of VBO
	extern  PFNGLMAPBUFFERARBPROC			 glMapBufferARB;            // map VBO procedure
	extern  PFNGLUNMAPBUFFERARBPROC			 glUnmapBufferARB;    


	liquidObject::liquidObject(unsigned int ID, float alph, float min, float max, float waveLVL, float waveFREQ, float stretch, int dirX, int dirZ, float sped):
							 textureID(ID), alpha(alph), minLevel(min), maxLevel(max), speed(sped), waveLevel(waveLVL), waveFrequency(waveFREQ), waveStretch(stretch), Rising(true),

							 movementX(0), movementZ(0), directionX(dirX), directionZ(dirZ), indexArray(NULL), indexCounter(0),

							 VBOv(0), VBOvt(0), VBOvn(0), VBOvc(0)
	{}

	liquidObject::~liquidObject()
	{
		
		// czyszczenie VBO
			
			if( VBOv != 0 )
				glDeleteBuffersARB(1, &VBOv);

			if( VBOvt != 0 )
				glDeleteBuffersARB(1, &VBOvt);

			if( VBOvn != 0 )
				glDeleteBuffersARB(1, &VBOvn);

			if( VBOvc != 0 )
				glDeleteBuffersARB(1, &VBOvc);
	}