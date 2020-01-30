#include "PreLoader.h"

void PreLoader::loadFromFile(std::string filename)
{
	std::string line;
	std::istringstream inputStream;
	std::ifstream in;
	in.open("Models/" + filename + ".nyp", std::ios::binary | std::ios::in);

	std::getline(in, line);
	inputStream.str(line);
	inputStream >> m_nrOfModels; // Get number of objects to read
	inputStream.clear();
	m_models = new Model[m_nrOfModels];


	for (int i = 0; i < m_nrOfModels; i++) // Reads all objects
		m_models[i].loadModel(in);
}

PreLoader::PreLoader()
{
	m_nrOfModels = 0;
	m_models = nullptr;
}

PreLoader::~PreLoader()
{
	if (m_models) delete[] m_models;
}
