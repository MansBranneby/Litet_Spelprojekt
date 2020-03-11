#pragma once
#include "StructsAndEnums.h"
#include "Robot.h"
#include "ConstantBuffer.h"

class SpawnDrone
{
private:
	// Resources
	std::vector<Resource*>* m_resources;

	// Resource spawning lists
	std::vector<XMFLOAT2> m_spawns;
	std::vector<bool> m_freeSpawns;
	void loadLists();
	
	// Icon warnings
	ConstantBuffer* m_constantBufferIcons;
	XMVECTOR m_nextSpawnWarning;

	// Spawning
	int m_normalSpawnAmount;
	int m_specialSpawnAmount;
	void startSpawn();
	int getSpawnIndex();
	int getSpecialSpawnIndex();

	// Drone resource spawning
	int m_droneLightIndex;
	int m_spawnDroneState;
	int m_heldResourceIndex;
	XMVECTOR m_transportDestination;
	XMVECTOR m_transportDirection;
	XMVECTOR m_travelTarget;
	XMVECTOR m_travelDirection;
	float m_collectedTime;
	bool m_spawnDroneTravelling;
	bool m_spawnDroneRotating;
	GameObject m_BSPDdoor;
	GameObject m_spawnDroneBody;
	GameObject m_spawnDronePropeller[4];
	void setTravelTarget(XMVECTOR target);
	void setRotationTarget(XMVECTOR target);
	bool travelAndCheck(float dT, bool fastTravel);
	bool assignMission(Robot** robots);
	bool translateDoor(float dt, bool open);

public:
	SpawnDrone(std::vector<Resource*>* m_resourcesPtr);
	~SpawnDrone();

	void update(Robot** robots, float dT);
	void freeSpawn(int index);
	void ifHeldDecreaseResourceIndex();
	objectData getData(int model = -1);
	void setConstantBuffer(bool on);
};

