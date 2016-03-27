#pragma once

enum BlockType
{
	Air,
	Dirt,
	Stone,
	Diamond
};

class Block
{
public:
	Block() : Type_(BlockType::Air), IsActive_(true) { }
	Block(BlockType type) : Type_(type), IsActive_(true) { }
	~Block() { }

	// Setters
	void SetType(BlockType type) { Type_ = type; }
	void SetActive(bool flag) { IsActive_ = flag; }

	// Getters
	BlockType GetType() { return Type_; }
	bool GetActive() { return IsActive_; }

private:
	BlockType Type_;
	bool IsActive_;
};