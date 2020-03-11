#pragma once
#include "BackfaceCuller.h"
#include "Model.h"

class PreLoader
{
private:
	// Preloaded models
	std::vector<int> m_nrOfmodels[(int)objectType::e_nrOfEnums];
	std::vector<Model*> m_objects[(int)objectType::e_nrOfEnums];
	std::vector<int> m_nrOfCMeshes[(int)objectType::e_nrOfEnums];
	std::vector<Model*> m_cMesh[(int)objectType::e_nrOfEnums];

	// Backface culler
	BackfaceCuller* m_bFCuller;

	void loadFromFile(objectType type, std::string filename, std::string collisionFilename = ""); // Without .nyp file extension
	void setObjectData(objectType type, objectData data, int variant = 0);
	void setObjectData(objectType type, objectData data, objectData relativeData, int variant = 0);
	void loadCollisionMesh(objectType type, std::string filename); // Without .nyp file extension
	void setModelData(objectType type, objectData data, int variant = 0);
	void setModelData(objectType type, objectData data, objectData relativeData, int variant = 0);
	void setCMeshData(objectType type, objectData data, int variant = 0);
	void setCMeshData(objectType type, objectData data, objectData relativeData, int variant = 0);

public:
	PreLoader();
	~PreLoader();

	objectData getBVObjectData(ObjectType type, int modelNr, int variant = 0) const;
	boundingData getBoundingData(ObjectType type, int modelNr, int variant = 0) const;
	std::vector<XMFLOAT3> getCollisionMesh(ObjectType type, int modelNr, int variant = 0) const;
	std::vector<XMFLOAT3> getCollisionMesh(ObjectType type, objectData data, int modelNr, int variant = 0) const;
	std::vector<XMFLOAT3> getCollisionMesh(ObjectType type, objectData data, objectData relativeData, int modelNr, int variant = 0) const;
	BillboardData getSubModelBillboardData(ObjectType type, int variant, int modelNr, int subModelNr);
	int getNrOfVariants(ObjectType type) const;
	int getNrOfModels(ObjectType type, int variant) const;
	int getNrOfSubModels(ObjectType type, int variant, int modelNr) const;

	void setStaticData(objectType type, objectData data, int variant = 0); // Set position for static objects

	void drawCM(objectType type, int variant = 0);
	void drawCM(objectType type, objectData data, int variant = 0);
	void drawCM(objectType type, objectData data, objectData relativeData, int variant = 0);
	void draw(objectType type, int variant = 0);
	void draw(objectType type, objectData data, int modelNr = -1, int subModelNr = -1, int variant = 0);
	void draw(objectType type, objectData data, objectData relativeData, int modelNr = -1, int subModelNr = -1, int variant = 0, bool leftMaterial = true);
	void draw(objectType type, BillboardData billboardData, int modelNr, int subModelNr, int variant);
	void drawOneMaterial(objectType type, objectData data, int variant = 0);
	void drawOneMaterial(objectType type, objectData data, objectData relativeData, int variant = 0);
	void drawOneModel(objectType type, objectData data, int modelNr, int variant = 0);
	void drawOneModel(objectType type, objectData data, objectData relativeData, int modelNr, int variant = 0);
	void drawOneModelAndMat(objectType type, objectData data, int modelNr, int variant = 0);
	void drawOneModelAndMat(objectType type, objectData data, objectData relativeData, int modelNr, int variant = 0);
	void setSubModelData(objectType type, objectData data, int modelNr, int subModelNr, int variant = 0);
	void cull(objectType type, int variant = 0); // Draw static object and update index buffers so only front facing triangles are used.
};

