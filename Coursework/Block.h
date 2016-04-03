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
	~Block();
	
	// Setters
	bool SetName(string blockName);
	void SetType(BlockType type);
	void SetSolid(bool solid);

	void SetNeighbour(Direction direction, Block* block);

	// Getters
	BlockType GetType();
	bool IsSolid();

private:
	string BlockName_;
	BlockType Type_;
	bool IsSolid_;

	Block* BlockUp_;
	Block* BlockDown_;
	Block* BlockLeft_;
	Block* BlockRight_;
	Block* BlockForward_;
	Block* BlockBackward_;
};