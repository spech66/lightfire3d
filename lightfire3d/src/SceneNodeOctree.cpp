//-----------------------------------------------------------------------------
//  SceneNodeOctree.cpp
//  Copyright (C) 2004 by Sebastian Pech
//  This file is part of the "Lightfire3D Engine".
// 	For conditions of distribution and use, see copyright notice in Main.h
//  - Octree and Nodes -
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "Main.h"

namespace LF3D
{

void COctreeNode::Create(vector<ISceneNode*> SceneNodes, int Depth, int MaxDepth)
{
	prv_SceneNodes = SceneNodes;
	
	Depth++;
	if(Depth == MaxDepth || 0 == SceneNodes.size())
		return;
	
	BoundingBox NodeBox;
	int i;

	for(i = 0; i < prv_SceneNodes.size(); i++)		// Get the BB of this node
	{
		BoundingBox TempBB = prv_SceneNodes[i]->GetBoundingBox();
		if(TempBB.Min.x < NodeBox.Min.x) NodeBox.Min.x = TempBB.Min.x;
		if(TempBB.Min.y < NodeBox.Min.y) NodeBox.Min.y = TempBB.Min.y;
		if(TempBB.Min.z < NodeBox.Min.z) NodeBox.Min.z = TempBB.Min.z;
		
		if(TempBB.Max.x > NodeBox.Max.x) NodeBox.Max.x = TempBB.Max.x;
		if(TempBB.Max.y > NodeBox.Max.y) NodeBox.Max.y = TempBB.Max.y;
		if(TempBB.Max.z > NodeBox.Max.z) NodeBox.Max.z = TempBB.Max.z;
	}
	
	vector<ISceneNode*> ChildSceneNodes[8];		
	//Temporary array to assign the scenes to the nodes
	//0-4 == < z\2 # 5-7 == > z\2
	float HalfX = ((NodeBox.Max.x - NodeBox.Min.x)/2)+NodeBox.Min.x;
	float HalfY = ((NodeBox.Max.y - NodeBox.Min.y)/2)+NodeBox.Min.y;
	float HalfZ = ((NodeBox.Max.z - NodeBox.Min.z)/2)+NodeBox.Min.z;
	for(i = 0; i < prv_SceneNodes.size(); i++)
	{
		BoundingBox TempBB = prv_SceneNodes[i]->GetBoundingBox();
		int ZMod = 0;
		if(TempBB.Min.z > HalfZ)			// We create the nodes around the min values
			ZMod = 4;
		
		if(TempBB.Min.x <= HalfX && TempBB.Min.y <= HalfY)
			ChildSceneNodes[ZMod].push_back(prv_SceneNodes[i]);

		if(TempBB.Min.x > HalfX && TempBB.Min.y <= HalfY)
			ChildSceneNodes[ZMod+1].push_back(prv_SceneNodes[i]);
			
		if(TempBB.Min.x <= HalfX && TempBB.Min.y > HalfY)
			ChildSceneNodes[ZMod+2].push_back(prv_SceneNodes[i]);

		if(TempBB.Min.x > HalfX && TempBB.Min.y > HalfY)
			ChildSceneNodes[ZMod+3].push_back(prv_SceneNodes[i]);					
	}
	
	//Create up to 8 Child nodes
	for(i = 0; i < 8; i++)
	{
		if(ChildSceneNodes[i].size() > 0)
		{	
			COctreeNode *TempOCN = new COctreeNode;	
			TempOCN->Create(ChildSceneNodes[i] , Depth, MaxDepth);
			prv_ChildNodes.push_back(TempOCN);
		}
	}
}

void COctreeNode::Delete()
{
	for(int i = 0; i < prv_ChildNodes.size(); i++)
	{
		prv_ChildNodes[i]->Delete();
		SAFE_DELETE(prv_ChildNodes[i]);
	}
	prv_ChildNodes.erase(prv_ChildNodes.begin(), prv_ChildNodes.end());
}

void COctreeNode::Render()
{
	// If there are no more subdivisions we render this node
	if(prv_ChildNodes.size() == 0)
	{
		for(int i = 0; i < prv_SceneNodes.size(); i++)
			prv_SceneNodes[i]->Render();
	} else {
		for(int i = 0; i < prv_ChildNodes.size(); i++)
			prv_ChildNodes[i]->Render();
	}
}

//-----------------------------------------------------------------------------
// Name:		CSceneNodeOctree
// Description:	Contructor
//-----------------------------------------------------------------------------
CSceneNodeOctree::CSceneNodeOctree(CCore* Core): ISceneNode(Core)
{
	prv_Core = Core;
	MaxDepth = 10;	// ToDo: 10 is a nice value but Should this be more dynamic?
}

//-----------------------------------------------------------------------------
// Name:		CSceneNodeOctree
// Description:	Destructor
//-----------------------------------------------------------------------------
CSceneNodeOctree::~CSceneNodeOctree()
{
	DeleteNodes();
}

//-----------------------------------------------------------------------------
// Name:		BuildTree
// Description:	Create all Tree nodes
//-----------------------------------------------------------------------------
void CSceneNodeOctree::BuildTree()
{
	RootNode.Delete();
	RootNode.Create(prv_Nodes, 0, MaxDepth);
}

//-----------------------------------------------------------------------------
// Name:		Update
// Description:	Update all nodes
//-----------------------------------------------------------------------------
void CSceneNodeOctree::Update()
{
	for(int i = 0; i < prv_Nodes.size(); i++)
		prv_Nodes[i]->Update();
		
	BuildTree(); //ToDo Can we improve this?
}

//-----------------------------------------------------------------------------
// Name:		Render
// Description:	Render this node and all child nodes
//-----------------------------------------------------------------------------
void CSceneNodeOctree::Render()
{
	RenderNode();	// The Octree checks what childs to render
}

//-----------------------------------------------------------------------------
// Name:		Render
// Description:	Render this node
//-----------------------------------------------------------------------------
void CSceneNodeOctree::RenderNode()
{
	RootNode.Render();
}

//-----------------------------------------------------------------------------
// Name:		DeleteNodes
// Description:	Delete all nodes
//-----------------------------------------------------------------------------
void CSceneNodeOctree::DeleteNodes()
{
	for(int i = 0; i < prv_Nodes.size(); i++)
	{
		prv_Nodes[i]->DeleteNodes();
		SAFE_DELETE(prv_Nodes[i]);
	}
	prv_Nodes.erase(prv_Nodes.begin(), prv_Nodes.end());
}

//-----------------------------------------------------------------------------
// Name:		GetNodes
// Description:	Get all child nodes
//-----------------------------------------------------------------------------
vector<ISceneNode*>& CSceneNodeOctree::GetNodes()
{
	return prv_Nodes;
}

};
