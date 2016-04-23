#include "Toolbar.h"
#include "WindowManager.h"

Toolbar::Toolbar()
{
	Background_ = 0;
	ToolbarIcons_ = 0;
}

Toolbar::~Toolbar()
{
}

bool Toolbar::Initialise()
{
	int windowHeight;

	// Get height of the window
	windowHeight = WindowManager::Instance()->GetWindowResolution().height;

	//=======================
	// Initialise Background
	//=======================

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
	Background_->GetTransform()->SetY(-(windowHeight * 0.8) / 2);
	Background_->SetShader("texture");

	//==================
	// Initialise Icons
	//==================

	ToolbarIcons_ = new InstancedSprite;
	if (!ToolbarIcons_)
	{
		return false;
	}
	Result_ = ToolbarIcons_->Initialise(Rect3D(64, 64), "ui_toolbar_icons.dds");
	if (!Result_)
	{
		return false;
	}
	ToolbarIcons_->SetShader("instancedtexture");
	ToolbarIcons_->SetBlendMode(BlendMode::AlphaMasked);

	// Add Icon Data
	ToolData_.push_back(InstanceData(D3DXVECTOR3(-320,	(-windowHeight * 0.8f) / 2, 1), D3DXVECTOR2(1, 0), D3DXVECTOR2(16, 2)));
	ToolData_.push_back(InstanceData(D3DXVECTOR3(-240,	(-windowHeight * 0.8f) / 2, 1), D3DXVECTOR2(2, 0), D3DXVECTOR2(16, 2)));
	ToolData_.push_back(InstanceData(D3DXVECTOR3(-160,	(-windowHeight * 0.8f) / 2, 1), D3DXVECTOR2(3, 0), D3DXVECTOR2(16, 2)));
	ToolData_.push_back(InstanceData(D3DXVECTOR3(-80,	(-windowHeight * 0.8f) / 2, 1), D3DXVECTOR2(4, 0), D3DXVECTOR2(16, 2)));
	ToolData_.push_back(InstanceData(D3DXVECTOR3(0,		(-windowHeight * 0.8f) / 2, 1), D3DXVECTOR2(5, 0), D3DXVECTOR2(16, 2)));
	
	//--TODO
	// Maybe more efficient to just have sprite that acts as the highlighter
	// Then we dont have to rebuild every time we switch

	//=================
	// Initialise Vars
	//=================

	// Update Data
	SelectedTool_ = 0;
	ToolData_[SelectedTool_].textureOffset.y = 1;

	// Refresh UI
	RefreshTools();

	return true;
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

	return true;
}

void Toolbar::HandleToolUpdate(unsigned int key, int index)
{
	if (!InputManager::Instance()->GetKeyDown(key))
	{
		return;
	}

	// Make sure we are accessing a valid tool
	if (index >= ToolData_.size())
	{
		return;
	}

	// Revert current tool back to original
	ToolData_[SelectedTool_].textureOffset.y = 0;

	// Update tool
	SelectedTool_ = index;
	ToolData_[SelectedTool_].textureOffset.y = 1;

	// Refresh Sprite
	RefreshTools();
}

void Toolbar::RefreshTools()
{
	if (ToolData_.empty())
	{
		return;
	}

	// Add the instances to the sprite
	for (unsigned int i = 0; i < ToolData_.size(); i++)
	{
		ToolbarIcons_->AddInstance(ToolData_[i]);
	}

	// Rebuild the sprite
	ToolbarIcons_->RebuildInstanceBuffer();
}