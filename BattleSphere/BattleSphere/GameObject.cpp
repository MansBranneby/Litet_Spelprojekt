#include "GameObject.h"

GameObject::GameObject()
{
	m_position = XMVectorSet(0, 0, 0, 0);
	m_rotation = XMVectorSet(0, 0, 0, 0);
	m_scale = XMVectorSet(0, 0, 0, 0);
}

GameObject::~GameObject()
{
}

void GameObject::update()
{
}

void GameObject::draw()
{

}

void GameObject::setPosition(float x, float y, float z)
{
	XMVECTOR newPos = { x, y, z, 0 };
	m_position = newPos;
}

void GameObject::setPosition(XMVECTOR pos)
{
	m_position = pos;
}

void GameObject::move(float dX, float dY, float dZ)
{
	XMVECTOR dPos = { dX, dY, dZ, 0 };
	m_position += dPos;
}

void GameObject::move(XMVECTOR dPos)
{
	m_position += dPos;
}

void GameObject::setRotation(XMVECTOR rotation)
{
	m_rotation = rotation;
}

void GameObject::setRotation(float vx, float vy, float vz, float rotDeg)
{
	XMVECTOR rotation = { vx, vy, vz, rotDeg };
	m_rotation = rotation;
}

void GameObject::rotate(XMVECTOR dRotation)
{
	m_rotation += dRotation;
}

void GameObject::rotate(float vx, float vy, float vz, float dRotDeg)
{
	XMVECTOR dRotate = { vx, vy, vz, dRotDeg };
	m_rotation += dRotate;
}

void GameObject::setScale(XMVECTOR scale)
{
	m_scale = scale;
}

void GameObject::setScale(float xScale, float yScale, float zScale)
{
	XMVECTOR scale = { xScale, yScale, zScale, 0 };
	m_scale = scale;
}

void GameObject::scale(XMVECTOR scale)
{
	m_scale += scale;
}

void GameObject::scale(float xScale, float yScale, float zScale)
{
	XMVECTOR scale = { xScale, yScale, zScale, 0 };
	m_scale += scale;
}

XMVECTOR GameObject::getPosition()
{
	return m_position;
}