#include "PreLoader.h"

void PreLoader::loadFromFile(objectType type, std::string filename)
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
	objectData tempData;
	setObjectData(type, tempData, variant);
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

PreLoader::PreLoader()
{	
	// Load objects
	//loadFromFile(objectType::e_drone, "?");
	loadFromFile(objectType::e_weapon, "1mesh1mat");
	loadFromFile(objectType::e_robot, "BattleSphere");
	loadFromFile(objectType::e_node, "Building");
	loadFromFile(objectType::e_projectile, "1mesh1mat");
	//loadFromFile(objectType::e_resource, "?");
	loadFromFile(objectType::e_scene, "Scene");
}

PreLoader::~PreLoader()
{	
	for (int i = 0; i < OBJECT_TYPES; i++)
	{
		for (int j = 0; j < m_objects[i].size(); j++)
			if (m_objects[i][j]) delete[] m_objects[i][j];
	}
}


void PreLoader::setStaticData(objectType type, objectData data, int variant)
{
	int typ = (int)type;
	for (int i = 0; i < m_nrOfmodels[typ][variant]; i++)
		m_objects[typ][variant][i].setObjectData(data);
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
