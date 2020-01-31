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
	std::vector<std::vector<int>> m_nrOfmodels;
	std::vector<std::vector<Model*>> m_objects;

	void loadFromFile(objectType type, std::string filename); // Without .nyp file extension

	void setPosition(XMVECTOR pos, objectType type, int modelNr = 0);
	void setRotation(XMVECTOR rotation, , objectType type, int modelNr = 0);
	void setScale(XMVECTOR scale, objectType type, int modelNr = 0);
public:
	PreLoader();
	~PreLoader();

    void setObjectData(objectData data, objectType type, int objectVariant = 0);
    void draw(objectType type, int objectVariant = 0);
};

