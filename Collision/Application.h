#ifndef APPLICATION_H
#define APPLICATION_H

#define WIN32_LEAN_AND_MEAN

#include <assert.h>

#include <stdio.h>
#include <windows.h>
#include <d3d11.h>

#include "CommonApp.h"
#include "CommonMesh.h"
#include <vector>
#include "Sphere.h"

class HeightMap;
class Sphere;

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//
// An example of creating a shader of your own, that fits in
// relatively neatly with the CommonApp functionality.
//
// Edit the shader as the program runs, then Alt+Tab back to it and
// press F5 to reload the shader. Instant feedback!
//
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class Application:
public CommonApp
{
private:

	struct Manifold
	{
		Sphere *A;
		Sphere *B;
		float penetration;
		XMVECTOR collNorm;
	};
public:

	static Application* s_pApp;
	static HeightMap* m_pHeightMap;
	static CommonMesh* m_pSphereMesh;
	static std::vector<Sphere> m_SSpherePool;

protected:
	bool HandleStart();
	void HandleStop();
	void HandleUpdate(float delta);
	void HandleRender();
	
	bool TestSphereSphere(Manifold*);
	void HandleSphereCollision(Sphere* a, Sphere* b, XMVECTOR* norm);
	void PositionalCorrection(Manifold*);

private:

	float m_frameCount;

	bool m_reload;

	float m_rotationAngle;
	float m_cameraZ;
	bool m_bWireframe;

	int m_cameraState;

	float dTime;
	
	XMFLOAT3 mSpherePos;
	XMFLOAT3 mSphereVel;
	float mSphereSpeed;
	bool mSphereCollided;
	XMFLOAT3 mGravityAcc;

	void ReloadShaders();
};

#endif