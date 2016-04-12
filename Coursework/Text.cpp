#include "Text.h"
#include "DirectXManager.h"
#include "ShaderManager.h"
#include "WindowManager.h"
#include "AssetManager.h"

Text::Text()
{
	// Initialise pointers
	Font_ = 0;
}

Text::Text(const Text& other)
{
}

Text::~Text()
{
}

// Initialising
bool Text::Initialise(HWND hwnd, string fontName, string fontTexture, int letterCount)
{
	// Store the resolution of the screen
	WindowResolution_ = WindowManager::Instance()->GetWindowResolution();

	// Load the font to use
	AssetManager::Instance()->LoadFont(&Font_, fontName, letterCount);
	if (!Font_)
	{
		MessageBox(hwnd, L"Could not initialise the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the model
	Model_ = new Model;

	// Create Base Material
	FontMaterial_ = new Material;
	FontMaterial_->SetBaseTexture(fontTexture);

	// Define shader
	Shader_ = ShaderManager::Instance()->GetShader("font");
	if (!Shader_)
	{
		return false;
	}

	return true;
}

// Shutdown
void Text::Shutdown()
{
	// Release the sentences
	Model_->Shutdown();
	Sentences_.clear();

	return;
}

// Text Generation
void Text::CreateText(string text, Vector2 position, D3DXVECTOR4 colour, Alignment align)
{
	Material* createdMaterial = 0;

	// Create the sentence
	SentenceType sentence;

	// Set sentence properties
	sentence.ID = Sentences_.size();
	sentence.text = text;
	sentence.position = position;
	sentence.align = align;
	sentence.value = 0;
	sentence.useValue = false;

	//==============
	// Create Model
	//==============

	Result_ = BuildSentence(sentence);
	if (!Result_)
	{
		return;
	}

	//=================
	// Create Material
	//=================

	// Copy the font material
	createdMaterial = new Material(*FontMaterial_);
	if (!createdMaterial)
	{
		return;
	}

	// Apply colour
	createdMaterial->SetVector4("BaseColour", colour);
	Model_->AddMaterial(createdMaterial);

	// Add to the array
	Sentences_.push_back(sentence);

	// Clean Up
	createdMaterial = 0;
}

bool Text::SetText(int ID, string text)
{
	// Update text
	Sentences_[ID].text = text;

	// Rebuild sentence with new information
	Result_ = BuildSentence(Sentences_[ID]);
	if (!Result_)
	{
		return false;
	}

	return true;
}

bool Text::SetValue(int ID, float value)
{
	// Update text
	Sentences_[ID].value = value;

	// Flag we have set the value
	Sentences_[ID].useValue = true;

	// Rebuild sentence with new information
	Result_ = BuildSentence(Sentences_[ID]);
	if (!Result_)
	{
		return false;
	}


	return true;
}

bool Text::SetPosition(int ID, Vector2 position)
{
	Sentences_[ID].position = position;

	// Update the sentence vertex buffer with the new string information.
	Result_ = BuildSentence(Sentences_[ID]);
	if (!Result_)
	{
		return false;
	}

	return true;
}

void Text::SetColour(int ID, D3DXVECTOR4 colour)
{
	Model_->GetMaterial(ID)->SetVector4("BaseColour", colour);
}

void Text::DisableValue(int ID)
{
	Sentences_[ID].useValue = false;
}

// Frame
bool Text::Render()
{
	// Make sure we have sentences to render
	if (Sentences_.empty())
	{
		return true;
	}

	// Look through sentences and render them
	for (unsigned int i = 0; i < Sentences_.size(); i++)
	{
		PrepareSentence(i);

		Result_ = Shader_->Prepare(Model_->GetMaterial(i));
		if (!Result_)
		{
			return false;
		}
		Shader_->Render(Model_->GetMesh(i)->GetIndexCount());
	}

	return true;
}

bool Text::BuildSentence(SentenceType sentence)
{
	int numLetters;
	VertexData* textMesh;
	unsigned long* indices;
	float drawX, drawY;
	HRESULT result;
	int renderLength, vertexCount, indexCount;
	string finalText, convertedValue;

	// Define final output
	finalText = sentence.text;

	// Add a value if we have specified
	if (sentence.useValue)
	{
		// Convert value and remove trailing characters
		convertedValue = to_string(sentence.value);
		convertedValue.erase(convertedValue.find_last_not_of('0') + 1, std::string::npos);
		if (convertedValue[convertedValue.size() - 1] == '.')
		{
			convertedValue.erase(convertedValue.end() - 1);
		}
		finalText += convertedValue;
	}

	// Get the number of letters in the sentence.
	numLetters = finalText.length();

	// Calculate mesh counters
	vertexCount = 6 * numLetters;
	indexCount = vertexCount;

	// Create the vertex array
	textMesh = new VertexData[vertexCount];
	if (!textMesh)
	{
		return false;
	}

	// Create index array
	indices = new unsigned long[indexCount];
	for (int i = 0; i < indexCount; i++)
	{
		indices[i] = i;
	}
	
	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((WindowResolution_.width / 2) * -1) + sentence.position.x);
	drawY = (float)((WindowResolution_.height / 2) - sentence.position.y);

	// Calculate how big the sentence will be to draw on screen
	renderLength = Font_->GetRenderSize(finalText);

	// Adjust starting draw position based on alignment
	if (sentence.align == CENTRE)
	{
		drawX -= renderLength / 2;
	}
	else if (sentence.align == RIGHT)
	{
		drawX -= renderLength;
	}

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	Font_->BuildVertexArray(textMesh, finalText, drawX, drawY);

	// Create a mesh for the text
	Mesh3D* createdMesh = new Mesh3D;
	createdMesh->SetIndexCount(indexCount);
	createdMesh->SetVertexCount(vertexCount);
	createdMesh->SetMesh(textMesh, indices);
	Result_ = createdMesh->Build();
	if (!Result_)
	{
		return false;
	}

	// Update or add mesh
	Model_->UpdateMesh(sentence.ID, createdMesh);

	// Cleanup
	textMesh = 0;
	indices = 0;
	createdMesh = 0;

	return true;
}

bool Text::PrepareSentence(int index)
{
	unsigned int stride, offset;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// Set vertex buffer stride and offset
    stride = sizeof(VertexData); 
	offset = 0;

	// Get buffers from mesh
	vertexBuffer = Model_->GetMesh(index)->GetVertexBuffer();
	indexBuffer = Model_->GetMesh(index)->GetIndexBuffer();

	// Set the vertex buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::Instance()->GetDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles
	DirectXManager::Instance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Cleanup
	vertexBuffer = 0;
	indexBuffer = 0;

	return true;
}