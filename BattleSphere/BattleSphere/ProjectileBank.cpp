#include "ProjectileBank.h"

ProjectileBank* ProjectileBank::m_instance = nullptr;

ProjectileBank::ProjectileBank()
{
}

ProjectileBank* ProjectileBank::getInstance()
{
	if (m_instance == nullptr)
		m_instance = new ProjectileBank();

	return m_instance;
}

void ProjectileBank::addProjectile(XMVECTOR pos, XMVECTOR rot, XMVECTOR dir, int type, int damage)
{
	Projectile* proj = new Projectile(pos, rot, dir, type, damage);
	m_projectiles.push_back(proj);
}

void ProjectileBank::removeProjectile(int index)
{
	delete m_projectiles[index];
	m_projectiles.erase(m_projectiles.begin() + index);
}

void ProjectileBank::moveProjectiles(float dt)
{
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		m_projectiles[i]->move(dt);
	}
}

void ProjectileBank::changeDirection(int index, XMVECTOR relPos)
{
	m_projectiles[index]->setDirection(relPos);
}

std::vector<Projectile*> ProjectileBank::getList()
{
	return m_projectiles;
}

void ProjectileBank::release()
{
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		delete m_projectiles[i];
	}
}
