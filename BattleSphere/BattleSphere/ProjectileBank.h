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

	void addProjectile(XMVECTOR pos, XMVECTOR colour, XMVECTOR rot, XMVECTOR dir, int type, int damage, float blastRange, int owner);
	void removeProjectile(int index);
	void moveProjectiles(float dt);
	void changeDirection(int index, XMVECTOR relPos, XMVECTOR colour, int owner);
	std::vector<Projectile*> getList();

	void release();
};
