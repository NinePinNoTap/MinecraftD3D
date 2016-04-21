#pragma once

#include <D3DX11.h>
#include <d3dx10math.h>

#include "GameObject.h"
#include "InputManager.h"
#include "VirtualKeys.h"
#include "Singleton.h"
#include "Utilities.h"

class Camera : public GameObject, public Singleton<Camera>
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	// Initialising
	bool Initialise();

	// Frame
	bool Frame();

	// Matrices
	bool Render();
	void GetViewMatrix(D3DXMATRIX& ViewMatrix);
	void Render2DViewMatrix();
	void Get2DViewMatrix(D3DXMATRIX& ViewMatrix);
	void RenderReflection(float);
	void GetReflectionMatrix(D3DXMATRIX& ViewMatrix);

	// Setters
	void SetStartPosition(float x, float y, float z);
	void SetStartRotation(float x, float y, float z);
	void SetSpeed(float speed);
	void Reset();
	void AllowFlying(bool flag);
	void AllowLooking(bool flag);
	void AllowMovement(bool flag);

	// Getters
	float GetMovementSpeed();
	bool IsMoving();

private:
	// Updating
	void HandleMouse();

	// Generation
	D3DXMATRIX GenerateMatrix(D3DXVECTOR3 upVector, D3DXVECTOR3 positionVector, D3DXVECTOR3 lookAtVector, bool isReflection = false);

	// Vectors
	D3DXVECTOR3 StartPosition_;
	D3DXVECTOR3 StartRotation_;
	D3DXVECTOR3 Position2D_;
	D3DXVECTOR3 Velocity_;

	// Matrices
	D3DXMATRIX ViewMatrix_;
	D3DXMATRIX ViewMatrix2D_;
	D3DXMATRIX ReflectionViewMatrix_;

	// Flags
	bool CanFreeRoam_;
	bool CanLook_;
	bool CanMove_;

	// Properties
	float MoveSpeed_;
};


