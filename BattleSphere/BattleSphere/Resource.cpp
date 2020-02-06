#include "Resource.h"

Resource::Resource(int type)
{
	m_type = type;
	setScale(0.4f, 0.4f, 0.4f);
	if (type == 1)
		m_material.diffuse = XMVectorSet(1, 0, 0, 0);
	else if (type == 2)
		m_material.diffuse = XMVectorSet(1, 1, 0, 0);
	else if (type == 3)
		m_material.diffuse = XMVectorSet(1, 0, 1, 0);
	else
		m_material.diffuse = XMVectorSet(0, 1, 1, 0);
}

Resource::~Resource()
{
}

int Resource::getType()
{
	return m_type;
}