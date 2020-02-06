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
		m_objects[typ][variant][i].loadModel(in);
	in.close();

	objectData tempData;
	setModelData(type, tempData, variant);

	// If a valid collision file name was included, load the model, else place dummy model
	(collisionFilename == "") ? m_nrOfCMeshes[typ].push_back(0) : loadCollisionMesh(type, collisionFilename);
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
		m_cMesh[typ][variant][i].loadModel(in);
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
	// Load objects
	//loadFromFile(objectType::e_drone, "?");
	loadFromFile(objectType::e_weapon, "1mesh1mat");
	loadFromFile(objectType::e_robot, "BattleSphere", "2mesh2mat");
	loadFromFile(objectType::e_node, "Building");
	loadFromFile(objectType::e_projectile, "1mesh1mat");
	//loadFromFile(objectType::e_resource, "?");
	loadFromFile(objectType::e_scene, "SceneBig");
}

PreLoader::~PreLoader()
{	
	for (int i = 0; i < OBJECT_TYPES; i++)
	{
		for (int j = 0; j < m_objects[i].size(); j++)
			if (m_objects[i][j]) delete[] m_objects[i][j];
		for (int j = 0; j < m_cMesh[i].size(); j++)
			if (m_cMesh[i][j]) delete[] m_cMesh[i][j];
	}
}

XMFLOAT3* PreLoader::getCollisionMesh(objectType type, objectData data, int modelNr, int variant)
{
	int typ = (int)type;
	return m_objects[typ][variant][modelNr].getCollisionMesh(data);
}

XMFLOAT3* PreLoader::getCollisionMesh(objectType type, objectData data, objectData relativeData, int modelNr, int variant)
{
	int typ = (int)type;
	return m_objects[typ][variant][modelNr].getCollisionMesh(data, relativeData);
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

void PreLoader::draw(objectType type, objectData data, int variant)
{
	int typ = (int)type;
	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++)
	{
		m_objects[typ][variant][i].setObjectData(data);
		m_objects[typ][variant][i].draw();
	}
}

void PreLoader::draw(objectType type, objectData data, objectData relativeData, int variant)
{
	int typ = (int)type;
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
