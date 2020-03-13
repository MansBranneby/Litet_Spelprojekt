#pragma once
#include "StructsAndEnums.h"

DirectX::XMVECTOR getClosestPointFromPointToTriangle(DirectX::XMVECTOR p, DirectX::XMVECTOR a, DirectX::XMVECTOR b, DirectX::XMVECTOR c);
DirectX::XMVECTOR getClosestPointFromPointToOBB(DirectX::XMVECTOR point, boundingData OBB);
CollisionInfo testOBBSphere(DirectX::XMVECTOR spherePos, float sphereRad, boundingData OBB);
CollisionInfo testOBBOBB(boundingData aOBB, boundingData bOBB);
CollisionInfo testSphereTriangle(DirectX::XMVECTOR SpherePos, float sphereRad, DirectX::XMVECTOR a, DirectX::XMVECTOR b, DirectX::XMVECTOR c);
CollisionInfo testSphereSphere(DirectX::XMVECTOR aPos, DirectX::XMVECTOR bPos, float aRad, float bRad);
CollisionInfo testMovingSphereSphere(DirectX::XMVECTOR aPos, DirectX::XMVECTOR bPos, float aRad, float bRad, DirectX::XMVECTOR aVel, DirectX::XMVECTOR bVel);
bool testLineLine(XMVECTOR startL1, XMVECTOR endL1, XMVECTOR startL2, XMVECTOR endL2);
bool testLineTriangle(XMVECTOR start, XMVECTOR end, XMVECTOR triPos[3]);
float testLineTriangleT(XMVECTOR start, XMVECTOR end, XMVECTOR triPos[3]);
float testRayTriangle(XMVECTOR origin, XMVECTOR dir, XMVECTOR triPos[3]);
bool testLineSphere(XMVECTOR start, XMVECTOR end, XMVECTOR spherePos, float radius);