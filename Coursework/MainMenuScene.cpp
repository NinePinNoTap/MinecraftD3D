#include "MainMenuScene.h"
#include "ApplicationManager.h"
#include "WindowManager.h"

MainMenuScene::MainMenuScene()
{
}


MainMenuScene::~MainMenuScene()
{
}

bool MainMenuScene::Initialise(HWND hwnd)
{
	//===================
	// Create Background
	//===================

	Background_ = new Sprite;
	if (!Background_)
	{
		return false;
	}

	Result_ = Background_->Initialise(Rect3D(WindowManager::Instance()->GetWindowResolution()), "bg_mainmenu.dds");
	if (!Result_)
	{
		return false;
	}
	Background_->SetShader("texture");

	//===============
	// Create Buttons
	//===============

	CreateButton(350, 325);
	CreateButton(350, 475);

	ButtonText_ = new Text;
	if (!ButtonText_)
	{
		return false;
	}
	Result_ = ButtonText_->Initialise(WindowManager::Instance()->GetHWND(), "minecraftia.txt", "minecraftia.dds", 89);
	if (!Result_)
	{
		return false;
	}
	ButtonText_->CreateText("PLAY GAME", Vector2(350, 305), Colour::White, Alignment::CENTRE);
	ButtonText_->CreateText("EXIT GAME", Vector2(350, 455), Colour::White, Alignment::CENTRE);

	return true;
}

void MainMenuScene::Shutdown()
{

}

bool MainMenuScene::Frame()
{
	// Get Mouse Position
	POINT mousePos;
	GetCursorPos(&mousePos);

	// Convert to window space
	ScreenToClient(GetActiveWindow(), &mousePos);
	D3DXVECTOR3 screenMousePos = D3DXVECTOR3(mousePos.x, mousePos.y, 1);
	screenMousePos.x = ((WindowManager::Instance()->GetWindowResolution().width / 2) * -1) + screenMousePos.x;
	screenMousePos.y = (WindowManager::Instance()->GetWindowResolution().height / 2) - screenMousePos.y;

	if (!Buttons_.empty())
	{
		for (unsigned int i = 0; i < Buttons_.size(); i++)
		{
			//Buttons_[i]->Frame();

			BoundingBox currentBox = Buttons_[i]->GetModel()->GetMesh()->GetBoundingBox();
			currentBox.SetPosition(Buttons_[i]->GetTransform()->GetPosition());

			// Check if we are mousing over
			if (CheckCollision(currentBox, screenMousePos))
			{
				Buttons_[i]->GetModel()->GetMaterial()->SetVector4("BaseColour", D3DXVECTOR4(0,0,1,0.2));

				if (InputManager::Instance()->GetKeyDown(VK_LBUTTON))
				{
					if (i == 0)
						ApplicationManager::Instance()->SetScene(SceneState::GAME);
					else if (i == 1)
						exit(0);
				}
			}
			else
			{
				Buttons_[i]->GetModel()->GetMaterial()->SetVector4("BaseColour", D3DXVECTOR4(0,0,0,0));
			}
		}
	}

	// Render the Menu
	Render();

	return true;
}

void MainMenuScene::Render()
{
	DirectXManager::Instance()->BeginScene();

	// Render Background
	Background_->Render();

	// Render Buttons
	if (!Buttons_.empty())
	{
		for (unsigned int i = 0; i < Buttons_.size(); i++)
		{
			Buttons_[i]->Render();
		}
	}

	ButtonText_->Render();

	DirectXManager::Instance()->EndScene();
}

void MainMenuScene::Reset()
{
	ShowCursor(true);

	return;
}

void MainMenuScene::CreateButton(int x, int y)
{
	float drawX;
	float drawY;

	// Calculate where to draw the button
	drawX = (float)(((WindowManager::Instance()->GetWindowResolution().width / 2) * -1) + x);
	drawY = (float)((WindowManager::Instance()->GetWindowResolution().height / 2) - y);

	// Create the button
	Sprite* createdButton = new Sprite;
	if (!createdButton)
	{
		return;
	}

	// Initialise the button
	Result_ = createdButton->Initialise(Rect3D(410, 85, 0), "bg_button.dds");
	if (!Result_)
	{
		return;
	}

	// Set position
	createdButton->GetTransform()->SetPosition(drawX, drawY, 1);

	// Set Shader
	createdButton->SetShader("tint");

	// Enable alpha blending
	createdButton->SetRenderModes(RenderMode::Off, RenderMode::Off, RenderMode::Off, BlendMode::AlphaMasked);

	// Add to list
	Buttons_.push_back(createdButton);

	// Clean Up
	createdButton = 0;
}