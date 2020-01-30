#include "PreLoader.h"

void PreLoader::loadFromFile(objectType type, std::string filename, int variant = 0)
{
	std::string line;
	std::istringstream inputStream;
	std::ifstream in;
	in.open("Models/" + filename + ".nyp", std::ios::binary | std::ios::in);

	std::getline(in, line);
	inputStream.str(line);
	inputStream >> m_nrOfmodels[type][variant]; // Get number of objects to read
	inputStream.clear();
	m_objects[type].push_back(new Model[m_nrOfmodels[type][variant]]);


	for (int i = 0; i < m_nrOfModels; i++) // Reads all objects
		m_models.loadModel(in);
}

PreLoader::PreLoader()
{
	m_objects.reserve(OBJECT_TYPES);
	m_nrOfmodels.reserve(OBJECT_TYPES);
}

PreLoader::~PreLoader()
{	
}
