#include "Transform.h"

Transform::Transform()
{
	SetPosition(0, 0, 0);
	SetRotation(0, 0, 0);
	SetScale(1, 1, 1);
}

Transform::~Transform()
{
}


// Overall Setters
void Transform::SetPosition(D3DXVECTOR3 Position)
{
	Position_ = Position;
}

void Transform::SetRotation(D3DXVECTOR3 Rotation)
{
	Rotation_ = Rotation;
}

void Transform::SetScale(D3DXVECTOR3 Scale)
{
	Scale_ = Scale;
}

void Transform::SetPosition(float x, float y, float z)
{
	Position_ = D3DXVECTOR3(x, y, z);
}

void Transform::SetRotation(float x, float y, float z)
{
	Rotation_ = D3DXVECTOR3(x, y, z);

	// Keep rotation values between 0 and 360
	Wrap(Rotation_.x, 0, 360);
	Wrap(Rotation_.y, 0, 360);
	Wrap(Rotation_.z, 0, 360);
}

void Transform::SetScale(float x, float y, float z)
{
	Scale_ = D3DXVECTOR3(x, y, z);
}

// Individual Position Setters
void Transform::SetX(float x)
{
	Position_.x = x;
}

void Transform::SetY(float y)
{
	Position_.y = y;
}

void Transform::SetZ(float z)
{
	Position_.z = z;
}

// Individual Rotation Setters
void Transform::SetRotationX(float x)
{
	Rotation_.x = x;
}

void Transform::SetRotationY(float y)
{
	Rotation_.y = y;
}

void Transform::SetRotationZ(float z)
{
	Rotation_.z = z;
}

// Overall Changer
void Transform::Move(D3DXVECTOR3 amount)
{
	Position_ += amount;
}

void Transform::Rotate(D3DXVECTOR3 amount)
{
	Rotation_ += amount;

	// Keep rotation values between 0 and 360
	Wrap(Rotation_.x, 0, 360);
	Wrap(Rotation_.y, 0, 360);
	Wrap(Rotation_.z, 0, 360);
}

void Transform::Move(float x, float y, float z)
{
	Position_.x += x;
	Position_.y += y;
	Position_.z += z;
}

void Transform::Rotate(float x, float y, float z)
{
	Rotation_.x += x;
	Rotation_.y += y;
	Rotation_.z += z;

	// Keep rotation values between 0 and 360
	Wrap(Rotation_.x, 0, 360);
	Wrap(Rotation_.y, 0, 360);
	Wrap(Rotation_.z, 0, 360);
}

// Individual Position Changer
void Transform::MoveX(float x)
{
	Position_.x += x;
}

void Transform::MoveY(float y)
{
	Position_.y += y;
}

void Transform::MoveZ(float z)
{
	Position_.z += z;
}

// Individual Rotation Changer
void Transform::RotateX(float x)
{
	Rotation_.x += x;

	// Make sure rotation is between 0 and 360
	Wrap(Rotation_.x, 0, 360);
}

void Transform::RotateY(float y)
{
	Rotation_.y += y;

	// Make sure rotation is between 0 and 360
	Wrap(Rotation_.y, 0, 360);
}

void Transform::RotateZ(float z)
{
	Rotation_.z += z;

	// Make sure rotation is between 0 and 360
	Wrap(Rotation_.z, 0, 360);
}

// Transformation Getters
D3DXVECTOR3 Transform::GetPosition()
{
	return Position_;
}

D3DXVECTOR3 Transform::GetRotation()
{
	return Rotation_;
}

D3DXVECTOR3 Transform::GetScale()
{
	return Scale_;
}

// Vector Getters
D3DXVECTOR3 Transform::GetForwardVector()
{
	D3DXVECTOR3 ForwardVector;
	D3DXMATRIX RotationMatrix;

	//===========================
	// Create the Forward Vector
	//===========================

	// Create a rotation matrix based on pitch/yaw/roll
	GetRotationMatrix(RotationMatrix);

	// Point in Z-Axis

	// Apply rotation to the vector
	D3DXVec3TransformCoord(&ForwardVector, &FORWARD_VECTOR, &RotationMatrix);

	return ForwardVector;
}

D3DXVECTOR3 Transform::GetRightVector()
{
	D3DXVECTOR3 RightVector;
	D3DXMATRIX RotationMatrix;

	//=========================
	// Create the Right Vector
	//=========================

	// Create a rotation matrix based on pitch/yaw/roll
	GetRotationMatrix(RotationMatrix);

	// Apply rotation to the vector
	D3DXVec3TransformCoord(&RightVector, &RIGHT_VECTOR, &RotationMatrix);

	return RightVector;
}

D3DXVECTOR3 Transform::GetUpVector()
{
	D3DXVECTOR3 UpVector;
	D3DXMATRIX RotationMatrix;

	//======================
	// Create the Up Vector
	//======================

	// Create a rotation matrix based on pitch/yaw/roll
	GetRotationMatrix(RotationMatrix);

	// Apply rotation to the vector
	D3DXVec3TransformCoord(&UpVector, &UP_VECTOR, &RotationMatrix);

	return UpVector;
}

// Individual Position Getters
float Transform::GetX()
{
	return Position_.x;
}

float Transform::GetY()
{
	return Position_.y;
}

float Transform::GetZ()
{
	return Position_.z;
}

// Individual Rotation Getters
float Transform::GetPitch()
{
	return Rotation_.x;
}

float Transform::GetYaw()
{
	return Rotation_.y;
}

float Transform::GetRoll()
{
	return Rotation_.z;
}

// Matrix Getters
void Transform::GetTranslationMatrix(D3DXMATRIX& TranslationMatrix)
{
	D3DXMatrixTranslation(&TranslationMatrix, Position_.x, Position_.y, Position_.z);
}

void Transform::GetRotationMatrix(D3DXMATRIX& RotationMatrix)
{
	// Convert degrees to radians
	float Pitch = D3DXToRadian(Rotation_.x);
	float Yaw = D3DXToRadian(Rotation_.y);
	float Roll = D3DXToRadian(Rotation_.z);

	D3DXMatrixRotationYawPitchRoll(&RotationMatrix, Yaw, Pitch, Roll);
}

void Transform::GetScaleMatrix(D3DXMATRIX& ScaleMatrix)
{
	// Crate matrices using the vectors
	D3DXMatrixScaling(&ScaleMatrix, Scale_.x, Scale_.y, Scale_.z);
}

void Transform::GetWorldMatrix(D3DXMATRIX& WorldMatrix)
{
	D3DXMATRIX ScaleMatrix;
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX TranslationMatrix;

	// Retrieve each transformation matrix
	GetScaleMatrix(ScaleMatrix);
	GetRotationMatrix(RotationMatrix);
	GetTranslationMatrix(TranslationMatrix);

	// Apply each matrix to the world matrix
	D3DXMatrixMultiply(&WorldMatrix, &WorldMatrix, &ScaleMatrix);
	D3DXMatrixMultiply(&WorldMatrix, &WorldMatrix, &RotationMatrix);
	D3DXMatrixMultiply(&WorldMatrix, &WorldMatrix, &TranslationMatrix);
}

