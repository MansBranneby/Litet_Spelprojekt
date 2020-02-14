#pragma once
#include "Projectile.h"

class ProjectileBank
{
private:
	ProjectileBank();
	static ProjectileBank* m_instance;

	std::vector<Projectile*> m_projectiles;

public:
	static ProjectileBank* getInstance();

	void addProjectile(XMVECTOR pos, XMVECTOR rot, XMVECTOR dir, int type, int damage);
	void removeProjectile(int index);
	void moveProjectiles(float dt);
	std::vector<Projectile*> getList();

	void release();
};
