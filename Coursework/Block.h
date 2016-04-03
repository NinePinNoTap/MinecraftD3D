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
	~Block();
	
	// Setters
	bool SetName(string blockName);
	void SetType(BlockType type);
	void SetSolid(bool solid);

	// Getters
	BlockType GetType();
	bool IsSolid();

private:
	string BlockName_;
	BlockType Type_;
	bool IsSolid_;
};