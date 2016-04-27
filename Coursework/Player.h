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
		MoveVelocity_.y = 0.0f;
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
	void HandlePhysics();
	void GroundCheck();
	void HandleMovementKey(unsigned int key, D3DXVECTOR3 moveAmount);
	void UpdatePosition(D3DXVECTOR3 playerPos);

	// Attributes
	float Height_;
	float MoveSpeed_;
	float LookSpeed_;
	D3DXVECTOR3 MoveVelocity_;

	// Jumping
	float JumpPower_;
	bool IsGrounded_;

	// Physics
	bool UseGravity_;
};
