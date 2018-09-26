#include "AppClass.h"
void Application::InitVariables(void)
{
	//init the mesh
	//m_pMesh = new MyMesh();
	for (size_t i = 0; i < 46; i++)
	{
		m_pMesh = new MyMesh();
		m_pMesh->GenerateCube(1.0f, C_BLACK);
		meshes.push_back(m_pMesh);
	}

	m_pMesh->GenerateCube(1.0f, C_BLACK);
	//m_pMesh->GenerateSphere(1.0f, 5, C_WHITE);
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

	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	
	matrix4 m4Scale = glm::scale(IDENTITY_M4, vector3(1.0f,1.0f,1.0f));
	static float value = 0.0f;
	matrix4 m4Translate = glm::translate(IDENTITY_M4, vector3(value, 2.0f, 3.0f));
	value += 0.01f;

	//matrix4 m4Model = m4Translate * m4Scale;
	matrix4 m4Model = m4Scale * m4Translate;

	m_pMesh->Render(m4Projection, m4View, m4Model);

	//Middle Row
	meshes[1]->Render(m4Projection, m4View, glm::translate(vector3(1, 2, 3)));
	meshes[2]->Render(m4Projection, m4View, glm::translate(vector3(2, 2, 3)));
	meshes[3]->Render(m4Projection, m4View, glm::translate(vector3(3, 2, 3)));
	meshes[4]->Render(m4Projection, m4View, glm::translate(vector3(4, 2, 3)));
	meshes[5]->Render(m4Projection, m4View, glm::translate(vector3(5, 2, 3)));
	meshes[6]->Render(m4Projection, m4View, glm::translate(vector3(-1, 2, 3)));
	meshes[7]->Render(m4Projection, m4View, glm::translate(vector3(-2, 2, 3)));
	meshes[8]->Render(m4Projection, m4View, glm::translate(vector3(-3, 2, 3)));
	meshes[9]->Render(m4Projection, m4View, glm::translate(vector3(-4, 2, 3)));
	meshes[10]->Render(m4Projection, m4View, glm::translate(vector3(-5, 2, 3)));
	//Middle -1
	meshes[11]->Render(m4Projection, m4View, glm::translate(vector3(-5, 1, 3)));
	meshes[12]->Render(m4Projection, m4View, glm::translate(vector3(-3, 1, 3)));
	meshes[13]->Render(m4Projection, m4View, glm::translate(vector3(-2, 1, 3)));
	meshes[14]->Render(m4Projection, m4View, glm::translate(vector3(-1, 1, 3)));
	meshes[15]->Render(m4Projection, m4View, glm::translate(vector3(0, 1, 3)));
	meshes[16]->Render(m4Projection, m4View, glm::translate(vector3(1, 1, 3)));
	meshes[17]->Render(m4Projection, m4View, glm::translate(vector3(2, 1, 3)));
	meshes[18]->Render(m4Projection, m4View, glm::translate(vector3(3, 1, 3)));
	meshes[19]->Render(m4Projection, m4View, glm::translate(vector3(5, 1, 3)));
	//Middle -2
	meshes[20]->Render(m4Projection, m4View, glm::translate(vector3(-5, 0, 3)));
	meshes[21]->Render(m4Projection, m4View, glm::translate(vector3(-3, 0, 3)));
	meshes[22]->Render(m4Projection, m4View, glm::translate(vector3(3, 0, 3)));
	meshes[23]->Render(m4Projection, m4View, glm::translate(vector3(5, 0, 3)));
	//Middle -3
	meshes[24]->Render(m4Projection, m4View, glm::translate(vector3(-2, -1, 3)));
	meshes[25]->Render(m4Projection, m4View, glm::translate(vector3(-1, -1, 3)));
	meshes[26]->Render(m4Projection, m4View, glm::translate(vector3(1, -1, 3)));
	meshes[27]->Render(m4Projection, m4View, glm::translate(vector3(2, -1, 3)));
	//Middle +1
	meshes[28]->Render(m4Projection, m4View, glm::translate(vector3(4, 3, 3)));
	meshes[29]->Render(m4Projection, m4View, glm::translate(vector3(3, 3, 3)));
	meshes[30]->Render(m4Projection, m4View, glm::translate(vector3(1, 3, 3)));
	meshes[31]->Render(m4Projection, m4View, glm::translate(vector3(0, 3, 3)));
	meshes[32]->Render(m4Projection, m4View, glm::translate(vector3(-1, 3, 3)));
	meshes[33]->Render(m4Projection, m4View, glm::translate(vector3(-3, 3, 3)));
	meshes[34]->Render(m4Projection, m4View, glm::translate(vector3(-4, 3, 3)));
	//Middle +2
	meshes[35]->Render(m4Projection, m4View, glm::translate(vector3(-3, 4, 3)));
	meshes[36]->Render(m4Projection, m4View, glm::translate(vector3(-2, 4, 3)));
	meshes[37]->Render(m4Projection, m4View, glm::translate(vector3(-1, 4, 3)));
	meshes[38]->Render(m4Projection, m4View, glm::translate(vector3(0, 4, 3)));
	meshes[39]->Render(m4Projection, m4View, glm::translate(vector3(1, 4, 3)));
	meshes[40]->Render(m4Projection, m4View, glm::translate(vector3(2, 4, 3)));
	meshes[41]->Render(m4Projection, m4View, glm::translate(vector3(3, 4, 3)));
	//Middle +3
	meshes[42]->Render(m4Projection, m4View, glm::translate(vector3(2, 5, 3)));
	meshes[43]->Render(m4Projection, m4View, glm::translate(vector3(-2, 5, 3)));
	//Middle +4
	meshes[44]->Render(m4Projection, m4View, glm::translate(vector3(3, 6, 3)));
	meshes[45]->Render(m4Projection, m4View, glm::translate(vector3(-3, 6, 3)));


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
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes.erase(meshes.begin() + i);
	}
	//release GUI
	ShutdownGUI();
}