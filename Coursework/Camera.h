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
	bool initialise();

	// update
	bool update();

	// Matrices
	bool render();
	void getViewMatrix(D3DXMATRIX& ViewMatrix);
	void render2DViewMatrix();
	void get2DViewMatrix(D3DXMATRIX& ViewMatrix);
	void renderReflection(float);
	void getReflectionMatrix(D3DXMATRIX& ViewMatrix);

private:
	// Generation
	D3DXMATRIX generateMatrix(D3DXVECTOR3 upVector, D3DXVECTOR3 positionVector, D3DXVECTOR3 lookAtVector, bool isReflection = false);

	// Vectors
	D3DXVECTOR3 m_position2D;

	// Matrices
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_baseViewMatrix;
	D3DXMATRIX m_reflectionViewMatrix;
};


