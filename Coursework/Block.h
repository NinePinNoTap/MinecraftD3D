#pragma once

#include <string>

#include "GameObject.h"

using namespace std;

enum BlockType
{
	Air, Sand, Dirt, Cobblestone, Gold
};

enum BlockFace
{
	Up = 0, Down = 1, Left = 2, Right = 3, Front = 4, Back = 5
};

class Block : public GameObject
{
public:
	Block();
	Block(string blockName, BlockType blockType, bool isSolid);
	~Block();

	// Setters
	void SetType(BlockType type); 
	void SetSolid(bool solid);
	void SetFaceVisible(int index, bool flag);

	// Getters
	BlockType GetType();
	bool GetSolid();

private:
	bool Initialise();

	string BlockName_;
	BlockType Type_;
	bool IsSolid_;

	bool VisibleFace_[6];
};