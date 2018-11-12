//Lucas Nichols (lpn4937@rit.edu)

#ifndef __MYOCTANTCLASS_H_
#define __MYOCTANTCLASS_H_

#include "MyEntityManager.h"

namespace Simplex
{


class MyOctant
{
	//Much of MyOctant was adapted from here: https://github.com/Herm20/FreeFall/blob/master/Freefall/Octree.cpp
	//Some alterations for speed improvements and adaption to fit given methods/variables

	static uint octantCount; //the number of octants instanciated
	static uint maxLevel; //the maximum level an octant can go
	static uint idealEntityCount; //how many ideal entities this object will contain

	uint octID = 0; //current id for this octant
	uint currentLevel = 0; //current level of the octant
	uint numChildren = 0; //number of children on the octant

	float octSize = 0.0f; //size of the octant

	MeshManager* meshManager = nullptr; //mesh manager singleton
	MyEntityManager* entityManager = nullptr; //entity manager singleton

	vector3 octCenter = vector3(0.0f); //center point of the octant
	vector3 octMin = vector3(0.0f); //minimum vector of the octant
	vector3 octMax = vector3(0.0f); //maximum vector of the octant

	MyOctant* octParent = nullptr; //the parent of the current octant
	MyOctant* octChild[8]; //the children of the current octant
	
	std::vector<uint> entityList; //lists of entities under this octant

	MyOctant* octRoot = nullptr; //root octant
	std::vector<MyOctant*> objChild; //list of nodes that contain objects




public:
	MyOctant(uint p_maxLevel, uint p_idealEntityCount);

	MyOctant(vector3 p_octCenter, float p_octSize);

	~MyOctant(void);

	uint GetOctantCount(void);

	MyOctant* GetParent(void);

	float GetSize(void);

	vector3 GetCenterGlobal(void);

	vector3 GetMinGlobal(void);

	vector3 GetMaxGlobal(void);

	void Display(vector3 color);

	void Subdivide(void);

	MyOctant* GetChild(uint numChild);

	bool IsColliding(uint index);

	bool IsLeaf(void);

	bool ContainsMoreThan(uint numEntities);

	void KillBranches(void);

	void ClearEntityList(void);

	void ConstructTree(uint maxLevel);

	void AssignIdToEntity(void);

	void ConstructList(void);

private:
	void Init(void);
	void Release(void);
};

}


#endif // !__MYOCTANTCLASS_H_

