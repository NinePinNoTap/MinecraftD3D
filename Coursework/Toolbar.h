#pragma once

#include "InputManager.h"
#include "Sprite.h"
#include "InstancedSprite.h"
#include "VirtualKeys.h"

class Toolbar
{
public:
	Toolbar();
	~Toolbar();

	// Initialising
	bool Initialise();
	void Shutdown();

	// Frame
	bool Frame();
	bool Render();

	inline int GetTool() { return SelectedTool_; }

private:
	void HandleToolUpdate(unsigned int key, int index);
	void RefreshTools();

	Sprite* Background_;
	Sprite* ToolHighlighter_;
	InstancedSprite* ToolbarIcons_;

	int SelectedTool_;
	vector<InstanceData> ToolData_;
	bool Result_;
};

