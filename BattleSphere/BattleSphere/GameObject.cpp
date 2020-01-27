#include "GameObject.h"

GameObject::GameObject()
{
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
