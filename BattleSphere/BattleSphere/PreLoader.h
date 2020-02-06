#pragma once
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
	std::vector<int> m_nrOfCMeshes[OBJECT_TYPES];
	std::vector<Model*> m_cMesh[OBJECT_TYPES];

	void loadFromFile(objectType type, std::string filename, std::string collisionFilename = ""); // Without .nyp file extension
	void loadCollisionMesh(objectType type, std::string filename); // Without .nyp file extension
	void setModelData(objectType type, objectData data, int variant = 0);
	void setModelData(objectType type, objectData data, objectData relativeData, int variant = 0);
	void setCMeshData(objectType type, objectData data, int variant = 0);
	void setCMeshData(objectType type, objectData data, objectData relativeData, int variant = 0);

public:
	PreLoader();
	~PreLoader();

	XMFLOAT3* getCollisionMesh(objectType type, objectData data, int modelNr, int variant = 0);
	XMFLOAT3* getCollisionMesh(objectType type, objectData data, objectData relativeData, int modelNr, int variant = 0);
	
	void setStaticData(objectType type, objectData data, int variant = 0); // Set position for static objects
	
	void draw(objectType type, int variant = 0);
	void draw(objectType type, objectData data, int variant = 0);
	void draw(objectType type, objectData data, objectData relativeData, int variant = 0);
	void drawCM(objectType type, int variant = 0);
	void drawCM(objectType type, objectData data, int variant = 0);
	void drawCM(objectType type, objectData data, objectData relativeData, int variant = 0);
};

