#include "Text.h"
#include "DirectXManager.h"
#include "ShaderManager.h"
#include "WindowManager.h"
#include "AssetManager.h"

Text::Text()
{
	// initialise pointers
	m_font = 0;
}

Text::Text(const Text& other)
{
}

Text::~Text()
{
}

// Initialising
bool Text::initialise(HWND hwnd, string fontName, string fontTexture, int letterCount)
{
	// Store the resolution of the screen
	m_windowResolution = WindowManager::getInstance()->getWindowResolution();

	// onload the font to use
	AssetManager::getInstance()->loadFont(&m_font, fontName, letterCount);
	if (!m_font)
	{
		MessageBox(hwnd, L"Could not initialise the font object.", L"Error", MB_OK);
		return false;
	}

	// create the model
	m_model = new Model;

	// create Base Material
	m_material = new Material;
	m_material->setBaseTexture(fontTexture);

	// Define shader
	m_shader = ShaderManager::getInstance()->getShader("font");
	if (!m_shader)
	{
		return false;
	}

	// Define Transform
	m_transform = new Transform;

	// Define render Modes
	m_reflective = renderMode::Off;
	m_culled = renderMode::Off;
	m_depth = renderMode::Off;
	m_blendMode = BlendMode::AlphaMasked;

	return true;
}

// terminate
void Text::terminate()
{
	// Release the sentences
	m_model->terminate();
	m_sentences.clear();

	return;
}

// Text Generation
void Text::createText(string text, Vector2 position, D3DXVECTOR4 colour, Alignment align)
{
	Material* createdMaterial = 0;

	// create the sentence
	SentenceType sentence;

	// set sentence properties
	sentence.ID = m_sentences.size();
	sentence.text = text;
	sentence.position = position;
	sentence.align = align;
	sentence.value = 0;
	sentence.useValue = false;

	//==============
	// create Model
	//==============

	m_result = buildSentence(sentence);
	if (!m_result)
	{
		return;
	}

	//=================
	// create Material
	//=================

	// Copy the font material
	createdMaterial = new Material(*m_material);
	if (!createdMaterial)
	{
		return;
	}

	// Apply colour
	createdMaterial->setVector4("BaseColour", colour);
	m_model->addMaterial(createdMaterial);

	// Add to the array
	m_sentences.push_back(sentence);

	// Clean Up
	createdMaterial = 0;
}

bool Text::setText(int ID, string text)
{
	// update text
	m_sentences[ID].text = text;

	// Rebuild sentence with new information
	m_result = buildSentence(m_sentences[ID]);
	if (!m_result)
	{
		return false;
	}

	return true;
}

bool Text::setValue(int ID, float value)
{
	// update text
	m_sentences[ID].value = value;

	// Flag we have set the value
	m_sentences[ID].useValue = true;

	// Rebuild sentence with new information
	m_result = buildSentence(m_sentences[ID]);
	if (!m_result)
	{
		return false;
	}


	return true;
}

bool Text::setPosition(int ID, Vector2 position)
{
	m_sentences[ID].position = position;

	// update the sentence vertex buffer with the new string information.
	m_result = buildSentence(m_sentences[ID]);
	if (!m_result)
	{
		return false;
	}

	return true;
}

void Text::setColour(int ID, D3DXVECTOR4 colour)
{
	m_model->getMaterial(ID)->setVector4("BaseColour", colour);
}

void Text::disableValue(int ID)
{
	m_sentences[ID].useValue = false;
}

// update
bool Text::render()
{
	// Make sure we have sentences to render
	if (m_sentences.empty())
	{
		return true;
	}

	// Define how we want to see the model
	m_shader->setRenderMode(ProjectionMode::Orthographic, ViewMode::BaseView);

	// Define how we want the model to be rendered
	setRenderModes();

	// Look through sentences and render them
	for (unsigned int i = 0; i < m_sentences.size(); i++)
	{
		prepareSentence(i);

		m_result = m_shader->prepare(m_model->getMaterial(i));
		if (!m_result)
		{
			return false;
		}
		m_shader->render(m_model->getMesh(i)->getIndexCount());
	}

	// Reset Pipeline settings
	resetRenderModes();

	return true;
}

bool Text::buildSentence(SentenceType sentence)
{
	int numLetters;
	VertexData* textMesh;
	unsigned long* indices;
	float drawX, drawY;
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

	// get the number of letters in the sentence.
	numLetters = finalText.length();

	// Calculate mesh counters
	vertexCount = 6 * numLetters;
	indexCount = vertexCount;

	// create the vertex array
	textMesh = new VertexData[vertexCount];
	if (!textMesh)
	{
		return false;
	}

	// create index array
	indices = new unsigned long[indexCount];
	for (int i = 0; i < indexCount; i++)
	{
		indices[i] = i;
	}
	
	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((m_windowResolution.width / 2) * -1) + sentence.position.x);
	drawY = (float)((m_windowResolution.height / 2) - sentence.position.y);

	// Calculate how big the sentence will be to draw on screen
	renderLength = m_font->getRenderLength(finalText);

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
	m_font->buildVertexArray(textMesh, finalText, drawX, drawY);

	// create a mesh for the text
	Mesh3D* createdMesh = new Mesh3D;
	createdMesh->setIndexCount(indexCount);
	createdMesh->setVertexCount(vertexCount);
	createdMesh->setMesh(textMesh, indices);
	m_result = createdMesh->build();
	if (!m_result)
	{
		return false;
	}

	// update or add mesh
	m_model->updateMesh(sentence.ID, createdMesh);

	// Cleanup
	textMesh = 0;
	indices = 0;
	createdMesh = 0;

	return true;
}

bool Text::prepareSentence(int index)
{
	unsigned int stride, offset;
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	// set vertex buffer stride and offset
    stride = sizeof(VertexData); 
	offset = 0;

	// get buffers from mesh
	vertexBuffer = m_model->getMesh(index)->getVertexBuffer();
	indexBuffer = m_model->getMesh(index)->getIndexBuffer();

	// set the vertex buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::getInstance()->getDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

    // set the index buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::getInstance()->getDeviceContext()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // set the type of primitive that should be rendered from this vertex buffer, in this case triangles
	DirectXManager::getInstance()->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Cleanup
	vertexBuffer = 0;
	indexBuffer = 0;

	return true;
}