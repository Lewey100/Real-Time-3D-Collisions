#include "Sphere.h"
#include "HeightMap.h"
#include "Application.h"

Sphere::Sphere(CommonMesh* mesh, XMFLOAT3 pos, XMFLOAT3 vel, XMFLOAT3 gravityAcc)
{
	m_pSphereMesh = mesh;
	mSpherePos = pos;
	mSphereVel = vel;
	mGravityAcc = gravityAcc;
	mSphereRadius = 1.f;
	mSphereCollided = false;
	mSphereSpeed = 0;
	dampen = 0.8f;
	restitution = 1;
	mass = 2;
	inv_mass = 1 / mass;
}

void Sphere::Update(float dT) {

	// Update Sphere
	XMVECTOR vSColPos, vSColNorm;
	
	//dprintf("%f\n", dT);
	if (!mSphereCollided)
	{
		XMVECTOR vSPos = XMLoadFloat3(&mSpherePos);
		XMVECTOR vSVel = XMLoadFloat3(&mSphereVel);
		XMVECTOR vSAcc = XMLoadFloat3(&mGravityAcc);

		vSVel += vSAcc * dT; // The new velocity gets passed through to the collision so it can base its predictions on our speed NEXT FRAME
		vSPos += vSVel * dT; // Really important that we add LAST FRAME'S velocity as this was how fast the collision is expecting the ball to move
		
		XMStoreFloat3(&mSphereVel, vSVel);
		XMStoreFloat3(&mSpherePos, vSPos);

		//mSphereSpeed = XMVectorGetX(XMVector3Length(vSVel));
		
		mSphereCollided = Application::m_pHeightMap->SphereCollision(vSColNorm, *this);
		
		if (mSphereCollided)
		{
			reflectionVec = dampen * ((-2.f* XMVector3Dot(-vSVel, vSColNorm)) * vSColNorm);
			//vSPos = vSPos + reflectionVec;
			vSVel = vSVel - reflectionVec;
			XMStoreFloat3(&mSphereVel, vSVel);

			//mSphereVel = XMFLOAT3(-mSphereVel.x * dampen, -mSphereVel.y * dampen, -mSphereVel.z * dampen);
			mSphereCollided = false;
		}

		
		//vSPos -= dampen * reflectionVec;
		//XMStoreFloat3(&mSpherePos, vSPos);
	}
}

void Sphere::ApplyForce(XMFLOAT3 force, float dT)
{

}

