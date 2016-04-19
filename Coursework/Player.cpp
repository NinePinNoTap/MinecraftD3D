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
	GameObject::Initialise();

	// Define Start
	Transform_->SetPosition(0, 98, 0);
	Camera::Instance()->GetTransform()->SetPosition(Transform_->GetPosition() + D3DXVECTOR3(0, Height_, 0));

	return true;
}

bool Player::Frame()
{
	// Make sure the player is up to date with x/z (Need this until we have controls here)
	Transform_->SetX(Camera::Instance()->GetTransform()->GetPosition().x);
	Transform_->SetZ(Camera::Instance()->GetTransform()->GetPosition().z);

	// Set Gravity
	GravityAmount_ = 9.806f * PerformanceManager::Instance()->GetDeltaTime();

	// Apply Physics
	if (UseGravity_)
	{
		ApplyGravity();
	}

	// Debugging
	if (InputManager::Instance()->GetKeyDown(VK_O))
	{
		SetGravityOn();
	}

	return true;
}

void Player::GroundedCheck()
{
	int x, y, z;

	// Get current position
	x = Transform_->GetPosition().x;
	y = Transform_->GetPosition().y;
	z = Transform_->GetPosition().z;

	// Get the block we are currently in
	Block* beneathBlock = VoxelWorld::Instance()->GetBlock(x, y - 1, z);
	if (beneathBlock)
	{
		// Make sure the block beneath us is solid
		if (beneathBlock->IsSolid())
		{
			GroundLevel_ = beneathBlock->GetInstance().position.y + 1.0f;

			// Check if we are grounded
			if (Transform_->GetPosition().y - GravityAmount_ <= GroundLevel_)
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
	// Check if we are grounded
	GroundedCheck();

	if (IsGrounded_)
		return;

	// Apply Gravity
	Transform_->MoveY(-GravityAmount_);

	// Align Camera
	Camera::Instance()->GetTransform()->SetPosition(Transform_->GetPosition() + D3DXVECTOR3(0, Height_, 0));
}

void Player::SetHeight(float height)
{
	Height_ = height;
}