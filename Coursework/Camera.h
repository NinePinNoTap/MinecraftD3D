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

private:
	// Generation
	D3DXMATRIX GenerateMatrix(D3DXVECTOR3 upVector, D3DXVECTOR3 positionVector, D3DXVECTOR3 lookAtVector, bool isReflection = false);

	// Vectors
	D3DXVECTOR3 Position2D_;

	// Matrices
	D3DXMATRIX ViewMatrix_;
	D3DXMATRIX ViewMatrix2D_;
	D3DXMATRIX ReflectionViewMatrix_;
};


