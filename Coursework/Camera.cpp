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
bool Camera::initialise()
{
	//=======================
	// initialise Properties
	//=======================

	// create the transform
	m_transform = new Transform;
	
	// set position to view 2D objects
	m_position2D = D3DXVECTOR3(0, 0, -10);

	//=========================
	// generate 2D View Matrix
	//=========================

	render2DViewMatrix();

	return true;
}

// update
bool Camera::update()
{
	// render the view matrix
	render();

	return true;
}

// rendering
bool Camera::render()
{
	// create the view matrix
	m_viewMatrix = generateMatrix(Vector::Up, m_transform->getPosition(), Vector::Forward);

	return true;
}

void Camera::getViewMatrix(D3DXMATRIX& viewMatrix)
{
	// Return the view matrix
	viewMatrix = m_viewMatrix;

	return;
}

void Camera::render2DViewMatrix()
{
	// generate the 2D view matrix
	m_baseViewMatrix = generateMatrix(Vector::Up, m_position2D, Vector::Forward);

	return;
}

void Camera::get2DViewMatrix(D3DXMATRIX& viewMatrix)
{
	// Return the 2D view matrix
	viewMatrix = m_baseViewMatrix;

	return;
}

void Camera::renderReflection(float ReflectionHeight)
{
	D3DXVECTOR3 ReflectedPosition;

	// Retrieve our current position
	ReflectedPosition = m_transform->getPosition();

	// Reflect the Y-Axis
	ReflectedPosition.y = -ReflectedPosition.y + (ReflectionHeight * 2.0f);

	// generate the reflection matrix
	m_reflectionViewMatrix = generateMatrix(Vector::Up, ReflectedPosition, Vector::Forward, true);

	return;
}

void Camera::getReflectionMatrix(D3DXMATRIX& viewMatrix)
{
	// Return the reflection matrix
	viewMatrix = m_reflectionViewMatrix;

	return;
}

D3DXMATRIX Camera::generateMatrix(D3DXVECTOR3 UpVector, D3DXVECTOR3 PositionVector, D3DXVECTOR3 LookAtVector, bool isReflection)
{
	float Yaw;
	float Pitch;
	float Roll;
	D3DXMATRIX RotationMatrix; 
	D3DXMATRIX ViewMatrix; 

	//==========================
	// create a Rotation Matrix
	//==========================

	// Convert each axis from degrees to radians
	Pitch = D3DXToRadian(m_transform->getPitch());
	Yaw = D3DXToRadian(m_transform->getYaw());
	Roll = D3DXToRadian(m_transform->getRoll());

	// Invert the pitch if we are reflecting
	if (isReflection)
	{
		Pitch *= -1;
	}
	
	// generate the rotation matrix
	D3DXMatrixRotationYawPitchRoll(&RotationMatrix, Yaw, Pitch, Roll);

	//===========================
	// create the Look At Vector
	//===========================

	// Calculate the forward vector
	D3DXVec3TransformCoord(&LookAtVector, &LookAtVector, &RotationMatrix);

	// Calculate the up vector
	D3DXVec3TransformCoord(&UpVector, &UpVector, &RotationMatrix);

	// move the position of the camera to the viewer
	LookAtVector = PositionVector + LookAtVector;

	// create our final view matrix
	D3DXMatrixLookAtLH(&ViewMatrix, &PositionVector, &LookAtVector, &UpVector);

	return ViewMatrix;
}