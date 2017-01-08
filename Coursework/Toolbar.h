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
	bool initialise();
	void terminate();

	// update
	bool update();
	bool render();

	inline int getTool() { return m_selectedIndex; }

private:
	void handleToolSelection(unsigned int key, int index);
	void refresh();

	Sprite* m_background;
	Sprite* m_highlighter;
	InstancedSprite* m_toolbarIcons;

	int m_selectedIndex;
	vector<InstanceData> m_toolData;
	bool m_result;
};

