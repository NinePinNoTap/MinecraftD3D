#pragma once

#include <string>

#include "GameObject.h"

using namespace std;

enum BlockType
{
	Air, Sand, Dirt, Cobblestone, Gold
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

	// Getters
	BlockType GetType();
	bool GetSolid();

private:
	bool Initialise();

	string BlockName_;
	BlockType Type_;
	bool IsSolid_;
};