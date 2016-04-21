#pragma once

#include <string>

#include "InstancedGameObject.h"
#include "ViewFrustumManager.h"

using namespace std;

enum BlockType
{
	None, Air, Dirt, Sand, Gravel, Wood, Cobblestone, Stone, Gold, Diamond, Water, Lava, Bedrock
};


enum Direction
{
	Up, Down, Left, Right, Forward, Backward
};

class Block
{
public:
	Block();
	Block(string blockName, BlockType blockType, D3DXVECTOR2 textureOffset, D3DXVECTOR2 totalTextures, bool isSolid);
	~Block();

	// Frame
	void Refresh();

	// Functionality
	void CopyFrom(Block& block);

	// Setters
	void SetPosition(float x, float y, float z);
	void SetNeighbour(Direction direction, Block* block);

	// Getters
	BlockType GetType();
	InstanceData GetInstance();
	bool IsSolid();
	bool IsActive();


private:
	void HandleNeighbours();
	bool CheckNeighbour(int i);

	// Block Properties
	string BlockName_;
	BlockType BlockType_;
	D3DXVECTOR3 Position_;
	D3DXVECTOR2 TextureOffset_;
	D3DXVECTOR2 TotalTextures_;
	bool IsSolid_;
	bool IsActive_;

	// Neighbours
	Block* NeighbourBlocks_[6];
};