#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>

#include "Config.h"
#include "Utilities.h"

using namespace Config;

class Transform
{
public:
	Transform();
	~Transform();

	// Overall setter
	void setPosition(D3DXVECTOR3 Position);
	void setRotation(D3DXVECTOR3 Rotation);
	void setScale(D3DXVECTOR3 Scale);

	void setPosition(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void setScale(float x, float y, float z);

	// Individual Position setter
	void setX(float x);
	void setY(float y);
	void setZ(float z);

	// Individual Rotation setter
	void setRotationX(float x);
	void setRotationY(float y);
	void setRotationZ(float z);

	// Overall Changer
	void move(D3DXVECTOR3 amount);
	void rotate(D3DXVECTOR3 amount);

	void move(float x, float y, float z);
	void rotate(float x, float y, float z);

	// Individual Position Changer
	void moveX(float x);
	void moveY(float y);
	void moveZ(float z);

	// Individual Rotation Changer
	void rotateX(float x);
	void rotateY(float y);
	void rotateZ(float z);

	// Transformation getters
	D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getRotation();
	D3DXVECTOR3 getScale();

	// Vector getters
	D3DXVECTOR3 getForwardVector();
	D3DXVECTOR3 getRightVector();
	D3DXVECTOR3 getUpVector();

	// Individual Position getters
	float getX();
	float getY();
	float getZ();

	// Individual Rotation getters
	float getPitch();
	float getYaw();
	float getRoll();

	// Matrix getters
	void getTranslationMatrix(D3DXMATRIX& TranslationMatrix);
	void getRotationMatrix(D3DXMATRIX& RotationMatrix);
	void getScaleMatrix(D3DXMATRIX& ScaleMatrix);
	void getWorldMatrix(D3DXMATRIX& WorldMatrix);

	// Reset
	void ResetPosition();
	void ResetRotation();
	void ResetScale();

private:
	// Transforms
	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_rotation;
	D3DXVECTOR3 m_scale;
};


