#pragma once

#include <string>

#include "BoundingBox.h"
#include "InstanceData.h"

using namespace std;

struct BoundingBox;

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

	// update
	void Refresh();

	// Functionality
	void clone(Block& block);

	// setters
	void setPosition(float x, float y, float z);
	void setNeighbour(Direction direction, Block* block);

	// getters
	inline BlockType getType() { return m_blockType; }
	inline bool isSolid() { return m_isSolid; }
	inline bool isActive() { return m_isActive; }
	inline BoundingBox getBoundingBox() { return m_boundingBox; }
	InstanceData getInstance();

private:
	void handleNeighbours();
	bool checkNeighbour(int i);

	// Block Properties
	string m_blockName;
	BlockType m_blockType;
	D3DXVECTOR3 m_position;
	D3DXVECTOR2 m_textureOffset;
	D3DXVECTOR2 m_totalTextures;
	bool m_isSolid;
	bool m_isActive;

	// Neighbours
	Block* m_neighbours[6];

	BoundingBox m_boundingBox;
};