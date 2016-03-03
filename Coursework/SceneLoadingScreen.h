#pragma once

#include "Sprite.h"
#include "DirectXManager.h"
#include "Scene3D.h"
#include "ShaderManager.h"

class SceneLoadingScreen : public Scene3D
{
public:
	SceneLoadingScreen();
	~SceneLoadingScreen();

	// Initialising
	bool Initialise(Rect2D WindowResolution);

	void Reset();

	// Shutdown
	void Shutdown();

	// Frame
	bool Frame();

private:
	void Render();

	void GenerateMatrices();
	void SetShaderManagerVars();

	Sprite* Background_;
	Sprite* Bar_;
	Sprite* BarMask_;
	int LoadingProgress_;
	Text* LoadingText_;
};


