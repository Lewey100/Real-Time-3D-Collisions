#include "Application.h"
#include "HeightMap.h"
#include "Sphere.h"

Application* Application::s_pApp = NULL;
HeightMap* Application::m_pHeightMap = NULL;
CommonMesh* Application::m_pSphereMesh = NULL;

std::vector<Sphere> Application::m_SSpherePool;

const int CAMERA_TOP = 0;
const int CAMERA_ROTATE = 1;
const int CAMERA_MAX = 2;



//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////



bool Application::HandleStart()
{
	s_pApp = this;

	m_frameCount = 0.0f;

	m_bWireframe = true;

	m_pHeightMap = new HeightMap("Resources/heightmap.bmp", 2.0f, 0.75f);

	m_pSphereMesh = CommonMesh::NewSphereMesh(this, 1.0f, 16, 16);

	
	
	//mSpherePos = XMFLOAT3(-14.0, 20.0f, -14.0f);
	//mSphereVel = XMFLOAT3(0.0f, 0.0f, 0.0f);
	//mGravityAcc = XMFLOAT3(0.0f, 0.0f, 0.0f);

	m_cameraZ = 50.0f;
	m_rotationAngle = 0.f;

	m_reload = false;

	ReloadShaders();

	if (!this->CommonApp::HandleStart())
		return false;

	this->SetRasterizerState(false, m_bWireframe);

	m_cameraState = CAMERA_ROTATE;

	mSphereCollided = false;

	m_SSpherePool.push_back(Sphere::Sphere(m_pSphereMesh, XMFLOAT3(100, 100, 0), XMFLOAT3(0.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, -0.00f, 0.0f))); //test sphere creation for U I and D dropping tool

	return true;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void Application::HandleStop()
{
	delete m_pHeightMap;

	if (m_pSphereMesh)
		delete m_pSphereMesh;

	this->CommonApp::HandleStop();
}



//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void Application::ReloadShaders()
{
	if (m_pHeightMap->ReloadShader() == false)
		this->SetWindowTitle("Reload Failed - see Visual Studio output window. Press F5 to try again.");
	else
		this->SetWindowTitle("Collision: Zoom / Rotate Q, A / O, P, Camera C, Drop Sphere R, N and T, Wire W");
}

void Application::HandleUpdate(float delta)
{

	if (m_cameraState == CAMERA_ROTATE)
	{
		if (this->IsKeyPressed('Q') && m_cameraZ > 38.0f)
			m_cameraZ -= 1.0f;

		if (this->IsKeyPressed('A'))
			m_cameraZ += 1.0f;

		if (this->IsKeyPressed('O'))
			m_rotationAngle -= .01f;

		if (this->IsKeyPressed('P'))
			m_rotationAngle += .01f;
	}

	if (this->IsKeyPressed('2'))
	{
		delete m_pHeightMap;

		m_pHeightMap = new HeightMap("Resources/heightmapflat.bmp", 2.0f, 0.75f);
	}

	if (this->IsKeyPressed('1'))
	{
		delete m_pHeightMap;
		m_pHeightMap = new HeightMap("Resources/heightmap.bmp", 2.0f, 0.75f);
	}

	static bool dbC = false;

	if (this->IsKeyPressed('C'))
	{
		if (!dbC)
		{
			if (++m_cameraState == CAMERA_MAX)
				m_cameraState = CAMERA_TOP;

			dbC = true;
		}
	}
	else
	{
		dbC = false;
	}


	static bool dbW = false;
	if (this->IsKeyPressed('W'))
	{
		if (!dbW)
		{
			m_bWireframe = !m_bWireframe;
			this->SetRasterizerState(false, m_bWireframe);
			dbW = true;
		}
	}
	else
	{
		dbW = false;
	}


	if (this->IsKeyPressed(VK_F5))
	{
		if (!m_reload)
		{
			ReloadShaders();
			m_reload = true;
		}
	}
	else
		m_reload = false;

	if (this->IsKeyPressed(VK_SPACE)) {
		delta = delta / 16;	//slow down game speed to 1/6th regular speed.
	}
	else {
		delta = 1.0f / 60.0f;
	}

	static bool dbH = false;
	static bool bottomDelete = false;

	if (this->IsKeyPressed('H')) 
	{
		if (!dbH)
		{
			if (bottomDelete)
			{
				int deletedFaces = m_pHeightMap->EnableAll(); //enables bottom layers of heightmap
				
			}
			else {
				int deletedFaces = m_pHeightMap->DisableBelowLevel(FACES_DISABLED); //disables bottom 4 layers of heightmap
			}
			bottomDelete = !bottomDelete; //inverts boolean check
		}
	}
	else {
		dbH = false;
	}

	static bool dbR = false;
	if (this->IsKeyPressed('R'))
	{
		if (dbR == false)
		{
			static int dx = 0;
			static int dy = 0;
			m_SSpherePool.push_back(Sphere::Sphere(m_pSphereMesh, XMFLOAT3((float)((rand() % 14 - 7.0f) - 0.5), 20.0f, (float)((rand() % 14 - 7.0f) - 0.5)), XMFLOAT3(0.0f, 0.2f, 0.0f), XMFLOAT3(0.0f, -50.f, 0.0f)));
			//Sphere sphere =
			//mSpherePos = XMFLOAT3((float)((rand() % 14 - 7.0f) - 0.5), 20.0f, (float)((rand() % 14 - 7.0f) - 0.5));
			//mSphereVel = XMFLOAT3(0.0f, 0.2f, 0.0f);
			//mGravityAcc = XMFLOAT3(0.0f, -0.05f, 0.0f);
			//mSphereCollided = false;
			dbR = true;
		}
	}
	else
	{
		dbR = false;
	}


	static bool dbT = false;
	if (this->IsKeyPressed('T'))
	{
		if (dbT == false)
		{
			static int dx = 0;
			static int dy = 0;
			mSpherePos = XMFLOAT3(mSpherePos.x, 20.0f, mSpherePos.z);
			mSphereVel = XMFLOAT3(0.0f, 0.2f, 0.0f);
			mGravityAcc = XMFLOAT3(0.0f, -0.05f, 0.0f);
			mSphereCollided = false;
			dbT = true;
		}
	}
	else
	{
		dbT = false;
	}

	static int dx = 0;
	static int dy = 0;
	static int seg = 0;
	static bool dbN = false;

	if (this->IsKeyPressed('N'))
	{
		if (dbN == false)
		{
			if (++seg == 2)
			{
				seg = 0;
				if (++dx == 15)
				{
					if (++dy == 15) dy = 0;
					dx = 0;
				}
			}

			if (seg == 0)
				mSpherePos = XMFLOAT3(((dx - 7.0f) * 2) - 0.5f, 20.0f, ((dy - 7.0f) * 2) - 0.5f);
			else
				mSpherePos = XMFLOAT3(((dx - 7.0f) * 2) + 0.5f, 20.0f, ((dy - 7.0f) * 2) + 0.5f);

			mSphereVel = XMFLOAT3(0.0f, 0.2f, 0.0f);
			mGravityAcc = XMFLOAT3(0.0f, -0.05f, 0.0f);
			m_SSpherePool.push_back(Sphere::Sphere(m_pSphereMesh, mSpherePos, mSphereVel, mGravityAcc));
			mSphereCollided = false;
			dbN = true;
		}
	}
	else
	{
		dbN = false;
	}

	static bool dbF = false;
	if (this->IsKeyPressed('F'))
	{
		if (dbF == false)
		{
			mSphereVel = XMFLOAT3(0.0f, 0.2f, 0.0f);
			mGravityAcc = XMFLOAT3(0.0f, -50.f, 0.0f);
			//dprintf("%i", m_pHeightMap->FaceCount());
			XMFLOAT3 NewmSpherePos = XMFLOAT3(-10.f, 20.f,0.f);
			m_SSpherePool.push_back(Sphere::Sphere(m_pSphereMesh, NewmSpherePos, mSphereVel, mGravityAcc)); //create two new spheres to drop and meet in the middle of the heightmap
			NewmSpherePos = XMFLOAT3(10.f,20.f,0.f);
			m_SSpherePool.push_back(Sphere::Sphere(m_pSphereMesh, NewmSpherePos, mSphereVel, mGravityAcc));
	
			dbF = true;
		}
	}
	else {
		dbF = false;
	}

	static bool dbU = false;
	static int vertIndex = 0, faceIndex = 0;

	if (this->IsKeyPressed('U'))
	{
		if (!dbU)
		{
			

			faceIndex++; //moves up the faces in the heightmap
			if (faceIndex > m_pHeightMap->FaceCount()) //check to make sure it isn't out of range
			{
				faceIndex = 0;
			}
			vertIndex = 3; //always be on center when cycling through faces

			mSphereVel = XMFLOAT3(0.0f, 0.2f, 0.0f);
			mGravityAcc = XMFLOAT3(0, 0, 0);
			mSpherePos = XMFLOAT3(m_pHeightMap->GetHeightMapFacePos(faceIndex, vertIndex).x, 20.0f, m_pHeightMap->GetHeightMapFacePos(faceIndex, vertIndex).z); //sets position to the matching face from heightMap
			m_SSpherePool[0].mSpherePos = mSpherePos; //sets the moveable sphere's position to the new face
			m_SSpherePool[0].mGravityAcc = mGravityAcc; //resets the sphere's velocity and gravity so it falls straight down
			m_SSpherePool[0].mSphereVel = mSphereVel;
			dbU = true;
		}
	}
	else {
		dbU = false;
	}

	static bool dbI = false;

	if (this->IsKeyPressed('I'))
	{
		if (!dbI)
		{


			faceIndex--; //goes back through the heightmap
			if (faceIndex < 0)
			{
				faceIndex = m_pHeightMap->FaceCount(); //Make sure it's in range
			}
			vertIndex = 3; //always be on center of the face

			mSphereVel = XMFLOAT3(0.0f, 0.2f, 0.0f);
			mGravityAcc = XMFLOAT3(0.0f, 0.f, 0.0f);
			mSpherePos = XMFLOAT3(m_pHeightMap->GetHeightMapFacePos(faceIndex, vertIndex).x, 20.0f, m_pHeightMap->GetHeightMapFacePos(faceIndex, vertIndex).z); //sets position to heightmap face position
			m_SSpherePool[0].mSpherePos = mSpherePos;
			m_SSpherePool[0].mGravityAcc = mGravityAcc; //sets the spheres values as above
			m_SSpherePool[0].mSphereVel = mSphereVel;
			dbI = true;
		}
	}
	else {
		dbI = false;
	}

	static bool dbD = false;

	if (this->IsKeyPressed('D'))
	{
		if (!dbD)
		{
			dbD = true;
			vertIndex++; //cycle through vertices of the face
			if (vertIndex > 3)
			{
				vertIndex = 0; //check whether its in range of the amount of options
			}
			mSphereVel = XMFLOAT3(0.0f, 0.2f, 0.0f);
			mSpherePos = XMFLOAT3(m_pHeightMap->GetHeightMapFacePos(faceIndex, vertIndex).x, 20.0f, m_pHeightMap->GetHeightMapFacePos(faceIndex, vertIndex).z); //sets the sphere to the position of the given vertex
			mGravityAcc = XMFLOAT3(0.0f, -50.f, 0.0f); //gravity to drop the sphere
			m_SSpherePool[0].mSpherePos = mSpherePos;
			m_SSpherePool[0].mGravityAcc = mGravityAcc; //sets the spheres values to have it interact with the world
			m_SSpherePool[0].mSphereVel = mSphereVel;


		}
	}
	else {
		dbD = false;
	}

	if (m_SSpherePool.size() > 0)
	{
		for (size_t i = 0; i < m_SSpherePool.size() - 1; i++)
		{

			m_SSpherePool[i].Update(delta);

			for (int j = 0; j < m_SSpherePool.size() - 2; j++)
			{
				Manifold man; //collision pair
				man.A = &m_SSpherePool[i];
				man.B = &m_SSpherePool[j];
				if (TestSphereSphere(&man) && i != j) //check collisions of all the collision pairs, ignoring the any spheres checking itself
				{
					HandleSphereCollision(man.A, man.B, &man.collNorm);
					PositionalCorrection(&man);
				}
			}

			if (m_SSpherePool[i].mSpherePos.y < -20.f)
			{
				m_SSpherePool.erase(m_SSpherePool.begin() + i); //if below the world, remove sphere
			}
		}
	}



	

}

void Application::HandleSphereCollision(Sphere* a, Sphere* b, XMVECTOR* norm)
{
	XMVECTOR sphere1Vel = XMLoadFloat3(&a->mSphereVel);
	XMVECTOR sphere2Vel = XMLoadFloat3(&b->mSphereVel);

	XMVECTOR relVel = sphere2Vel - sphere1Vel;

	float velAlongNorm = XMVectorGetX(XMVector3Dot(relVel, *norm));
	/*if (velAlongNorm >= 0)
	{
		return;
	}*/

	float restit = min(a->restitution, b->restitution);

	float impulseScal = (-(1 + 0.2f) * velAlongNorm) / (a->inv_mass + b->inv_mass);

	XMVECTOR impulse = impulseScal * *norm;
	/*XMVECTOR sphereForce = (1.0 / a->mass) * impulse;*/

	impulse = a->dampen * impulse;
	
	XMStoreFloat3(&a->mSphereVel, sphere1Vel - impulse);

	XMStoreFloat3(&b->mSphereVel, sphere2Vel + impulse);
}

void Application::PositionalCorrection(Manifold* m)
{
	Sphere* A = m->A;
	Sphere* B = m->B;
	const float percent = 0.5f;
	const float slop = 0.01f;

	XMVECTOR APos = XMLoadFloat3(&A->mSpherePos);
	XMVECTOR BPos = XMLoadFloat3(&B->mSpherePos);

	XMVECTOR correction = ((max(m->penetration - slop, 0.0f)) / (A->inv_mass + B->inv_mass)) * percent * m->collNorm;

	APos = APos - correction;
	BPos = BPos + correction;

	XMStoreFloat3(&A->mSpherePos, APos);
	XMStoreFloat3(&B->mSpherePos, BPos);

}



bool Application::TestSphereSphere(Manifold* m)
{
	Sphere* sphere1 = m->A;
	Sphere* sphere2 = m->B;

	XMVECTOR n = XMLoadFloat3(&sphere2->mSpherePos) - XMLoadFloat3(&sphere1->mSpherePos);

	float dist = XMVectorGetX(XMVector3Dot(n,n));

	float radiusSum = sphere1->mSphereRadius * sphere2->mSphereRadius;

	dist = sqrt(dist);
	if (dist > radiusSum)
	{
		return false;
	}

	if (dist != 0)
	{
		m->penetration = radiusSum - dist;

		m->collNorm = n / dist;
			return true;
	}

	else {
		m->penetration = sphere1->mSphereRadius;
		m->collNorm = XMVectorSet(0, 1, 0, 0);
		return true;
	}
}



//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

void Application::HandleRender()
{
	XMVECTOR vCamera, vLookat;
	XMVECTOR vUpVector = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX matProj, matView;

	switch (m_cameraState)
	{
	case CAMERA_TOP:
		vCamera = XMVectorSet(0.0f, 100.0f, 0.1f, 0.0f);
		vLookat = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		matView = XMMatrixLookAtLH(vCamera, vLookat, vUpVector);
		matProj = XMMatrixOrthographicLH(64, 36, 1.5f, 5000.0f);
		break;
	case CAMERA_ROTATE:
		vCamera = XMVectorSet(sin(m_rotationAngle)*m_cameraZ, (m_cameraZ*m_cameraZ) / 50, cos(m_rotationAngle)*m_cameraZ, 0.0f);
		vLookat = XMVectorSet(0.0f, 10.0f, 0.0f, 0.0f);
		matView = XMMatrixLookAtLH(vCamera, vLookat, vUpVector);
		matProj = XMMatrixPerspectiveFovLH(float(D3DX_PI / 7), 2, 1.5f, 5000.0f);
		break;
	}

	this->EnableDirectionalLight(1, XMFLOAT3(-1.f, -1.f, -1.f), XMFLOAT3(0.55f, 0.55f, 0.65f));
	this->EnableDirectionalLight(2, XMFLOAT3(1.f, -1.f, 1.f), XMFLOAT3(0.15f, 0.15f, 0.15f));

	this->SetViewMatrix(matView);
	this->SetProjectionMatrix(matProj);

	this->Clear(XMFLOAT4(0.05f, 0.05f, 0.5f, 1.f));

	XMMATRIX worldMtx;


	SetDepthStencilState(false, true);
	m_pHeightMap->Draw(m_frameCount);
	if (m_SSpherePool.size() > 0) {
		for (size_t i = 0; i < m_SSpherePool.size() - 1; i++)
		{
			worldMtx = XMMatrixTranslation(m_SSpherePool[i].mSpherePos.x, m_SSpherePool[i].mSpherePos.y, m_SSpherePool[i].mSpherePos.z);
			this->SetWorldMatrix(worldMtx);
			SetDepthStencilState(true, true);
			if (m_SSpherePool[i].m_pSphereMesh)
				m_SSpherePool[i].m_pSphereMesh->Draw();
		}
	}
	m_frameCount++;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Application application;

	Run(&application);

	return 0;
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
