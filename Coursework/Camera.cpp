#include "Camera.h"

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
void Camera::Initialise()
{
	//=======================
	// Initialise Properties
	//=======================

	// Create the transform
	Transform_ = new Transform;

	// Set default transform
	SetStartPosition(0, 0, 0);
	
	// Set the starting move speed
	SetSpeed(1.0f);

	// Set default move velocity
	Velocity_ = ZERO;

	// Set Base Vectors
	Position2D_ = D3DXVECTOR3(0, 0, -10);

	//=========================
	// Generate 2D View Matrix
	//=========================

	Render2DViewMatrix();

	//==================
	// Initialise Flags
	//==================

	CanFreeRoam_ = true;
	CanLook_ = true;
}

// Frame
bool Camera::Frame()
{
	// Make sure we are currently on the ApplicationManager to register key presses
	if (!WindowActive())
		return true;

	if (!CanMove_)
		return true;

	// Default Velocity
	Velocity_ = D3DXVECTOR3(0, 0, 0);

	//=================
	// Camera Movement
	//=================

	// Forward / Backward
	if (InputManager::Instance()->GetKey(VK_W))
	{
		Velocity_ += Transform_->GetForwardVector() * MoveSpeed_;
	}
	else if (InputManager::Instance()->GetKey(VK_S))
	{
		Velocity_ += Transform_->GetForwardVector() * -MoveSpeed_;
	}

	// Left / Right
	if (InputManager::Instance()->GetKey(VK_A))
	{
		Velocity_ += Transform_->GetRightVector() * -MoveSpeed_;
	}
	else if (InputManager::Instance()->GetKey(VK_D))
	{
		Velocity_ += Transform_->GetRightVector() * MoveSpeed_;
	}

	// Check if we can fly
	if (CanFreeRoam_)
	{
		// Up / Down
		if (InputManager::Instance()->GetKey(VK_SPACE))
		{
			Velocity_ += Transform_->GetUpVector() * MoveSpeed_;
		}
		else if (InputManager::Instance()->GetKey(VK_SHIFT))
		{
			Velocity_ += Transform_->GetUpVector() * -MoveSpeed_;
		}
	}
	else
	{
		Velocity_.y = 0;
	}

	// Apply velocities to the camera
	Transform_->Move(Velocity_);

	//========================
	// Center Mouse On Screen
	//========================

	if (CanLook_)
	{
		HandleMouse();
	}

	return true;
}

void Camera::HandleMouse()
{
	// Get the position of the mouse
	POINT MousePos;
	GetCursorPos(&MousePos);

	// Calculate the middle of the screen
	static int CenterScreenX = GetSystemMetrics(SM_CXSCREEN) / 2;
	static int CenterScreenY = GetSystemMetrics(SM_CYSCREEN) / 2;

	// Calculate a combine x/y position for mouse and screen
	int MouseTotal = MousePos.x + MousePos.y;
	int ScreenTotal = CenterScreenX + CenterScreenY;

	// Check if the mouse has moved off from the center of the screen
	if (MouseTotal != ScreenTotal)
	{
		// Calculate the difference between the position of the mouse and the middle of the screen
		float DeltaX = (MousePos.x - CenterScreenX) / CAMERA_SENSITIVITY;
		float DeltaY = (MousePos.y - CenterScreenY) / CAMERA_SENSITIVITY;

		// Adjust the rotation based on the above movement
		Transform_->Rotate(DeltaY, DeltaX, 0);

		// Keep Mouse in Center Screen
		LockMouseToCenter();
	}
}

// Rendering
bool Camera::Render()
{
	// Create the view matrix
	ViewMatrix_ = GenerateMatrix(UP_VECTOR, Transform_->GetPosition(), FORWARD_VECTOR);

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
	ViewMatrix2D_ = GenerateMatrix(UP_VECTOR, Position2D_, FORWARD_VECTOR);

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
	ReflectionViewMatrix_ = GenerateMatrix(UP_VECTOR, ReflectedPosition, FORWARD_VECTOR, true);

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

// Setters
void Camera::SetStartPosition(float x, float y, float z)
{
	// Store the starting position for later
	StartPosition_ = D3DXVECTOR3(x, y, z);

	// Set our initial position to the start
	Transform_->SetPosition(x, y, z);
}

void Camera::SetStartRotation(float x, float y, float z)
{
	StartRotation_ = D3DXVECTOR3(x, y, z);

	Transform_->SetRotation(x, y, z);
}

void Camera::SetSpeed(float speed)
{
	// Set the speed the camera moves at
	MoveSpeed_ = speed;
}

void Camera::Reset()
{
	// Store the starting positions
	Transform_->SetPosition(StartPosition_);
	Transform_->SetRotation(0, 0, 0);
}

// Flags
void Camera::AllowFlying(bool flag)
{
	CanFreeRoam_ = flag;
}

void Camera::AllowLooking(bool flag)
{
	CanLook_ = flag;
}

void Camera::AllowMovement(bool flag)
{
	CanMove_ = flag;
}

float Camera::GetMovementSpeed()
{
	return MoveSpeed_;
}

bool Camera::IsMoving()
{
	return D3DXVec3Length(&Velocity_) != 0;
}