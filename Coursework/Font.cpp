#include "Font.h"

Font::Font()
{
	// Initialise pointers to null
	Font_ = 0;
	Texture_ = 0;
}

Font::Font(const Font& other)
{
}

Font::~Font()
{
}

// Initialising
bool Font::Initialise(const char* fontFilename, string textureFilename)
{
	bool Result;

	// Load in the text file containing the font data
	Result = LoadFontData(fontFilename);
	if(!Result)	{ return false;	}

	// Load the texture that has the font characters on it
	Result = LoadTexture(textureFilename);
	if(!Result)	{ return false;	}

	return true;
}

// Shutdown
void Font::Shutdown()
{
	// Release the texture object
	if (Texture_)
	{
		Texture_->Shutdown();
		delete Texture_;
		Texture_ = 0;
	}

	// Release the font data array
	if (Font_)
	{
		delete[] Font_;
		Font_ = 0;
	}

	return;
}

// Loading
bool Font::LoadFontData(const char* filename)
{
	ifstream fin;
	int i;
	char temp;

	// Create the font spacing buffer
	Font_ = new FontType[95];
	if(!Font_) { return false; }

	// Read in the font size and spacing between chars
	fin.open(filename);
	if(fin.fail()) { return false; }

	// Read in the 95 used ascii characters for text
	for (i = 0; i<95; i++)
	{
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while(temp != ' ')
		{
			fin.get(temp);
		}

		fin >> Font_[i].left;
		fin >> Font_[i].right;
		fin >> Font_[i].size;
	}

	// Close the file
	fin.close();

	return true;
}

bool Font::LoadTexture(string filename)
{
	bool Result;

	// Create the texture object
	Texture_ = new Texture;
	if(!Texture_) { return false; }

	// Initialise the texture object
	Result = Texture_ -> Initialise(filename);
	if(!Result)	{ return false;	}

	return true;
}

// Building
void Font::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters;
	int index;
	int letter;

	// Coerce the InputManager vertices into a VertexType structure
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence
	numLetters = (int)strlen(sentence);

	// Initialise the index to the vertex array
	index = 0;

	// Draw each letter onto a quad
	for (int i = 0; i<numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels
		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left
			vertexPtr[index].texture = D3DXVECTOR2(Font_[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + Font_[letter].size), (drawY - 16), 0.0f);  // Bottom right
			vertexPtr[index].texture = D3DXVECTOR2(Font_[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // Bottom left
			vertexPtr[index].texture = D3DXVECTOR2(Font_[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left
			vertexPtr[index].texture = D3DXVECTOR2(Font_[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3(drawX + Font_[letter].size, drawY, 0.0f);  // Top right
			vertexPtr[index].texture = D3DXVECTOR2(Font_[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = D3DXVECTOR3((drawX + Font_[letter].size), (drawY - 16), 0.0f);  // Bottom right
			vertexPtr[index].texture = D3DXVECTOR2(Font_[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel
			drawX = drawX + Font_[letter].size + 1.0f;
		}
	}

	return;
}

// Getters
ID3D11ShaderResourceView* Font::GetTexture()
{
	return Texture_ -> GetTexture();
}

float Font::GetRenderSize(char* text)
{
	int LetterID;
	float Total;
	int Length;
	
	// Initialise vars
	Total = 0;
	Length = (int)strlen(text);

	// Loop through the sentence and calculate width
	for (int i = 0; i < Length; i++)
	{
		// Convert to char to ascii ID
		LetterID = ((int)text[i]) - 32;

		// If the letter is a space
		if (LetterID == 0)
		{
			// Adjust accordingly
			Total += 3.0f;
		}
		else
		{
			// Adjust by the width of the letter
			Total += (Font_[LetterID].size + 1);
		}
	}

	return Total;
}