#include "MyOctant.h"
using namespace Simplex;

uint MyOctant::octantCount = 0;
uint MyOctant::maxLevel = 3;
uint MyOctant::idealEntityCount = 5;

void Simplex::MyOctant::Init(void)
{
	//make sure values are set to 0;
	numChildren = 0;
	octSize = 0.0f;

	octID = octantCount;
	octantCount++;

	octCenter = vector3(0.0f);
	octMin = vector3(0.0f);
	octMax = vector3(0.0f);

	//get the instances of mesh and entity manager
	meshManager = MeshManager::GetInstance();
	entityManager = MyEntityManager::GetInstance();

	octRoot = nullptr;
	octParent = nullptr;
	for (int i = 0; i < 8; i++)
	{
		octChild[i] = nullptr;
	}
}
Simplex::MyOctant::MyOctant(uint p_MaxLevel, uint p_IdealEntityCount)
{
	Init();
	octantCount = 0;
	maxLevel = p_MaxLevel;
	idealEntityCount = p_IdealEntityCount;
	octID = octantCount;

	octRoot = this;
	objChild.clear();

	int entityCount = entityManager->GetEntityCount();
	std::vector<vector3> minMaxList;
	for (size_t i = 0; i < entityCount; i++)
	{
		MyEntity* entity = entityManager->GetEntity(i);
		MyRigidBody* rb = entity->GetRigidBody();
		//start min max at global
		minMaxList.push_back(rb->GetCenterGlobal());
		minMaxList.push_back(rb->GetCenterGlobal());
	}
	MyRigidBody* rb = new MyRigidBody(minMaxList);

	vector3 halfWidth = rb->GetHalfWidth();
	float fMax = halfWidth.x;
	for (size_t i = 0; i < 3; i++)
	{
		if (fMax < halfWidth[i])
		{
			fMax = halfWidth[i];
		}
	}
	octCenter = rb->GetCenterLocal();
	minMaxList.clear();
	SafeDelete(rb);

	octSize = fMax * 2.0f;
	octMin = octCenter - (vector3(fMax));
	octMax = octCenter + (vector3(fMax));

	octantCount++;
	ConstructTree(maxLevel);
}

Simplex::MyOctant::MyOctant(vector3 p_octCenter, float p_octSize)
{
	Init();
	octCenter = p_octCenter;
	octSize = p_octSize;
	octMax = octCenter + (vector3(octSize)) / 2.0f;
	octMin = octCenter - (vector3(octSize)) / 2.0f;

	octantCount++;
}

Simplex::MyOctant::~MyOctant(void){ Release(); }

uint Simplex::MyOctant::GetOctantCount(void){ return octantCount; }
MyOctant * Simplex::MyOctant::GetParent(void){ return octParent; }
float Simplex::MyOctant::GetSize(void){	return octSize; }
vector3 Simplex::MyOctant::GetCenterGlobal(void) { return octCenter; }
vector3 Simplex::MyOctant::GetMinGlobal(void) { return octMin; }
vector3 Simplex::MyOctant::GetMaxGlobal(void) { return octMax; }

