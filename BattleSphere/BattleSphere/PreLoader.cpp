#include "PreLoader.h"

void PreLoader::loadFromFile(objectType type, std::string filename, std::string collisionFilename)
{
	int typ = (int)type;
	std::string line;
	std::istringstream inputStream;
	std::ifstream in;
	in.open("Models/" + filename + ".nyp", std::ios::binary | std::ios::in);

	int variant = (int)m_objects[typ].size();
	int nrOfModels = 0;
	std::getline(in, line);
	inputStream.str(line);
	inputStream >> nrOfModels; // Get number of objects to read
	m_nrOfmodels[typ].push_back(nrOfModels);
	inputStream.clear();
	m_objects[typ].push_back(new Model[m_nrOfmodels[typ][variant]]);


	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++) // Reads all objects
		m_objects[typ][variant][i].loadModel(in, type);
	in.close();

	objectData tempData;
	setModelData(type, tempData, variant);

	// If a valid collision file name was included, load the model, else place dummy model
	(collisionFilename == "") ? m_nrOfCMeshes[typ].push_back(0) : loadCollisionMesh(type, collisionFilename);
}

void PreLoader::setObjectData(objectType type, objectData data, int variant)
{
	int typ = (int)type;
	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++)
		m_objects[typ][variant][i].setObjectData(data);
}

void PreLoader::setObjectData(objectType type, objectData data, objectData relativeData, int variant)
{
	int typ = (int)type;
	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++)
		m_objects[typ][variant][i].setObjectData(data, relativeData);
}

void PreLoader::loadCollisionMesh(objectType type, std::string filename)
{
	int typ = (int)type;
	std::string line;
	std::istringstream inputStream;
	std::ifstream in;
	in.open("Collision Meshes/" + filename + ".nyp", std::ios::binary | std::ios::in);

	int variant = (int)m_cMesh[typ].size();
	int nrOfModels = 0;
	std::getline(in, line);
	inputStream.str(line);
	inputStream >> nrOfModels; // Get number of objects to read
	m_nrOfCMeshes[typ].push_back(nrOfModels);
	inputStream.clear();
	m_cMesh[typ].push_back(new Model[m_nrOfCMeshes[typ][variant]]);


	for (int i = 0; i < m_nrOfCMeshes[typ][variant]; i++) // Reads all objects
		m_cMesh[typ][variant][i].loadModel(in, type);
	in.close();
	objectData tempData;
	setCMeshData(type, tempData, variant);
}

void PreLoader::setModelData(objectType type, objectData data, int variant)
{
	int typ = (int)type;
	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++)
		m_objects[typ][variant][i].setObjectData(data);
}

void PreLoader::setModelData(objectType type, objectData data, objectData relativeData, int variant)
{
	int typ = (int)type;
	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++)
		m_objects[typ][variant][i].setObjectData(data, relativeData);
}

void PreLoader::setCMeshData(objectType type, objectData data, int variant)
{
	int typ = (int)type;
	for (int i = 0; i < m_nrOfCMeshes[typ][variant]; i++)
		m_cMesh[typ][variant][i].setObjectData(data);
}

void PreLoader::setCMeshData(objectType type, objectData data, objectData relativeData, int variant)
{
	int typ = (int)type;
	for (int i = 0; i < m_nrOfCMeshes[typ][variant]; i++)
		m_cMesh[typ][variant][i].setObjectData(data, relativeData);
}

