#include "Text.h"
#include "DirectXManager.h"
#include "ShaderManager.h"

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
bool Text::Initialise(HWND hwnd, ScreenResolution WindowResolution)
{
	// Store the resolution of the screen
	WindowResolution_ = WindowResolution;

	// Create the font object
	Font_ = new Font;
	if(!Font_) { return false; }

	// Initialise the font object.
	Result_ = Font_ -> Initialise("Data/Font/shruti.txt", L"Data/Font/shruti.dds");
	if (!Result_)
	{
		MessageBox(hwnd, L"Could not initialise the font object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool Text::InitialiseSentence(SentenceType** sentence, int maxLength)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT Result;
	int i;

	// Create a new sentence object.
	*sentence = new SentenceType;
	if (!*sentence) { return false; }

	// Initialise the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set the maximum length of the sentence.
	(*sentence)->maxLength = maxLength;

	// Set the number of vertices in the vertex array.
	(*sentence)->vertexCount = 6 * maxLength;

	// Set the number of indexes in the index array.
	(*sentence)->indexCount = (*sentence)->vertexCount;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices) { return false; }

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices) { return false; }

	// Initialise vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType)* (*sentence)->vertexCount));

	// Initialise the index array.
	for (i = 0; i<(*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	Result = DirectXManager::Instance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(Result)) { return false; }

	// Set up the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	Result = DirectXManager::Instance()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(Result)) { return false; }

	// Release the vertex array as it is no longer needed
	delete[] vertices;
	vertices = 0;

	// Release the index array as it is no longer needed
	delete[] indices;
	indices = 0;

	return true;
}

// Shutdown
void Text::Shutdown()
{
	// Release the font object.
	if (Font_)
	{
		Font_->Shutdown();
		delete Font_;
		Font_ = 0;
	}

	// Release the sentences.
	for (unsigned int i = 0; i < Sentences_.size(); i++)
	{
		ReleaseSentence(&Sentences_[i]);
	}

	return;
}

// Text Generation
void Text::CreateText(char* text, Vector2 position, Colour colour, Alignment align)
{
	// Create the sentence
	SentenceType* TempSentence = new SentenceType;

	// Initialise the sentence with a base length
	InitialiseSentence(&TempSentence, 64);

	// Set the text, position and colour
	UpdateSentence(TempSentence, text, position, colour, align);

	// Add to the array
	Sentences_.push_back(TempSentence);
}

bool Text::SetText(int ID, char* text)
{
	// Update the sentence vertex buffer with the new string information.
	bool Result = UpdateSentence(Sentences_[ID], text, Sentences_[ID]->position, Sentences_[ID]->colour, Sentences_[ID]->align);
	if (!Result) { return false; }

	return true;
}

bool Text::SetText(int ID, char* text, float value)
{
	int TextLength;
	char* TextToDisplay;

	// Calculate how big the array needs to be
	TextLength = strlen(text) + ToStr(value).length();

	// Create the array to store the text
	TextToDisplay = new char[TextLength];

	// Add the data to the array
	strcpy(TextToDisplay, text);
	strcat(TextToDisplay, ToStr(value).c_str());

	// Update the text
	bool Result = SetText(ID, TextToDisplay);
	if (!Result) { return false; }

	return true;
}

bool Text::SetPosition(int ID, Vector2 NewPosition)
{
	// Update the sentence vertex buffer with the new string information.
	bool Result = UpdateSentence(Sentences_[ID], Sentences_[ID]->text, NewPosition, Sentences_[ID]->colour, Sentences_[ID]->align);
	if (!Result)
	{
		return false;
	}

	return true;
}

bool Text::SetColour(int ID, Colour NewColour)
{
	// Update the sentence vertex buffer with the new string information.
	bool Result = UpdateSentence(Sentences_[ID], Sentences_[ID]->text, Sentences_[ID]->position, NewColour, Sentences_[ID]->align);
	if (!Result)
	{
		return false;
	}

	return true;
}

// Frame
bool Text::Render()
{
	// Look through sentences and render them
	for (unsigned int i = 0; i < Sentences_.size(); i++)
	{
		Result_ = RenderSentence(Sentences_[i]);
		if (!Result_)
		{
			return false;
		}
	}

	return true;
}

bool Text::UpdateSentence(SentenceType* sentence, char* text, Vector2 position, Colour colour, Alignment align)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT Result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// Store sentence data
	sentence -> text = text;
	sentence -> position = position;
	sentence -> colour = colour;
	sentence -> align = align;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(text);

	// Check for possible buffer overflow.
	if(numLetters > sentence -> maxLength) { return false; }

	// Create the vertex array.
	vertices = new VertexType[sentence -> vertexCount];
	if(!vertices) { return false; }

	// Initialise vertex array to zeros at first.
	memset(vertices, 0, (sizeof(VertexType) * sentence -> vertexCount));

	// Calculate the X and Y pixel position on the screen to start drawing to.
	drawX = (float)(((WindowResolution_.width / 2) * -1) + position.x);
	drawY = (float)((WindowResolution_.height / 2) - position.y);

	// Calculate how big the sentence will be to draw on screen
	int TextRenderWidth = Font_ -> GetRenderSize(text);

	// Adjust starting draw position based on alignment
	if (align == CENTRE)
	{
		drawX -= TextRenderWidth / 2;
	}
	else if (align == RIGHT)
	{
		drawX -= TextRenderWidth;
	}

	// Use the font class to build the vertex array from the sentence text and sentence draw location.
	Font_ -> BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock the vertex buffer so it can be written to.
	Result = DirectXManager::Instance()->GetDeviceContext()->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(Result)) { return false; }

	// Get a pointer to the data in the vertex buffer.
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer.
	memcpy(verticesPtr, (void*)vertices, (sizeof(VertexType) * sentence -> vertexCount));

	// Unlock the vertex buffer.
	DirectXManager::Instance()->GetDeviceContext()->Unmap(sentence->vertexBuffer, 0);

	// Release the vertex array as it is no longer needed.
	delete [] vertices;
	vertices = 0;

	return true;
}

void Text::ReleaseSentence(SentenceType** sentence)
{
	if(*sentence)
	{
		// Release the sentence vertex buffer.
		if((*sentence) -> vertexBuffer)
		{
			(*sentence) -> vertexBuffer -> Release();
			(*sentence) -> vertexBuffer = 0;
		}

		// Release the sentence index buffer.
		if((*sentence) -> indexBuffer)
		{
			(*sentence) -> indexBuffer -> Release();
			(*sentence) -> indexBuffer = 0;
		}

		// Release the sentence.
		delete *sentence;
		*sentence = 0;
	}

	return;
}

bool Text::RenderSentence(SentenceType* sentence)
{
	unsigned int stride, offset;

	// Set vertex buffer stride and offset.
    stride = sizeof(VertexType); 
	offset = 0;

	// Set the vertex buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::Instance()->GetDeviceContext()->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles
	DirectXManager::Instance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Render the text using the font shader
	Result_ = ShaderManager::Instance()->FontRender(sentence, Font_->GetTexture());
	if (!Result_)
	{
		return false;
	}

	return true;
}