#include "Player.h"
#include "VoxelWorld.h"

Player::Player() : GameObject()
{
	m_height = 1;
	m_useGravity = false;
}

Player::~Player()
{

}

bool Player::initialise()
{
	// Call parent initialise
	GameObject::initialise();

	// Define begin Position
	m_transform->setPosition(0, 98, 0);
	Camera::getInstance()->getTransform()->setPosition(m_transform->getPosition() + D3DXVECTOR3(0, m_height, 0));

	// Define Vars
	m_lookSpeed = 10.0f;
	m_moveSpeed = 4.137f;
	m_jumpPower = 15.0f;
	m_moveVelocity = Vector::Zero;

	return true;
}

bool Player::update()
{
	//=================
	// handle Controls
	//=================

	handlemovement();
	handleLooking();
	handlePhysics();

	//===========
	// Debugging -- REMOVE LATER
	//===========

	if (InputManager::getInstance()->getKeyDown(VK_O))
	{
		setGravity(!m_useGravity);
	}

	Camera::getInstance()->update();

	return true;
}

void Player::handlemovement()
{
	// Reset velocity
	m_moveVelocity.x = m_moveVelocity.z = 0.0f;

	//=================
	// Check for Input
	//=================

	// Forward / Backward
	handlemovementKey(VK_W, m_transform->getForwardVector());
	handlemovementKey(VK_S, -m_transform->getForwardVector());

	// Left / Right
	handlemovementKey(VK_A, -m_transform->getRightVector());
	handlemovementKey(VK_D, m_transform->getRightVector());

	if (m_grounded)
	{
		handlemovementKey(VK_SPACE, m_transform->getUpVector() * m_jumpPower);
		
		// Sprint
		if (InputManager::getInstance()->getKey(VK_SHIFT))
		{
			m_moveVelocity.x *= 2;
			m_moveVelocity.z *= 2;
		}
	}
	else
	{
		// Gravity
		if (!m_useGravity)
		{
			return;
		}

		// Apply Gravity
		m_moveVelocity.y -= Physics::Gravity;
	}

	if (m_moveVelocity.y < -9)
	{
		m_moveVelocity.y = -9;
	}

	// Check for Ground
	GroundCheck();
}

void Player::handlemovementKey(unsigned int key, D3DXVECTOR3 moveAmount)
{
	if (!InputManager::getInstance()->getKey(key))
	{
		return;
	}

	m_moveVelocity += (moveAmount * m_moveSpeed);
}

void Player::handleLooking()
{
	float deltaX, deltaY;

	if (!windowActive())
	{
		return;
	}

	// Check if we have changed where we are looking
	if (InputManager::getInstance()->getMousePosOnScreen() == System::CentreScreen)
	{
		return;
	}

	// Calculate the difference between the position of the mouse and the middle of the screen
	deltaX = (InputManager::getInstance()->getMousePosOnScreen().x - System::CentreScreen.x) / m_lookSpeed;
	deltaY = (InputManager::getInstance()->getMousePosOnScreen().y - System::CentreScreen.y) / m_lookSpeed;

	// rotate the Player in Y
	m_transform->rotateY(deltaX);

	// rotate the Camera in X and Y
	Camera::getInstance()->getTransform()->rotateX(deltaY);
	Camera::getInstance()->getTransform()->rotateY(deltaX);

	// Keep Mouse in Center Screen
	lockMouseToCenter();
}

void Player::handlePhysics()
{
	D3DXVECTOR3 nextStep, blockPos;
	Block* blockNextStep;
	BoundingBox boundingBox;
	float lowestDistance, currentDistance;
	int solutionID;
	
	// Calculate next position
	nextStep = m_transform->getPosition() + m_moveVelocity * PerformanceManager::getInstance()->getDeltaTime();

	// Apply to Transforms -- NEED TO FIX PHYSICS
	updatePosition(nextStep);
	return;



	







	// Check if theres a block in the next position
	blockNextStep = VoxelWorld::getInstance()->getBlock(nextStep.x, nextStep.y, nextStep.z);
	if (!blockNextStep)
	{
		// Apply to Transforms
		updatePosition(nextStep);
		return;
	}

	// Make sure the block is solid
	if (!blockNextStep->isSolid())
	{
		// Apply to Transforms
		updatePosition(nextStep);
		return;
	}

	//===================
	// handle Collisions
	//===================

	roundFloat(nextStep.x, 1);
	roundFloat(nextStep.y, 1);
	roundFloat(nextStep.z, 1);

	// Make it easier for readability
	blockPos = blockNextStep->getInstance().position;
	boundingBox = blockNextStep->getBoundingBox();

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

	// begin with a high number
	lowestDistance = RAND_MAX;

	// Check if we are colliding with the box
	if (checkCollision(boundingBox, nextStep))
	{
		// Find the intersecting side
		for (int i = 0; i < 6; i++)
		{
			currentDistance = distance(nextStep, boxPositions[i]);
			if (currentDistance < lowestDistance)
			{
				// Store lowest
				lowestDistance = currentDistance;

				// Save index
				solutionID = i;
			}
		}

		// update to solution
		nextStep = collisionOutcomes[solutionID];
	}

	// Apply to Transforms
	updatePosition(nextStep);
}

void Player::GroundCheck()
{
	D3DXVECTOR3 nextStep;
	Block* blockNextStep;

	// Reset Flag
	m_grounded = false;

	// Check whats beneath us
	nextStep = m_transform->getPosition();// +m_moveVelocity * PerformanceManager::getInstance()->getDeltaTime();
	nextStep.y -= 1;

	// get next step block
	blockNextStep = VoxelWorld::getInstance()->getBlock(nextStep.x, nextStep.y, nextStep.z);
	if (!blockNextStep)
	{
		return;
	}

	// Ensure its not air
	if (!blockNextStep->isSolid())
	{
		return;
	}

	// round to 1 decimal place
	roundFloat(nextStep.y, 1);

	// Check if we are colliding
	if (nextStep.y < blockNextStep->getBoundingBox().top)
	{
		m_moveVelocity.y = 0.0f;
		m_grounded = true;
	}
}

void Player::updatePosition(D3DXVECTOR3 playerPos)
{
	// set feet position
	m_transform->setPosition(playerPos);

	// set head position
	Camera::getInstance()->getTransform()->setPosition(m_transform->getPosition() + D3DXVECTOR3(0, m_height, 0));
}