PreLoader::PreLoader()
{	
	// Initialize backface culler
	m_bFCuller = new BackfaceCuller;

	// Load objects
	loadFromFile(objectType::e_drone, "Drone");
	loadFromFile(objectType::e_weapon, "GamePlay\\Weapon1");
	loadFromFile(objectType::e_resource, "GamePlay\\Weapon1");
	loadFromFile(objectType::e_robot, "GamePlay\\BattleSphere", "BattleSphere");
	loadFromFile(objectType::e_node, "OnMap\\NodeGoldenDuck(Node)", "NodeGoldenDuck(Node)Coll");
	loadFromFile(objectType::e_projectile, "1mesh1mat", "1mesh1mat");

	// On map (with collision)
	loadFromFile(objectType::e_static, "OnMap\\Bar", "BarColl");
	loadFromFile(objectType::e_static, "OnMap\\Wall", "WallColl");
	loadFromFile(objectType::e_static, "OnMap\\GasStation", "GasStationColl");
	loadFromFile(objectType::e_static, "OnMap\\HeadLights", "HeadLightsColl");
	loadFromFile(objectType::e_static, "Background\\Edge", "EdgeColl");
		// Nodes
	loadFromFile(objectType::e_static, "OnMap\\NodeHotel", "NodeHotelColl");

		// Static Background (Skyscrapers with collision) // Later billboards?
	loadFromFile(objectType::e_static, "Background\\BSTower", "BSTowerColl");
	loadFromFile(objectType::e_static, "Background\\SS1", "SS1Coll");
	loadFromFile(objectType::e_static, "Background\\SS2", "SS2Coll");
	loadFromFile(objectType::e_static, "Background\\SS3", "SS3Coll");
	loadFromFile(objectType::e_static, "Background\\SS4", "SS4Coll");


	// Background
	loadFromFile(objectType::e_scene, "OnMap\\ChinaTown"); // Later to static
	loadFromFile(objectType::e_scene, "OnMap\\BigWall");
	loadFromFile(objectType::e_scene, "OnMap\\NodeGoldenDuck");

	loadFromFile(objectType::e_scene, "Background\\Ground");
	//loadFromFile(objectType::e_scene, "Background\\RoadPaint"); // Maybe bad looking?
	loadFromFile(objectType::e_scene, "Background\\Freeway");
	loadFromFile(objectType::e_scene, "Other\\Car");
	loadFromFile(objectType::e_extra, "Other\\CarDBO");
	//loadFromFile(objectType::e_extra, "1mesh1mat"); // Delete
}

PreLoader::~PreLoader()
{	
	if (m_bFCuller) delete m_bFCuller;

	for (int i = 0; i < OBJECT_TYPES; i++)
	{
		for (int j = 0; j < m_objects[i].size(); j++)
			if (m_objects[i][j]) delete[] m_objects[i][j];
		for (int j = 0; j < m_cMesh[i].size(); j++)
			if (m_cMesh[i][j]) delete[] m_cMesh[i][j];
	}
}

objectData PreLoader::getBVObjectData(objectType type, int modelNr, int variant) const
{
	int typ = (int)type;
	return m_cMesh[typ][variant][modelNr].getBVObjectData();
}

boundingData PreLoader::getBoundingData(objectType type, int modelNr, int variant) const
{
	int typ = (int)type;
	return m_cMesh[typ][variant][modelNr].getBoundingData();
}

std::vector<XMFLOAT3> PreLoader::getCollisionMesh(objectType type, int modelNr, int variant) const
{
	int typ = (int)type;
	return m_cMesh[typ][variant][modelNr].getCollisionMesh();
}

std::vector<XMFLOAT3> PreLoader::getCollisionMesh(objectType type, objectData data, int modelNr, int variant) const
{
	int typ = (int)type;
	return m_cMesh[typ][variant][modelNr].getCollisionMesh(data);
}

std::vector<XMFLOAT3> PreLoader::getCollisionMesh(objectType type, objectData data, objectData relativeData, int modelNr, int variant) const
{
	int typ = (int)type;
	return m_cMesh[typ][variant][modelNr].getCollisionMesh(data, relativeData);
}

int PreLoader::getNrOfVariants(objectType type) const
{
	return (int)m_objects[(int)type].size();
}

int PreLoader::getNrOfModels(objectType type, int variants) const
{
	return m_nrOfmodels[(int)type][variants];
}

void PreLoader::setStaticData(objectType type, objectData data, int variant)
{
	int typ = (int)type;
	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++)
	{
		m_objects[typ][variant][i].setObjectData(data);
	}
	for (int i = 0; i < m_nrOfCMeshes[typ][variant]; i++)
	{
		m_cMesh[typ][variant][i].setObjectData(data);
	}
}

