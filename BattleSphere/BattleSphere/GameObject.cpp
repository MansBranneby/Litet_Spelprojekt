#include "GameObject.h"

GameObject::GameObject()
{
	m_nrOfModels = 0;
	m_models = nullptr;
}

GameObject::GameObject(std::string filename)
{
	m_nrOfModels = 0;
	m_models = nullptr;
	loadFromFile(filename);
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
}

void GameObject::update()
{
}

void GameObject::draw()
{
	for (int i = 0; i < m_nrOfModels; i++)
		m_models[i].draw();
}

void GameObject::setPosition(float x, float y, float z)
{
	XMVECTOR newPos = { x, y, z, 0 };
	for (int i = 0; i < m_nrOfModels; i++)
		m_models[i].setPosition(newPos);
}

void GameObject::setPosition(XMVECTOR pos)
{
	for (int i = 0; i < m_nrOfModels; i++)
		m_models[i].setPosition(pos);
}

void GameObject::move(XMVECTOR dPos)
{
	for (int i = 0; i < m_nrOfModels; i++)
		m_models[i].move(dPos);
}

void GameObject::move(float dX, float dY, float dZ)
{
	XMVECTOR dPos = { dX, dY, dZ, 0 };
	for (int i = 0; i < m_nrOfModels; i++)
		m_models[i].move(dPos);
}

void GameObject::rotate(float vx, float vy, float vz, float rotDeg)
{
	for (int i = 0; i < m_nrOfModels; i++)
		m_models[i].rotate(vx, vy, vz, rotDeg);
}

void GameObject::scale(float xScale, float yScale, float zScale)
{
	for (int i = 0; i < m_nrOfModels; i++)
		m_models[i].scale(xScale, yScale, zScale);
}
