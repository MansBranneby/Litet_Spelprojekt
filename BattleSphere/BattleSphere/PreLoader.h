#pragma once
#include "BackfaceCuller.h"
#include "Model.h"

#include "Weapon.h"
#include "Robot.h"
#include "Node.h"
#include "Projectile.h"
#include "Resource.h"

#define OBJECT_TYPES 7
// objectType enum: e_drone, e_weapon, e_robot, e_node, e_projectile, e_resource


class PreLoader
{
private:
	// Preloaded models
	std::vector<int> m_nrOfmodels[OBJECT_TYPES];
	std::vector<Model*> m_objects[OBJECT_TYPES];

	// Backface culler
	BackfaceCuller* m_bFCuller;

	void loadFromFile(objectType type, std::string filename); // Without .nyp file extension
	void setObjectData(objectType type, objectData data, int variant = 0);
	void setObjectData(objectType type, objectData data, objectData relativeData, int variant = 0);

public:
	PreLoader();
	~PreLoader();

	void setStaticData(objectType type, objectData data, int variant = 0); // Set position for static objects
	void draw(objectType type, int variant = 0);
	void draw(objectType type, objectData data, int variant = 0);
	void draw(objectType type, objectData data, objectData relativeData, int variant = 0);
	void cullDraw(objectType type, int variant = 0); // Draw static object and update index buffers so only front facing triangles are used.
};