void PreLoader::draw(objectType type, int variant)
{
	int typ = (int)type;
	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++)
		m_objects[typ][variant][i].draw();
}

void PreLoader::draw(objectType type, objectData data, int modelNr, int subModelNr, int variant)
{
	int typ = (int)type;
	if (modelNr != -1) 
		m_objects[typ][variant][modelNr].setObjectData(data, subModelNr);
	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++)
	{
		m_objects[typ][variant][i].setObjectData(data);
		m_objects[typ][variant][i].draw();
	}
}

void PreLoader::draw(objectType type, objectData data, objectData relativeData, int modelNr, int subModelNr, int variant)
{
	int typ = (int)type;
	if (modelNr != -1)
		m_objects[typ][variant][modelNr].setObjectData(data, relativeData, subModelNr);
	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++)
	{
		m_objects[typ][variant][i].setObjectData(data, relativeData);
		m_objects[typ][variant][i].draw();
	}
}

void PreLoader::drawCM(objectType type, int variant)
{
	int typ = (int)type;
	for (int i = 0; i < m_nrOfCMeshes[typ][variant]; i++)
		m_cMesh[typ][variant][i].draw();
}

void PreLoader::drawCM(objectType type, objectData data, int variant)
{
	int typ = (int)type;
	for (int i = 0; i < m_nrOfCMeshes[typ][variant]; i++)
	{
		m_cMesh[typ][variant][i].setObjectData(data);
		m_cMesh[typ][variant][i].draw();
	}
}

void PreLoader::drawCM(objectType type, objectData data, objectData relativeData, int variant)
{
	int typ = (int)type;
	std::vector<int> a = m_nrOfCMeshes[typ];
	for (int i = 0; i < m_nrOfCMeshes[typ][variant]; i++)
	{
		m_cMesh[typ][variant][i].setObjectData(data, relativeData);
		m_cMesh[typ][variant][i].draw();
	}
}

void PreLoader::drawOneMaterial(objectType type, objectData data, int variant)
{
	int typ = (int)type;
	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++)
	{
		m_objects[typ][variant][i].setAllObjectData(data);
	}
	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++)
	{
		m_objects[typ][variant][i].draw();
	}
}

void PreLoader::drawOneMaterial(objectType type, objectData data, objectData relativeData, int variant)
{
	int typ = (int)type;
	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++)
	{
		m_objects[typ][variant][i].setAllObjectData(data, relativeData);
	}
	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++)
	{
		m_objects[typ][variant][i].draw();
	}
}

void PreLoader::drawOneModel(objectType type, objectData data, int modelNr, int variant)
{
	int typ = (int)type;
		m_objects[typ][variant][modelNr].setObjectData(data);
		m_objects[typ][variant][modelNr].draw();
}

void PreLoader::drawOneModel(objectType type, objectData data, objectData relativeData, int modelNr, int variant)
{
	int typ = (int)type;
	m_objects[typ][variant][modelNr].setObjectData(data, relativeData);
	m_objects[typ][variant][modelNr].draw();
}

void PreLoader::drawOneModelAndMat(objectType type, objectData data, int modelNr, int variant)
{
	int typ = (int)type;
	m_objects[typ][variant][modelNr].setAllObjectData(data);
	m_objects[typ][variant][modelNr].draw();
}

void PreLoader::drawOneModelAndMat(objectType type, objectData data, objectData relativeData, int modelNr, int variant)
{
	int typ = (int)type;
	m_objects[typ][variant][modelNr].setAllObjectData(data, relativeData);
	m_objects[typ][variant][modelNr].draw();
}

void PreLoader::cull(objectType type, int variant)
{
	m_bFCuller->turnOnCullingPipeline();
	int typ = (int)type;
	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++)
		m_objects[typ][variant][i].cullDraw();
	m_bFCuller->turnOffCullingPipeline();
}
