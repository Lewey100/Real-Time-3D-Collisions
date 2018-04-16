#ifndef SPHERE_H
#define SPHERE_H


#include <assert.h>

#include <vector>
#include <stdio.h>
#include <windows.h>
#include <d3d11.h>

#include "CommonApp.h"
#include "CommonMesh.h"

#include "Application.h"
class Sphere
{
public:
	
	Sphere(CommonMesh* mesh, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 gravityAcc);

	void Update(float dT);
	void ApplyForce(XMFLOAT3 force, float dT);
	XMVECTOR reflectionVec = XMVectorZero();
	CommonMesh* m_pSphereMesh;
	XMFLOAT3 mSpherePos;
	XMFLOAT3 mSphereVel;
	XMFLOAT3 mGravityAcc;
	float mSphereRadius;
	bool mSphereCollided;
	float mSphereSpeed;
	float dampen;
	XMVECTOR normal;
	XMFLOAT3 penetration;
	float restitution;
	float mass;
	float inv_mass;
};



#endif
