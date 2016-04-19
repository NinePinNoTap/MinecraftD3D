#pragma once

#include "GameObject.h"
#include "Block.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	bool Initialise();
	bool Frame();

	void SetHeight(float height);
	void SetGravityOn()
	{
		UseGravity_ = true;
	}

private:
	void GroundedCheck();
	void ApplyGravity();

	float Height_;
	bool IsGrounded_;
	int GroundLevel_;
	float GravityAmount_;
	bool UseGravity_;
};
