#pragma once
#include "Model.h"

#include "Weapon.h"
#include "Robot.h"
#include "Node.h"
#include "Projectile.h"
#include "Resource.h"


class PreLoader
{
private: 
	// Preloaded models
	int m_nrOfModels;
	Model* m_models;

	// GameObjects
	std::vector<Weapon> m_weapons;
	std::vector<Robot> m_robots;
	std::vector<Node> m_nodes;
	std::vector<Projectile> m_Projectiles;
	std::vector<Resource> m_Resources;

	void loadFromFile(std::string filename); // Without .nyp file extension

public:
	PreLoader();
	~PreLoader();
	

};

