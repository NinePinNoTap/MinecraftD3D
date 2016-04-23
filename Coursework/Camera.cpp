#include "Camera.h"
#include "PerformanceManager.h"

Camera::Camera() : GameObject()
{
}

Camera::Camera(const Camera& other)
{
}

Camera::~Camera()
{
}

// Initialising
bool Camera::Initialise()
{
	//=======================
	// Initialise Properties
	//=======================

	// Create the transform
	Transform_ = new Transform;
	
	// Set position to view 2D objects
	Position2D_ = D3DXVECTOR3(0, 0, -10);

	//=========================
	// Generate 2D View Matrix
	//=========================

	Render2DViewMatrix();

	return true;
}

// Frame
bool Camera::Frame()
{
	// Render the view matrix
	Render();

	return true;
}

// Rendering
bool Camera::Render()
{
	// Create the view matrix
	ViewMatrix_ = GenerateMatrix(Vector::Up, Transform_->GetPosition(), Vector::Forward);

	return true;
}

void Camera::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	// Return the view matrix
	viewMatrix = ViewMatrix_;

	return;
}

void Camera::Render2DViewMatrix()
{
	// Generate the 2D view matrix
	ViewMatrix2D_ = GenerateMatrix(Vector::Up, Position2D_, Vector::Forward);

	return;
}

void Camera::Get2DViewMatrix(D3DXMATRIX& viewMatrix)
{
	// Return the 2D view matrix
	viewMatrix = ViewMatrix2D_;

	return;
}

void Camera::RenderReflection(float ReflectionHeight)
{
	D3DXVECTOR3 ReflectedPosition;

	// Retrieve our current position
	ReflectedPosition = Transform_->GetPosition();

	// Reflect the Y-Axis
	ReflectedPosition.y = -ReflectedPosition.y + (ReflectionHeight * 2.0f);

	// Generate the reflection matrix
	ReflectionViewMatrix_ = GenerateMatrix(Vector::Up, ReflectedPosition, Vector::Forward, true);

	return;
}

void Camera::GetReflectionMatrix(D3DXMATRIX& viewMatrix)
{
	// Return the reflection matrix
	viewMatrix = ReflectionViewMatrix_;

	return;
}

D3DXMATRIX Camera::GenerateMatrix(D3DXVECTOR3 UpVector, D3DXVECTOR3 PositionVector, D3DXVECTOR3 LookAtVector, bool isReflection)
{
	float Yaw;
	float Pitch;
	float Roll;
	D3DXMATRIX RotationMatrix; 
	D3DXMATRIX ViewMatrix; 

	//==========================
	// Create a Rotation Matrix
	//==========================

	// Convert each axis from degrees to radians
	Pitch = D3DXToRadian(Transform_->GetPitch());
	Yaw = D3DXToRadian(Transform_->GetYaw());
	Roll = D3DXToRadian(Transform_->GetRoll());

	// Invert the pitch if we are reflecting
	if (isReflection)
	{
		Pitch *= -1;
	}
	
	// Generate the rotation matrix
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix, Yaw, Pitch, Roll);

	//===========================
	// Create the Look At Vector
	//===========================

	// Calculate the forward vector
	D3DXVec3TransformCoord(&LookAtVector, &LookAtVector, &RotationMatrix);

	// Calculate the up vector
	D3DXVec3TransformCoord(&UpVector, &UpVector, &RotationMatrix);

	// Move the position of the camera to the viewer
	LookAtVector = PositionVector + LookAtVector;

	// Create our final view matrix
	D3DXMatrixLookAtLH(&ViewMatrix, &PositionVector, &LookAtVector, &UpVector);

	return ViewMatrix;
}