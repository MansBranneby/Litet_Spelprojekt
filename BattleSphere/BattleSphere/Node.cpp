#include "Node.h"

Node::Node(int type)
{
	m_type = type;
	setScale(0.3f, 0.3f, 0.3f);
	setRotation(0,1,0,90);
	if (type == 1)
		m_material.diffuse = XMVectorSet(1, 0, 0, 0);
	else if (type == 2)
		m_material.diffuse = XMVectorSet(1, 1, 0, 0);
	else if (type == 3)
		m_material.diffuse = XMVectorSet(1, 0, 1, 0);
	else if (type == 4)
		m_material.diffuse = XMVectorSet(1, 1, 1, 0);
	else if (type == 5)
		m_material.diffuse = XMVectorSet(0.2f, 0.2f, 1, -1);
	else
		m_material.diffuse = XMVectorSet(0, 1, 1, 0);
}

Node::~Node()
{
}

int Node::getType()
{
	return m_type;
}
