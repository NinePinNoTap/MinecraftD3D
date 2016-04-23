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

	inline void SetHeight(float height)
	{
		Height_ = height;
	}
	inline void SetGravity(bool flag = true)
	{
		UseGravity_ = flag;
	}
	inline void SetMoveSpeed(float moveSpeed)
	{
		MoveSpeed_ = moveSpeed;
	}
	inline void SetLookSpeed(float lookSpeed)
	{
		LookSpeed_ = lookSpeed;
	}

private:
	void HandleMovement();
	void HandleLooking();

	void HandleMovementKey(unsigned int key, D3DXVECTOR3 moveAmount);

	void GroundedCheck();
	void ApplyGravity();

	// Attributes
	float Height_;
	float MoveSpeed_;
	float LookSpeed_;
	D3DXVECTOR3 MoveVelocity_;

	// Physics
	bool IsGrounded_;
	int GroundLevel_;
	bool UseGravity_;
};
