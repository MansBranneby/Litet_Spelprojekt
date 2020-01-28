#include "GameObject.h"

GameObject::GameObject()
{
	m_nrOfModels = 0;
	m_models = nullptr;
}

GameObject::~GameObject()
{
	if (m_models) delete[] m_models;
}

void GameObject::loadFromFile(std::string filename)
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

	m_pos = XMVectorSet(0,0,0,0);
}

void GameObject::update()
{
}

void GameObject::draw()
{
	for (int i = 0; i < m_nrOfModels; i++)
	{
		m_models[i].draw();
	}
}

void GameObject::setPosition(XMVECTOR pos)
{
	m_pos = pos;
}

XMVECTOR GameObject::getPosition()
{
	return m_pos;
}
