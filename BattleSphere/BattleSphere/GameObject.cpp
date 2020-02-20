#include "GameObject.h"

GameObject::GameObject()
{
	m_position = XMVectorSet(0, 0, 0, 0);
	m_rotation = XMVectorSet(1, 0, 0, 0);
	m_scale = XMVectorSet(1, 1, 1, 0);

	m_material.ambient = XMVectorSet(-1, -1, -1, -1);
	m_material.diffuse = XMVectorSet(-1, -1, -1, -1);
	m_material.specular = XMVectorSet(-1, -1, -1, -1);
	m_material.emission = XMVectorSet(-1, -1, -1, -1);

	m_isDrawn = true;
}

GameObject::~GameObject()
{
}

void GameObject::update()
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
	m_rotation.m128_f32[0] = vx;
	m_rotation.m128_f32[1] = vy;
	m_rotation.m128_f32[2] = vz;
	m_rotation.m128_f32[3] += dRotDeg;
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

void GameObject::setAmbiet(float x, float y, float z, float w)
{
	m_material.ambient = XMVectorSet(x, y, z, w);
}

void GameObject::setDiffuse(float x, float y, float z, float w)
{
	m_material.diffuse = XMVectorSet(x, y, z, w);
}

void GameObject::setSpecular(float x, float y, float z, float w)
{
	m_material.specular = XMVectorSet(x, y, z, w);
}

void GameObject::setEmission(float x, float y, float z, float w)
{
	m_material.emission = XMVectorSet(x, y, z, w);
}

XMVECTOR GameObject::getPosition()
{
	return m_position;
}

objectData const GameObject::getData()
{
	objectData temp = { m_position, m_rotation, m_scale, m_material };
	return temp;
}

bool GameObject::isDrawn()
{
	return m_isDrawn;
}

void GameObject::setDrawn(bool isDrawn)
{
	m_isDrawn = isDrawn;
}
