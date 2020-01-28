#include "GameObject.h"

GameObject::GameObject()
{
	m_pos = XMVectorSet(0,0,0,0);
}

void GameObject::update()
{
}

void GameObject::draw()
{
}

void GameObject::setPosition(XMVECTOR pos)
{
	m_pos = pos;
}

XMVECTOR GameObject::getPosition()
{
	return m_pos;
}
