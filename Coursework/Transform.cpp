#include "Transform.h"

Transform::Transform()
{
	setPosition(0, 0, 0);
	setRotation(0, 0, 0);
	setScale(1, 1, 1);
}

Transform::~Transform()
{
}


// Overall setters
void Transform::setPosition(D3DXVECTOR3 Position)
{
	m_position = Position;
}

void Transform::setRotation(D3DXVECTOR3 Rotation)
{
	m_rotation = Rotation;
}

void Transform::setScale(D3DXVECTOR3 Scale)
{
	m_scale = Scale;
}

void Transform::setPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR3(x, y, z);
}

void Transform::setRotation(float x, float y, float z)
{
	m_rotation = D3DXVECTOR3(x, y, z);

	// Keep rotation values between 0 and 360
	wrap(m_rotation.x, 0, 360);
	wrap(m_rotation.y, 0, 360);
	wrap(m_rotation.z, 0, 360);
}

void Transform::setScale(float x, float y, float z)
{
	m_scale = D3DXVECTOR3(x, y, z);
}

// Individual Position setters
void Transform::setX(float x)
{
	m_position.x = x;
}

void Transform::setY(float y)
{
	m_position.y = y;
}

void Transform::setZ(float z)
{
	m_position.z = z;
}

// Individual Rotation setters
void Transform::setRotationX(float x)
{
	m_rotation.x = x;
}

void Transform::setRotationY(float y)
{
	m_rotation.y = y;
}

void Transform::setRotationZ(float z)
{
	m_rotation.z = z;
}

// Overall Changer
void Transform::move(D3DXVECTOR3 amount)
{
	m_position += amount;
}

void Transform::rotate(D3DXVECTOR3 amount)
{
	m_rotation += amount;

	// Keep rotation values between 0 and 360
	wrap(m_rotation.x, 0, 360);
	wrap(m_rotation.y, 0, 360);
	wrap(m_rotation.z, 0, 360);
}

void Transform::move(float x, float y, float z)
{
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
}

void Transform::rotate(float x, float y, float z)
{
	m_rotation.x += x;
	m_rotation.y += y;
	m_rotation.z += z;

	// clamp looking up and down
	clamp(m_rotation.x, -89, 90);

	// Keep rotation values between 0 and 360
	wrap(m_rotation.y, 0, 360);
	wrap(m_rotation.z, 0, 360);
}

// Individual Position Changer
void Transform::moveX(float x)
{
	m_position.x += x;
}

void Transform::moveY(float y)
{
	m_position.y += y;
}

void Transform::moveZ(float z)
{
	m_position.z += z;
}

// Individual Rotation Changer
void Transform::rotateX(float x)
{
	m_rotation.x += x;

	// clamp looking up and down
	clamp(m_rotation.x, -89, 90);
}

void Transform::rotateY(float y)
{
	m_rotation.y += y;

	// Make sure rotation is between 0 and 360
	wrap(m_rotation.y, 0, 360);
}

void Transform::rotateZ(float z)
{
	m_rotation.z += z;

	// Make sure rotation is between 0 and 360
	wrap(m_rotation.z, 0, 360);
}

// Transformation getters
D3DXVECTOR3 Transform::getPosition()
{
	return m_position;
}

D3DXVECTOR3 Transform::getRotation()
{
	return m_rotation;
}

D3DXVECTOR3 Transform::getScale()
{
	return m_scale;
}

// Vector getters
D3DXVECTOR3 Transform::getForwardVector()
{
	D3DXVECTOR3 ForwardVector;
	D3DXMATRIX RotationMatrix;

	//===========================
	// create the Forward Vector
	//===========================

	// create a rotation matrix based on pitch/yaw/roll
	getRotationMatrix(RotationMatrix);

	// Apply rotation to the vector
	D3DXVec3TransformCoord(&ForwardVector, &Vector::Forward, &RotationMatrix);

	return ForwardVector;
}

D3DXVECTOR3 Transform::getRightVector()
{
	D3DXVECTOR3 RightVector;
	D3DXMATRIX RotationMatrix;

	//=========================
	// create the Right Vector
	//=========================

	// create a rotation matrix based on pitch/yaw/roll
	getRotationMatrix(RotationMatrix);

	// Apply rotation to the vector
	D3DXVec3TransformCoord(&RightVector, &Vector::Right, &RotationMatrix);

	return RightVector;
}

D3DXVECTOR3 Transform::getUpVector()
{
	D3DXVECTOR3 UpVector;
	D3DXMATRIX RotationMatrix;

	//======================
	// create the Up Vector
	//======================

	// create a rotation matrix based on pitch/yaw/roll
	getRotationMatrix(RotationMatrix);

	// Apply rotation to the vector
	D3DXVec3TransformCoord(&UpVector, &Vector::Up, &RotationMatrix);

	return UpVector;
}

// Individual Position getters
float Transform::getX()
{
	return m_position.x;
}

float Transform::getY()
{
	return m_position.y;
}

float Transform::getZ()
{
	return m_position.z;
}

// Individual Rotation getters
float Transform::getPitch()
{
	return m_rotation.x;
}

float Transform::getYaw()
{
	return m_rotation.y;
}

float Transform::getRoll()
{
	return m_rotation.z;
}

// Matrix getters
void Transform::getTranslationMatrix(D3DXMATRIX& TranslationMatrix)
{
	D3DXMatrixTranslation(&TranslationMatrix, m_position.x, m_position.y, m_position.z);
}

void Transform::getRotationMatrix(D3DXMATRIX& RotationMatrix)
{
	// Convert degrees to radians
	float Pitch = D3DXToRadian(m_rotation.x);
	float Yaw = D3DXToRadian(m_rotation.y);
	float Roll = D3DXToRadian(m_rotation.z);

	D3DXMatrixRotationYawPitchRoll(&RotationMatrix, Yaw, Pitch, Roll);
}

void Transform::getScaleMatrix(D3DXMATRIX& ScaleMatrix)
{
	// Crate matrices using the vectors
	D3DXMatrixScaling(&ScaleMatrix, m_scale.x, m_scale.y, m_scale.z);
}

void Transform::getWorldMatrix(D3DXMATRIX& WorldMatrix)
{
	D3DXMATRIX ScaleMatrix;
	D3DXMATRIX RotationMatrix;
	D3DXMATRIX TranslationMatrix;

	// Retrieve each transformation matrix
	getScaleMatrix(ScaleMatrix);
	getRotationMatrix(RotationMatrix);
	getTranslationMatrix(TranslationMatrix);

	// Apply each matrix to the world matrix
	D3DXMatrixMultiply(&WorldMatrix, &WorldMatrix, &ScaleMatrix);
	D3DXMatrixMultiply(&WorldMatrix, &WorldMatrix, &RotationMatrix);
	D3DXMatrixMultiply(&WorldMatrix, &WorldMatrix, &TranslationMatrix);
}

// Reset
void Transform::ResetPosition()
{
	m_position = D3DXVECTOR3(0, 0, 0);
}

void Transform::ResetRotation()
{
	m_rotation = D3DXVECTOR3(0, 0, 0);
}

void Transform::ResetScale()
{
	m_scale = D3DXVECTOR3(1, 1, 1);
}