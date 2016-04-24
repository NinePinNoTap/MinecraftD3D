#include "Toolbar.h"
#include "WindowManager.h"

Toolbar::Toolbar()
{
	Background_ = 0;
	ToolHighlighter_ = 0;
	ToolbarIcons_ = 0;
}

Toolbar::~Toolbar()
{
}

bool Toolbar::Initialise()
{
	int barPosition;

	// Get height of the window
	barPosition = (-WindowManager::Instance()->GetWindowResolution().height * 0.8f) / 2;

	//====================
	// Initialise Sprites
	//====================

	// Background
	Background_ = new Sprite;
	if (!Background_)
	{
		return false;
	}
	Result_ = Background_->Initialise(Rect3D(728, 88), "ui_toolbar.dds");
	if (!Result_)
	{
		return false;
	}
	Background_->GetTransform()->SetY(barPosition);
	Background_->SetShader("texture");

	// Toolbar Icons
	ToolbarIcons_ = new InstancedSprite;
	if (!ToolbarIcons_)
	{
		return false;
	}
	Result_ = ToolbarIcons_->Initialise(Rect3D(64, 64), "ui_icons.dds");
	if (!Result_)
	{
		return false;
	}
	ToolbarIcons_->SetShader("instancedtexture");
	ToolbarIcons_->SetBlendMode(BlendMode::AlphaMasked);

	// Highlighter
	ToolHighlighter_ = new Sprite;
	if (!ToolHighlighter_)
	{
		return false;
	}
	Result_ = ToolHighlighter_->Initialise(Rect3D(92, 92), "ui_toolbar_select.dds");
	if (!Result_)
	{
		return false;
	}
	ToolHighlighter_->SetShader("texture");
	ToolHighlighter_->SetBlendMode(BlendMode::AlphaMasked);

	//==================
	// Initialise Tools
	//==================

	// Add Icon Data
	ToolData_.push_back(InstanceData(D3DXVECTOR3(-320, barPosition, 1), D3DXVECTOR2(31, 17), D3DXVECTOR2(32, 21))); // Pickaxe
	ToolData_.push_back(InstanceData(D3DXVECTOR3(-240, barPosition, 1), D3DXVECTOR2(18, 11), D3DXVECTOR2(32, 21))); // Dirt
	ToolData_.push_back(InstanceData(D3DXVECTOR3(-160, barPosition, 1), D3DXVECTOR2(3, 12),  D3DXVECTOR2(32, 21))); // Sand
	ToolData_.push_back(InstanceData(D3DXVECTOR3(-80,  barPosition, 1), D3DXVECTOR2(10, 8),  D3DXVECTOR2(32, 21))); // Cobblestone
	ToolData_.push_back(InstanceData(D3DXVECTOR3(  0,  barPosition, 1), D3DXVECTOR2(7, 12),  D3DXVECTOR2(32, 21))); // Stone
	ToolData_.push_back(InstanceData(D3DXVECTOR3( 80,  barPosition, 1), D3DXVECTOR2(6, 9),   D3DXVECTOR2(32, 21))); // Wood
	ToolData_.push_back(InstanceData(D3DXVECTOR3(160,  barPosition, 1), D3DXVECTOR2(30, 1),  D3DXVECTOR2(32, 21))); // Water Bucket

	// Create Instanced Sprite
	for (unsigned int i = 0; i < ToolData_.size(); i++)
	{
		ToolbarIcons_->AddInstance(ToolData_[i]);
	}
	ToolbarIcons_->RebuildInstanceBuffer();

	//=================
	// Initialise Vars
	//=================

	SelectedTool_ = 0;
	RefreshTools();

	return true;
}

void Toolbar::Shutdown()
{
	//==========
	// Clean Up
	//==========

	if (Background_)
	{
		Background_->Shutdown();
		Background_ = 0;
	}

	if (ToolHighlighter_)
	{
		ToolHighlighter_->Shutdown();
		ToolHighlighter_ = 0;
	}

	if (ToolbarIcons_)
	{
		ToolbarIcons_->Shutdown();
		ToolbarIcons_ = 0;
	}
}

bool Toolbar::Frame()
{
	//=======================
	// Handle Tool Switching
	//=======================

	HandleToolUpdate(VK_1, 0);
	HandleToolUpdate(VK_2, 1);
	HandleToolUpdate(VK_3, 2);
	HandleToolUpdate(VK_4, 3);
	HandleToolUpdate(VK_5, 4);
	HandleToolUpdate(VK_6, 5);
	HandleToolUpdate(VK_7, 6);

	return true;
}

bool Toolbar::Render()
{
	// Render the Background
	Result_ = Background_->Render();
	if (!Result_)
	{
		return false;
	}

	// Render the Icons
	Result_ = ToolbarIcons_->Render();
	if (!Result_)
	{
		return false;
	}

	// Render Highlighter
	Result_ = ToolHighlighter_->Render();
	if (!Result_)
	{
		return false;
	}

	return true;
}

void Toolbar::HandleToolUpdate(unsigned int key, int index)
{
	if (!InputManager::Instance()->GetKeyDown(key))
	{
		return;
	}

	// Update Index
	SelectedTool_ = index;

	// Update Sprite Position
	RefreshTools();
}

void Toolbar::RefreshTools()
{
	if (ToolData_.empty())
	{
		return;
	}

	// Update position
	ToolHighlighter_->GetTransform()->SetX(ToolData_[SelectedTool_].position.x);
	ToolHighlighter_->GetTransform()->SetY(ToolData_[SelectedTool_].position.y);
}