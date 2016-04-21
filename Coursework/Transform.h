#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>

#include "Config.h"
#include "Utilities.h"

class Transform
{
public:
	Transform();
	~Transform();

	// Overall Setter
	void SetPosition(D3DXVECTOR3 Position);
	void SetRotation(D3DXVECTOR3 Rotation);
	void SetScale(D3DXVECTOR3 Scale);

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetScale(float x, float y, float z);

	// Individual Position Setter
	void SetX(float x);
	void SetY(float y);
	void SetZ(float z);

	// Individual Rotation Setter
	void SetRotationX(float x);
	void SetRotationY(float y);
	void SetRotationZ(float z);

	// Overall Changer
	void Move(D3DXVECTOR3 amount);
	void Rotate(D3DXVECTOR3 amount);

	void Move(float x, float y, float z);
	void Rotate(float x, float y, float z);

	// Individual Position Changer
	void MoveX(float x);
	void MoveY(float y);
	void MoveZ(float z);

	// Individual Rotation Changer
	void RotateX(float x);
	void RotateY(float y);
	void RotateZ(float z);

	// Transformation Getters
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	D3DXVECTOR3 GetScale();

	// Vector Getters
	D3DXVECTOR3 GetForwardVector();
	D3DXVECTOR3 GetRightVector();
	D3DXVECTOR3 GetUpVector();

	// Individual Position Getters
	float GetX();
	float GetY();
	float GetZ();

	// Individual Rotation Getters
	float GetPitch();
	float GetYaw();
	float GetRoll();

	// Matrix Getters
	void GetTranslationMatrix(D3DXMATRIX& TranslationMatrix);
	void GetRotationMatrix(D3DXMATRIX& RotationMatrix);
	void GetScaleMatrix(D3DXMATRIX& ScaleMatrix);
	void GetWorldMatrix(D3DXMATRIX& WorldMatrix);

private:
	// Transforms
	D3DXVECTOR3 Position_;
	D3DXVECTOR3 Rotation_;
	D3DXVECTOR3 Scale_;
};