MyOctant * Simplex::MyOctant::GetChild(uint numChild)
{
	if (numChild > 7) return nullptr;
	return octChild[numChild];
}
//recursively display each octant and it's children
void Simplex::MyOctant::Display(vector3 color)
{
	for (size_t i = 0; i < numChildren; i++)
	{
		octChild[i]->Display(color);
	}
	meshManager->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, octCenter) * glm::scale(vector3(octSize)), color, RENDER_WIRE);
}
//recursively subdivide if amount of entities in area is greater than ideal
void Simplex::MyOctant::Subdivide(void)
{
	//stop if maxLevel is exceeded or already subdivided
	if (currentLevel >= maxLevel || numChildren != 0)
		return;

	numChildren = 8;
	float fSize = octSize / 4.0f;
	float fSizeD = fSize * 2.0f;

	vector3 center = octCenter;
	center.x -= fSize;
	center.y -= fSize;
	center.z -= fSize;

	//bottom left back
	octChild[0] = new MyOctant(center, fSizeD);

	//bottom right back
	center.x += fSizeD;
	octChild[1] = new MyOctant(center, fSizeD);

	//bottom right front
	center.z += fSizeD;
	octChild[2] = new MyOctant(center, fSizeD);

	//bottom left front
	center.x -= fSizeD;
	octChild[3] = new MyOctant(center, fSizeD);

	//Top left front
	center.y += fSizeD;
	octChild[4] = new MyOctant(center, fSizeD);

	//top left back
	center.z -= fSizeD;
	octChild[5] = new MyOctant(center, fSizeD);

	//top right back
	center.x += fSizeD;
	octChild[6] = new MyOctant(center, fSizeD);

	//top right front
	center.z += fSizeD;
	octChild[7] = new MyOctant(center, fSizeD);

	for (size_t i = 0; i < 8; i++)
	{
		octChild[i]->octParent = this;
		octChild[i]->currentLevel = currentLevel + 1;
		octChild[i]->octRoot = octRoot;
		//subdivide if there are more items in a division than ideal
		if (octChild[i]->ContainsMoreThan(idealEntityCount))
		{
			octChild[i]->Subdivide();
		}
	}
}
//ARBB Collision of octant and entity
bool Simplex::MyOctant::IsColliding(uint index)
{
	uint objectCount = entityManager->GetEntityCount();

	if (index >= objectCount)
		return false;

	MyEntity* entity = entityManager->GetEntity(index);
	MyRigidBody* rb = entity->GetRigidBody();
	vector3 entityCenter = rb->GetCenterGlobal();
	//size of the cube considered so true will return if in more than one octant
	vector3 halfWidth = rb->GetHalfWidth();

	//check for x
	if (octMax.x < entityCenter.x - halfWidth.x)
		return false;
	if (octMin.x > entityCenter.x + halfWidth.x)
		return false;

	//check for y
	if (octMax.y < entityCenter.y - halfWidth.y)
		return false;
	if (octMin.y > entityCenter.y + halfWidth.y)
		return false;

	//check for z
	if (octMax.z < entityCenter.z - halfWidth.z)
		return false;
	if (octMin.z > entityCenter.z + halfWidth.z)
		return false;

	return true;
}

bool Simplex::MyOctant::IsLeaf(void){ return numChildren == 0; }

bool Simplex::MyOctant::ContainsMoreThan(uint numEntities)
{
	//returns true if number of entities in area exceeds idealEntityCount
	int count = 0;
	int objectCount = entityManager->GetEntityCount();
	for (int i = 0; i < objectCount; i++)
	{
		if (IsColliding(i))
			count++;
		if (count > numEntities)
			return true;
	}
	return false;
}
//recursively kill braches and their children
void Simplex::MyOctant::KillBranches(void)
{
	//if it's a leaf, don't bother trying to kill branches
	if (IsLeaf()) return;

	for (int i = 0; i < numChildren; i++)
	{
		octChild[i]->KillBranches();
		delete octChild[i];
		octChild[i] = nullptr;
	}
	numChildren = 0;
}
//recursively clear entityList of each branch and it's child
void Simplex::MyOctant::ClearEntityList(void)
{
	for (size_t i = 0; i < numChildren; i++)
	{
		octChild[i]->ClearEntityList();
	}
	entityList.clear();
}

void Simplex::MyOctant::ConstructTree(uint p_maxLevel)
{
	//should only be done for root
	if (currentLevel != 0)
		return;

	maxLevel = p_maxLevel;
	octantCount = 1;

	entityList.clear();

	//clear the tree
	KillBranches();
	objChild.clear();

	//if the base tree
	if (ContainsMoreThan(idealEntityCount))
		Subdivide();

	AssignIdToEntity();
	ConstructList();
}

void Simplex::MyOctant::AssignIdToEntity(void)
{
	//continue until leaf is found
	for (int  i = 0; i < numChildren; i++)
	{
		octChild[i]->AssignIdToEntity();
	}
	if (IsLeaf())
	{
		uint numEntities = entityManager->GetEntityCount();
		for (int i = 0; i < numEntities; i++)
		{
			if (IsColliding(i))
			{
				entityList.push_back(i);
				entityManager->AddDimension(i, octID);
			}
		}
	}
}
//recursively assign octRoot
void Simplex::MyOctant::ConstructList(void)
{
	for (int i = 0; i < numChildren; i++)
	{
		octChild[i]->ConstructList();
	}
	
	if (entityList.size() > 0)
	{
		octRoot->objChild.push_back(this);
	}
}

void Simplex::MyOctant::Release(void)
{
	if (this == octRoot)
		KillBranches();

	entityList.clear();
	objChild.clear();
	octSize = 0;
	numChildren = 0;
}
