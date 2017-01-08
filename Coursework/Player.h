#pragma once

#include "GameObject.h"
#include "Block.h"

class Player : public GameObject
{
public:
	Player();
	~Player();

	bool initialise();
	bool update();

	inline void setHeight(float height)
	{
		m_height = height;
	}
	inline void setGravity(bool flag = true)
	{
		m_useGravity = flag;
		m_moveVelocity.y = 0.0f;
	}
	inline void setmoveSpeed(float moveSpeed)
	{
		m_moveSpeed = moveSpeed;
	}
	inline void setLookSpeed(float lookSpeed)
	{
		m_lookSpeed = lookSpeed;
	}

private:
	void handlemovement();
	void handleLooking();
	void handlePhysics();
	void GroundCheck();
	void handlemovementKey(unsigned int key, D3DXVECTOR3 moveAmount);
	void updatePosition(D3DXVECTOR3 playerPos);

	// Attributes
	float m_height;
	float m_moveSpeed;
	float m_lookSpeed;
	D3DXVECTOR3 m_moveVelocity;

	// Jumping
	float m_jumpPower;
	bool m_grounded;

	// Physics
	bool m_useGravity;
};
