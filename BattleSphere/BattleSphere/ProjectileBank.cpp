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

void ProjectileBank::addProjectile(XMVECTOR pos, XMVECTOR colour, XMVECTOR rot, XMVECTOR dir, int type, float damage, float blastRange, int owner)
{
	Projectile* proj = new Projectile(pos, colour, rot, dir, type, damage, blastRange, owner);
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
		if (m_projectiles[i]->move(dt)) {
			removeProjectile(i);
		}
	}
}

void ProjectileBank::changeDirection(int index, XMVECTOR relPos, XMVECTOR colour, int owner)
{
	m_projectiles[index]->setDirection(relPos, colour, owner);
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
