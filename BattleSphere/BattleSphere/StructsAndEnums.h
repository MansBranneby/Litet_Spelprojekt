#pragma once
#include "DX.h"
#include <DirectXMath.h>
#include "ConstantBuffer.h"
#include "Lights.h"
#include <vector>
#include <cmath>
#include <time.h> 

using namespace DirectX;

// Colors
#define RED 0
#define GREEN 1
#define BLUE 2
#define RED_DIFFUSE XMVectorSet(1.0f, 0, 0, -1)
#define GREEN_DIFFUSE XMVectorSet(0, 1.0f, 0, -1)
#define BLUE_DIFFUSE XMVectorSet(0, 0, 1.0f, -1)
#define RED_EMISSION XMVectorSet(0.35f, 0, 0, -1)
#define GREEN_EMISSION XMVectorSet(0, 0.35f, 0, -1)
#define BLUE_EMISSION XMVectorSet(0, 0, 0.35f, -1)

// Dynamic camera
#define MINIMUM_CAM_DISTANCE 25.0f
#define MAXIMUM_CAM_DISTANCE 140.0f
#define CHANGE_SPEED 0.15f

// Spawning drone
#define PROPELLER_SPEED 1700.0f
#define ROTATION_SPEED 60.0f
#define FAST_TRAVEL_SPEED 35.0f
#define SLOW_TRAVEL_SPEED 14.0f
#define DRONE_START XMVectorSet(46.0f, 15.0f, -46.5f, -1)
//#define DRONE_START XMVectorSet(66.0f, 5.0f, 46.5f, -1)
#define RESOURCE_OFFSET -8.0f
#define LIGHT_OFFSET 0.0f
#define ROTATION_THRESHOLD 0.006f // Radian difference bias
#define TRAVEL_THRESHOLD 0.3f
#define TRAVEL_HEIGHT 50.0f

// Spawning
#define START_SPAWNED_RESOURCES 4
#define MAX_RESOURCES_OUT_PER_PLAYER 10 // Default 2
#define SPAWN_INTERVAL 5.0f
#define SPECIAL_RESOURCE_CHANCE 25 // % chance

// Resource
#define FLOAT_INTENSITY 6.0f // Default 6
#define FLOAT_HEIGHT 3.0f // Default 3
#define SPIN_INTENSITY 20.0f // Default 20
#define SMALL_SCALE 0.7f // Default 0.9
#define SPAWN_ANIMATION_TIME 3.5f
#define SPAWN_HEIGHT 170.0f
#define FINAL_HEIGHT 0.6f
#define SPOTLIGHT_Y_OFFSET 30.0f
#define SPOTLIGHT_RANGE 200.0f


// Normal resources
#define PISTOL 0
#define RIFLE 1
#define MOVEMENT 2
#define SHIELD 3
#define DASH 4
#define REFLECT 5

#define BIGGEST_NORMAL_INDEX 5 // Update if adding resources!
// Special - bigger than normal number - resources


#define OBJECT_TYPES 9 // Nr of object types
enum class objectType
{
	e_drone = 0,
	e_weapon = 1,
	e_robot = 2,
	e_node = 3,
	e_projectile = 4,
	e_resource = 5,
	e_scene = 6,
	e_static = 7,
	e_extra
};

enum class renderPass
{
	e_scene,
	e_downSample,
	e_final,
	e_menu,
	e_menuAni,
	e_menuScene,
	e_opaque,
	e_transparent
};

enum class stateType
{
	e_gameState,
	e_mainMenu,
	e_default
};

struct vertex 
{
	float posX, posY, posZ;
	float u, v;
	float normX, normY, normZ;
};

struct material
{
	XMVECTOR ambient = XMVectorSet(0.5f, 0, 0.5f, 0); // ambient.xyz, illumination model enumeration (illum)
	XMVECTOR diffuse = XMVectorSet(0.5f, 0, 0.5f, 0); // diffuse.xyz, refraction (Ni)
	XMVECTOR specular = XMVectorSet(0.5f, 0, 0.5f, 0); // specular.xyz, shininess (Ns)
	XMVECTOR emission = XMVectorSet(0.5f, 0, 0.5f, 0); // emission.xyz, opacity (d)
};



enum class uiType
{
	e_mainMenuSelection,
	e_mainMenuBox,
	e_mainMenuText,
	e_static
};

struct objectData 
{
	XMVECTOR pos = XMVectorSet(0, 0, 0, 0);
	XMVECTOR staticRotation = XMVectorSet(1, 0, 0, 0);
	XMVECTOR rotation = XMVectorSet(1, 0, 0, 0);
	XMVECTOR scale = XMVectorSet(1, 1, 1, 0);
	material material;
};

// Game update return data
struct returnInfo
{
	float posX, posY, posZ;
	float u, v;
	float normX, normY, normZ;
};

struct boundingData {
	XMVECTOR pos = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT2 halfWD = XMFLOAT2(0.0f, 0.0f);
	XMVECTOR xAxis = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR zAxis = XMVectorSet(0.0f, 0.0f, 0.f, 0.0f);
};

struct vectorPairProjections {
	XMVECTOR vec1 = XMVectorSet(1, 0, 0, 0); // Vector
	XMVECTOR vec2 = XMVectorSet(0, 0, 1, 0); // Vector
	float minLength1 = 0;	// Vertex projection length
	float maxLength1 = 0;	// Vertex projection length
	float minLength2 = 0;	// Vertex projection length
	float maxLength2 = 0;	// Vertex projection length
};

// Information acquired from collision detection
struct CollisionInfo
{
	bool m_colliding;
	DirectX::XMVECTOR m_normal;
	DirectX::XMVECTOR m_contactPoint;

	CollisionInfo()
	{
		m_colliding = false;
		m_normal = DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f };
		m_contactPoint = DirectX::XMVECTOR{ 0.0f, 0.0f, 0.0f };
	}
};

struct vertexAndId
{
	float posX, posY, posZ;
	unsigned int iD;
};