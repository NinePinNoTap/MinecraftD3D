#include "Player.h"
#include "VoxelWorld.h"

Player::Player() : GameObject()
{
	Height_ = 1;
	UseGravity_ = false;
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
	JumpPower_ = 15.0f;
	MoveVelocity_ = Vector::Zero;

	return true;
}

bool Player::Frame()
{
	//=================
	// Handle Controls
	//=================

	HandleMovement();
	HandleLooking();
	HandlePhysics();

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

void Player::HandleMovement()
{
	// Reset velocity
	MoveVelocity_.x = MoveVelocity_.z = 0.0f;

	//=================
	// Check for Input
	//=================

	// Forward / Backward
	HandleMovementKey(VK_W, Transform_->GetForwardVector());
	HandleMovementKey(VK_S, -Transform_->GetForwardVector());

	// Left / Right
	HandleMovementKey(VK_A, -Transform_->GetRightVector());
	HandleMovementKey(VK_D, Transform_->GetRightVector());

	if (IsGrounded_)
	{
		HandleMovementKey(VK_SPACE, Transform_->GetUpVector() * JumpPower_);
	}
	else
	{
		// Gravity
		if (!UseGravity_)
		{
			return;
		}

		// Apply Gravity
		MoveVelocity_.y -= Physics::Gravity;
	}

	if (MoveVelocity_.y < -9)
	{
		MoveVelocity_.y = -9;
	}

	// Check for Ground
	GroundCheck();
}

void Player::HandleMovementKey(unsigned int key, D3DXVECTOR3 moveAmount)
{
	if (!InputManager::Instance()->GetKey(key))
	{
		return;
	}

	MoveVelocity_ += (moveAmount * MoveSpeed_);
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

void Player::HandlePhysics()
{
	D3DXVECTOR3 nextStep, blockPos;
	Block* blockNextStep;
	BoundingBox boundingBox;
	float lowestDistance, currentDistance;
	int solutionID;
	
	// Calculate next position
	nextStep = Transform_->GetPosition() + MoveVelocity_ * PerformanceManager::Instance()->GetDeltaTime();

	// Apply to Transforms -- NEED TO FIX PHYSICS
	UpdatePosition(nextStep);
	return;



	







	// Check if theres a block in the next position
	blockNextStep = VoxelWorld::Instance()->GetBlock(nextStep.x, nextStep.y, nextStep.z);
	if (!blockNextStep)
	{
		// Apply to Transforms
		UpdatePosition(nextStep);
		return;
	}

	// Make sure the block is solid
	if (!blockNextStep->IsSolid())
	{
		// Apply to Transforms
		UpdatePosition(nextStep);
		return;
	}

	//===================
	// Handle Collisions
	//===================

	Round(nextStep.x, 1);
	Round(nextStep.y, 1);
	Round(nextStep.z, 1);

	// Make it easier for readability
	blockPos = blockNextStep->GetInstance().position;
	boundingBox = blockNextStep->GetBoundingBox();

	// Calculate face centres
	D3DXVECTOR3 boxPositions[6] = {
		D3DXVECTOR3(boundingBox.left, blockPos.y, blockPos.z),
		D3DXVECTOR3(boundingBox.right, blockPos.y, blockPos.z),
		D3DXVECTOR3(blockPos.x, boundingBox.top, blockPos.z),
		D3DXVECTOR3(blockPos.x, boundingBox.bottom, blockPos.z),
		D3DXVECTOR3(blockPos.x, blockPos.y, boundingBox.front),
		D3DXVECTOR3(blockPos.x, blockPos.y, boundingBox.back),
	};

	// Calculate possible outcomes
	D3DXVECTOR3 collisionOutcomes[6] = { 
		D3DXVECTOR3(boundingBox.left,   nextStep.y,			nextStep.z), 
		D3DXVECTOR3(boundingBox.right,  nextStep.y,			nextStep.z),
		D3DXVECTOR3(nextStep.x,			boundingBox.top,	nextStep.z),
		D3DXVECTOR3(nextStep.x,			boundingBox.bottom, nextStep.z),
		D3DXVECTOR3(nextStep.x,			nextStep.y,			boundingBox.front),
		D3DXVECTOR3(nextStep.x,			nextStep.y,			boundingBox.back),
	};

	// Start with a high number
	lowestDistance = RAND_MAX;

	// Check if we are colliding with the box
	if (CheckCollision(boundingBox, nextStep))
	{
		// Find the intersecting side
		for (int i = 0; i < 6; i++)
		{
			currentDistance = Distance(nextStep, boxPositions[i]);
			if (currentDistance < lowestDistance)
			{
				// Store lowest
				lowestDistance = currentDistance;

				// Save index
				solutionID = i;
			}
		}

		// Update to solution
		nextStep = collisionOutcomes[solutionID];
	}

	// Apply to Transforms
	UpdatePosition(nextStep);
}

void Player::GroundCheck()
{
	D3DXVECTOR3 nextStep;
	Block* blockNextStep;

	// Reset Flag
	IsGrounded_ = false;

	// Calculate next step
	nextStep = Transform_->GetPosition() + MoveVelocity_ * PerformanceManager::Instance()->GetDeltaTime();

	// Get next step block
	blockNextStep = VoxelWorld::Instance()->GetBlock(nextStep.x, nextStep.y, nextStep.z);
	if (!blockNextStep)
	{
		return;
	}

	// Ensure its not air
	if (!blockNextStep->IsSolid())
	{
		return;
	}

	// Round to 1 decimal place
	Round(nextStep.y, 1);

	// Check if we are colliding
	if (nextStep.y < blockNextStep->GetBoundingBox().top)
	{
		MoveVelocity_.y = 0.0f;
		IsGrounded_ = true;
	}
}

void Player::UpdatePosition(D3DXVECTOR3 playerPos)
{
	// Set feet position
	Transform_->SetPosition(playerPos);

	// Set head position
	Camera::Instance()->GetTransform()->SetPosition(Transform_->GetPosition() + D3DXVECTOR3(0, Height_, 0));
}