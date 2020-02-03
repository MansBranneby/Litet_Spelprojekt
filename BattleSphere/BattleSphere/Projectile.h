#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>

#include "DX.h"
#include "GameObject.h"

using namespace DirectX;

#define PISTOL 0
#define RIFLE 1


class Projectile : public GameObject
{
private:
	int m_type;
	int m_damage;
	float m_velocity;
	XMVECTOR m_direction;

public:
	Projectile(int type, int damage);

	void setDamage(int damage);
	void setDirection(XMVECTOR direction);

	int getType();
	int getDamage();
	float getVelocity();
	XMVECTOR getDirection();
};