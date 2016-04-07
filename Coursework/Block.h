#pragma once

#include <string>

#include "InstancedGameObject.h"
#include "ViewFrustumManager.h"

using namespace std;

enum BlockType
{
	None, Air, Dirt, Sand, Gravel, Wood, 
	Cobblestone, Stone, Gold, Diamond,
	Water, Lava
};

class Block
{
public:
	Block();
	Block(string name, BlockType type, D3DXVECTOR2 offset, bool solid);
	~Block();

	// Frame
	void Refresh();

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

	string Name_;
	BlockType Type_;
	D3DXVECTOR3 Position_;
	D3DXVECTOR2 TextureOffset_;
	bool IsSolid_;
	bool IsActive_;

	Block* NeighbourBlocks_[6];
};