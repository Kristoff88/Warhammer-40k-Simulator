#pragma once


	#include "ObjectOperator//ObjectOperator.h"
	ObjectOperator &objectOperator = ObjectOperator::Singleton();

	#include "TerrainEditor//ModelsOperator//ModelsOperator.h"
	ModelsOperator &modelsOperator = ModelsOperator::Singleton();

	#include "Cubemap.h"
	#include "Clouds.h"
	#include "Lights.h"
	#include "Terrain.h"
	#include "Details.h"
	#include "Liquids.h"
	#include "Grass.h"
