#include "Player.h"
#include "VoxelWorld.h"

Player::Player() : GameObject()
{
	Height_ = 1;
	IsGrounded_ = false;
	UseGravity_ = false;
	GroundLevel_ = 0;
}

Player::~Player()
{

}

bool Player::Initialise()
{
	// Call parent initialise
	GameObject::Initialise();

	// Define Start Position
	Transform_->SetPosition(0, 98, 0);
	Camera::Instance()->GetTransform()->SetPosition(Transform_->GetPosition() + D3DXVECTOR3(0, Height_, 0));

	// Define Vars
	LookSpeed_ = 10.0f;
	MoveSpeed_ = 4.137f;

	return true;
}

bool Player::Frame()
{
	//=================
	// Handle Controls
	//=================

	HandleMovement();
	HandleLooking();

	//=========
	// Physics
	//=========

	if (UseGravity_)
	{
		GroundedCheck();
		ApplyGravity();
	}

	//===========
	// Debugging -- REMOVE LATER
	//===========

	if (InputManager::Instance()->GetKeyDown(VK_O))
	{
		SetGravity(!UseGravity_);
	}

	Camera::Instance()->Frame();

	return true;
}

void Player::GroundedCheck()
{
	// Get the block we are currently in
	Block* beneathBlock = VoxelWorld::Instance()->GetBlock(Transform_->GetPosition().x, Transform_->GetPosition().y - 1, Transform_->GetPosition().z);
	if (beneathBlock)
	{
		// Make sure the block beneath us is solid
		if (beneathBlock->IsSolid())
		{
			GroundLevel_ = beneathBlock->GetInstance().position.y + 1.0f;

			// Check if we are grounded
			if (Transform_->GetPosition().y - Physics::Gravity <= GroundLevel_)
			{
				Transform_->SetY(GroundLevel_);

				IsGrounded_ = true;
				return;
			}
		}
	}

	IsGrounded_ = false;
}

void Player::ApplyGravity()
{
	if (IsGrounded_)
		return;

	// Apply Gravity
	Transform_->MoveY(-Physics::Gravity * PerformanceManager::Instance()->GetDeltaTime());

	// Align Camera
	Camera::Instance()->GetTransform()->SetPosition(Transform_->GetPosition() + D3DXVECTOR3(0, Height_, 0));
}

void Player::HandleMovement()
{
	// Reset Movement Velocity
	MoveVelocity_ = D3DXVECTOR3(0, 0, 0);
	
	//====================
	// Check for Movement
	//====================

	// Forward / Backward
	HandleMovementKey(VK_W, Transform_->GetForwardVector());
	HandleMovementKey(VK_S, -Transform_->GetForwardVector());

	// Left / Right
	HandleMovementKey(VK_A, -Transform_->GetRightVector());
	HandleMovementKey(VK_D, Transform_->GetRightVector());

	//====================
	// Apply to Character
	//====================
	
	// Move Character
	Transform_->Move(MoveVelocity_ * MoveSpeed_ * PerformanceManager::Instance()->GetDeltaTime());

	// Update Camera Position
	Camera::Instance()->GetTransform()->SetPosition(Transform_->GetPosition() + D3DXVECTOR3(0, Height_, 0));
}

void Player::HandleMovementKey(unsigned int key, D3DXVECTOR3 moveAmount)
{
	if (!InputManager::Instance()->GetKey(key))
	{
		return;
	}

	MoveVelocity_ += moveAmount;
}

void Player::HandleLooking()
{
	float deltaX, deltaY;

	if (!WindowActive())
	{
		return;
	}

	// Check if we have changed where we are looking
	if (InputManager::Instance()->GetMousePosOnScreen() == System::CentreScreen)
	{
		return;
	}

	// Calculate the difference between the position of the mouse and the middle of the screen
	deltaX = (InputManager::Instance()->GetMousePosOnScreen().x - System::CentreScreen.x) / LookSpeed_;
	deltaY = (InputManager::Instance()->GetMousePosOnScreen().y - System::CentreScreen.y) / LookSpeed_;

	// Rotate the Player in Y
	Transform_->RotateY(deltaX);

	// Rotate the Camera in X and Y
	Camera::Instance()->GetTransform()->RotateX(deltaY);
	Camera::Instance()->GetTransform()->RotateY(deltaX);

	// Keep Mouse in Center Screen
	LockMouseToCenter();
}