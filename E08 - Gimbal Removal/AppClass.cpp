#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh
	m_pMesh = new MyMesh();
	m_pMesh->GenerateCube(1.0f, C_WHITE);
	//m_pMesh->GenerateCone(2.0f, 5.0f, 3, C_WHITE);
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//Get a timer
	static uint uClock = m_pSystem->GenClock();
	float fTimer = m_pSystem->GetTimeSinceStart(uClock);
	float fDeltaTime = m_pSystem->GetDeltaTime(uClock);

	//translate vector orientation into a matrix
	matrix4 m4OrientX = glm::rotate(IDENTITY_M4, m_v3Orientation.x, vector3(1.0f, 0.0f, 0.0f));
	matrix4 m4OrientY = glm::rotate(IDENTITY_M4, m_v3Orientation.y, vector3(0.0f, 1.0f, 0.0f));
	matrix4 m4OrientZ = glm::rotate(IDENTITY_M4, m_v3Orientation.z, vector3(0.0f, 0.0f, 1.0f));

	matrix4 m4Orientation = m4OrientX * m4OrientY * m4OrientZ;

	//m_v3Orientation

	//Orientation of the mesh
	//static quaternion qStart;
	//quaternion q1 = glm::angleAxis(fDeltaTime, vector3(0.0f, 1.0f, 0.0f));
	//qStart = qStart * q1;

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();

	m_pMesh->Render(m4Projection, m4View, ToMatrix4(m_qOrientation));
		
	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	SafeDelete(m_pMesh);

	//release GUI
	ShutdownGUI();
}