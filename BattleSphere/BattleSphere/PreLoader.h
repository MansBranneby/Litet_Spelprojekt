#pragma once
#include "Model.h"

#include "Weapon.h"
#include "Robot.h"
#include "Node.h"
#include "Projectile.h"
#include "Resource.h"

#define OBJECT_TYPES 6
// objectType enum: drone, weapon, robot, node, projectile, resource


class PreLoader
{
private:
	// Preloaded models
	std::vector<int> m_nrOfmodels[OBJECT_TYPES];
	std::vector<Model*> m_objects[OBJECT_TYPES];

	void loadFromFile(objectType type, std::string filename); // Without .nyp file extension
	void setObjectData(objectType type, objectData data, int variant = 0);
	void setObjectData(objectType type, objectData data, objectData relativeData, int variant = 0);

public:
	PreLoader();
	~PreLoader();

	void draw(objectType type, objectData data, int variant = 0);
	void draw(objectType type, objectData data, objectData relativeData, int variant = 0);
};

