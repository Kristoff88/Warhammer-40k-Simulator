#pragma once


	#include "ObjectOperator.h"
	ObjectOperator &objectOperator = ObjectOperator::Singleton();


	#include "CameraOperator.h"

	#include "TerrainEditor//ModelsOperator//ModelsOperator.h"
	ModelsOperator &modelsOperator = ModelsOperator::Singleton();

	#include "Cubemap.h"
	#include "Clouds.h"
	#include "Lights.h"
	#include "Terrain.h"
	#include "Details.h"
	#include "Liquids.h"
	#include "Grass.h"

	#include "Cursor.h"

	#include "TextOperator.h"
	#include "MenuOperator.h"
