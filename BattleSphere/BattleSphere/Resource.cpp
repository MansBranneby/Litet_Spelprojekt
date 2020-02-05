#include "Resource.h"

Resource::Resource(int type)
{
	m_type = type;
	setScale(0.4f, 0.4f, 0.4f);
	if (type == 1)
		m_material.diffuse = XMVector3Normalize(XMVectorSet(113, 28, 145, 0));
	else if (type == 2)
		m_material.diffuse = XMVector3Normalize(XMVectorSet(234, 0, 217, 0));
	else if (type == 3)
		m_material.diffuse = XMVector3Normalize(XMVectorSet(10, 189, 198, 0));
	else
		m_material.diffuse = XMVector3Normalize(XMVectorSet(19, 62, 124, 0));
}

Resource::~Resource()
{
}

int Resource::getType()
{
	return m_type;
}