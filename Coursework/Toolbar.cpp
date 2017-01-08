#include "Toolbar.h"
#include "WindowManager.h"

Toolbar::Toolbar()
{
	m_background = 0;
	m_highlighter = 0;
	m_toolbarIcons = 0;
}

Toolbar::~Toolbar()
{
}

bool Toolbar::initialise()
{
	int barPosition;

	// get height of the window
	barPosition = (-WindowManager::getInstance()->getWindowResolution().height * 0.8f) / 2;

	//====================
	// initialise Sprites
	//====================

	// Background
	m_background = new Sprite;
	if (!m_background)
	{
		return false;
	}
	m_result = m_background->initialise(Rect3D(728, 88), "ui_toolbar.dds");
	if (!m_result)
	{
		return false;
	}
	m_background->getTransform()->setY(barPosition);
	m_background->setShader("texture");

	// Toolbar Icons
	m_toolbarIcons = new InstancedSprite;
	if (!m_toolbarIcons)
	{
		return false;
	}
	m_result = m_toolbarIcons->initialise(Rect3D(64, 64), "ui_icons.dds");
	if (!m_result)
	{
		return false;
	}
	m_toolbarIcons->setShader("instancedtexture");
	m_toolbarIcons->setBlendMode(BlendMode::AlphaMasked);

	// Highlighter
	m_highlighter = new Sprite;
	if (!m_highlighter)
	{
		return false;
	}
	m_result = m_highlighter->initialise(Rect3D(92, 92), "ui_toolbar_select.dds");
	if (!m_result)
	{
		return false;
	}
	m_highlighter->setShader("texture");
	m_highlighter->setBlendMode(BlendMode::AlphaMasked);

	//==================
	// initialise Tools
	//==================

	// Add Icon Data
	m_toolData.push_back(InstanceData(D3DXVECTOR3(-320, barPosition, 1), D3DXVECTOR2(31, 17), D3DXVECTOR2(32, 21))); // Pickaxe
	m_toolData.push_back(InstanceData(D3DXVECTOR3(-240, barPosition, 1), D3DXVECTOR2(18, 11), D3DXVECTOR2(32, 21))); // Dirt
	m_toolData.push_back(InstanceData(D3DXVECTOR3(-160, barPosition, 1), D3DXVECTOR2(3, 12),  D3DXVECTOR2(32, 21))); // Sand
	m_toolData.push_back(InstanceData(D3DXVECTOR3(-80,  barPosition, 1), D3DXVECTOR2(10, 8),  D3DXVECTOR2(32, 21))); // Cobblestone
	m_toolData.push_back(InstanceData(D3DXVECTOR3(  0,  barPosition, 1), D3DXVECTOR2(7, 12),  D3DXVECTOR2(32, 21))); // Stone
	m_toolData.push_back(InstanceData(D3DXVECTOR3( 80,  barPosition, 1), D3DXVECTOR2(6, 9),   D3DXVECTOR2(32, 21))); // Wood
	m_toolData.push_back(InstanceData(D3DXVECTOR3(160,  barPosition, 1), D3DXVECTOR2(30, 1),  D3DXVECTOR2(32, 21))); // Water Bucket

	// create Instanced Sprite
	for (unsigned int i = 0; i < m_toolData.size(); i++)
	{
		m_toolbarIcons->addInstance(m_toolData[i]);
	}
	m_toolbarIcons->rebuildInstanceBuffer();

	//=================
	// initialise Vars
	//=================

	m_selectedIndex = 0;
	refresh();

	return true;
}

void Toolbar::terminate()
{
	//==========
	// Clean Up
	//==========

	if (m_background)
	{
		m_background->terminate();
		m_background = 0;
	}

	if (m_highlighter)
	{
		m_highlighter->terminate();
		m_highlighter = 0;
	}

	if (m_toolbarIcons)
	{
		m_toolbarIcons->terminate();
		m_toolbarIcons = 0;
	}
}

bool Toolbar::update()
{
	//=======================
	// handle Tool Switching
	//=======================

	handleToolSelection(VK_1, 0);
	handleToolSelection(VK_2, 1);
	handleToolSelection(VK_3, 2);
	handleToolSelection(VK_4, 3);
	handleToolSelection(VK_5, 4);
	handleToolSelection(VK_6, 5);
	handleToolSelection(VK_7, 6);

	return true;
}

bool Toolbar::render()
{
	// render the Background
	m_result = m_background->render();
	if (!m_result)
	{
		return false;
	}

	// render the Icons
	m_result = m_toolbarIcons->render();
	if (!m_result)
	{
		return false;
	}

	// render Highlighter
	m_result = m_highlighter->render();
	if (!m_result)
	{
		return false;
	}

	return true;
}

void Toolbar::handleToolSelection(unsigned int key, int index)
{
	if (!InputManager::getInstance()->getKeyDown(key))
	{
		return;
	}

	// update Index
	m_selectedIndex = index;

	// update Sprite Position
	refresh();
}

void Toolbar::refresh()
{
	if (m_toolData.empty())
	{
		return;
	}

	// update position
	m_highlighter->getTransform()->setX(m_toolData[m_selectedIndex].position.x);
	m_highlighter->getTransform()->setY(m_toolData[m_selectedIndex].position.y);
}