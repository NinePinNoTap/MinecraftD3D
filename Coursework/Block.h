#pragma once

#include <string>

#include "GameObject.h"
#include "ViewFrustumManager.h"
#include "BlockData.h"

using namespace std;

const int NO_OF_NEIGHBOURS = 6;

class Block : public GameObject
{
public:
	Block();
	~Block();

	bool Frame();
	
	// Setters
	bool SetName(string blockName);
	void SetType(BlockType type);
	void SetSolid(bool solid);

	void SetNeighbour(Direction direction, Block* block);

	// Getters
	BlockType GetType();
	bool IsSolid();

private:
	void HandleNeighbours();
	bool CheckNeighbour(int i);

	string BlockName_;
	BlockType Type_;
	bool IsSolid_;

	Block* NeighbourBlocks_[NO_OF_NEIGHBOURS];
};