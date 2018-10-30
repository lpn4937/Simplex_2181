#include "MyEntityManager.h"
using namespace Simplex;
//  MyEntityManager
MyEntityManager* MyEntityManager::m_pInstance = nullptr;
void MyEntityManager::Init(void)
{
	m_uEntityCount = 0;
	m_entityList.clear();
}
void MyEntityManager::Release(void)
{
	for (size_t i = 0; i < m_uEntityCount; i++)
	{
		SafeDelete(m_entityList[i]);
	}
	m_uEntityCount = 0;
	m_entityList.clear();
}
MyEntityManager* MyEntityManager::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new MyEntityManager();
	}
	return m_pInstance;
}
void MyEntityManager::ReleaseInstance()
{
	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
		m_pInstance = nullptr;
	}
}
int Simplex::MyEntityManager::GetEntityIndex(String a_sUniqueID)
{
	//find index of entity given id
	for (size_t i = 0; i < m_uEntityCount; i++)
	{
		if (m_entityList[i]->GetUniqueID() == a_sUniqueID)
			return i;
	}
	return -1;
}
//Accessors
Model* Simplex::MyEntityManager::GetModel(uint a_uIndex)
{
	//make sure list isn't empty
	if (m_entityList.size() == 0) return nullptr;

	//return model if index isn't out of range otherwise return last model
	return (a_uIndex >= m_uEntityCount) ? m_entityList[m_uEntityCount-1]->GetModel() : m_entityList[a_uIndex]->GetModel();
}
Model* Simplex::MyEntityManager::GetModel(String a_sUniqueID)
{
	//get the model of entity with certain id
	return m_entityList[GetEntityIndex(a_sUniqueID)]->GetModel();
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(uint a_uIndex)
{
	//return rigidbody if index isn't out of range
	return (a_uIndex > m_uEntityCount) ? nullptr : m_entityList[a_uIndex]->GetRigidBody();
}
RigidBody* Simplex::MyEntityManager::GetRigidBody(String a_sUniqueID)
{
	//return rigidbody of certain id
	return m_entityList[GetEntityIndex(a_sUniqueID)]->GetRigidBody();
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(uint a_uIndex)
{
	//make sure list isn't empty
	if (m_uEntityCount == 0) return IDENTITY_M4;

	//return model matrix if index isn't out of range
	return (a_uIndex > m_uEntityCount) ? m_entityList[m_uEntityCount-1]->GetModelMatrix() : m_entityList[a_uIndex]->GetModelMatrix();
}
matrix4 Simplex::MyEntityManager::GetModelMatrix(String a_sUniqueID)
{
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);
	//if it exists
	if (temp) return temp->GetModelMatrix();
	return IDENTITY_M4;
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, String a_sUniqueID)
{
	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);
	//if it exists
	if (temp) return temp->SetModelMatrix(a_m4ToWorld);
}
void Simplex::MyEntityManager::SetModelMatrix(matrix4 a_m4ToWorld, uint a_uIndex)
{
	//make sure list isn't empty
	if (m_uEntityCount == 0) return;

	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;
	//set model matrix of index
	m_entityList[a_uIndex]->SetModelMatrix(a_m4ToWorld);
}
//The big 3
MyEntityManager::MyEntityManager(){Init();}
MyEntityManager::MyEntityManager(MyEntityManager const& other){ }
MyEntityManager& MyEntityManager::operator=(MyEntityManager const& other) { return *this; }
MyEntityManager::~MyEntityManager(){Release();};
// other methods
void Simplex::MyEntityManager::Update(void)
{
	//Check collisions
	for (size_t i = 0; i < m_uEntityCount; i++)
	{
		for (size_t j = 0; j < m_uEntityCount; j++)
		{
			m_entityList[i]->IsColliding(m_entityList[j]);
		}
	}
}
void Simplex::MyEntityManager::AddEntity(String a_sFileName, String a_sUniqueID)
{
	//create new entity
	MyEntity* temp = new MyEntity(a_sFileName, a_sUniqueID);
	if (temp->IsInitialized())
	{
		m_entityList.push_back(temp);
		m_uEntityCount = m_entityList.size();
	}
}
void Simplex::MyEntityManager::RemoveEntity(uint a_uIndex)
{
	//check if list is empty
	if (m_uEntityCount == 0) return;
	
	//check bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	//swap with last entry
	if (a_uIndex != m_uEntityCount - 1)
	{
		std::swap(m_entityList[a_uIndex], m_entityList[m_uEntityCount - 1]);
	}

	//pop last entry
	SafeDelete(m_entityList[m_uEntityCount - 1]);
	m_entityList.pop_back();
	m_uEntityCount--;
	return;
}
void Simplex::MyEntityManager::RemoveEntity(String a_sUniqueID)
{
	//get index of id
	int index = GetEntityIndex(a_sUniqueID);
	RemoveEntity(index);
}
String Simplex::MyEntityManager::GetUniqueID(uint a_uIndex)
{
	//check if list is empty
	if (m_uEntityCount == 0) return nullptr;

	//check bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	//get id of entity at index
	return m_entityList[a_uIndex]->GetUniqueID();
}
MyEntity* Simplex::MyEntityManager::GetEntity(uint a_uIndex)
{
	//check if list is empty
	if (m_uEntityCount == 0) return nullptr;

	//check bounds
	if (a_uIndex >= m_uEntityCount)
		a_uIndex = m_uEntityCount - 1;

	//get entity at index
	return m_entityList[a_uIndex];
}
void Simplex::MyEntityManager::AddEntityToRenderList(uint a_uIndex, bool a_bRigidBody)
{
	//check bounds
	if (a_uIndex >= m_uEntityCount) 
	{
		for ( a_uIndex = 0; a_uIndex < m_uEntityCount; ++a_uIndex)
		{
			m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
		}
	}
	else
	{
		//add entity to render list with or without rigidbody
		m_entityList[a_uIndex]->AddToRenderList(a_bRigidBody);
	}

}
void Simplex::MyEntityManager::AddEntityToRenderList(String a_sUniqueID, bool a_bRigidBody)
{

	MyEntity* temp = MyEntity::GetEntity(a_sUniqueID);
	//if it exists
	//add entity to render list with or without rigidbody
	if (temp) temp->AddToRenderList(a_bRigidBody);
}