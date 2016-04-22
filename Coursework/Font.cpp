#include "Font.h"

Font::Font()
{
	// Initialise pointers to null
	Font_ = 0;
}

Font::Font(const Font& other)
{
}

Font::~Font()
{
}

// Initialising
bool Font::Initialise(const string fontFilename, int letterCount)
{
	ifstream fin;
	int i;
	char temp;

	// Create the font spacing buffer
	Font_ = new FontType[letterCount];
	if (!Font_)
	{
		return false;
	}

	// Read in the font size and spacing between chars
	fin.open(fontFilename);
	if (fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text
	for (i = 0; i<letterCount; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
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

// Shutdown
void Font::Shutdown()
{
	// Release the font data array
	if (Font_)
	{
		delete[] Font_;
		Font_ = 0;
	}

	return;
}

// Building
void Font::BuildVertexArray(VertexData* vertexData, string sentence, float drawX, float drawY)
{
	int index;
	int letter;

	// Initialise the index to the vertex array
	index = 0;

	// Draw each letter onto a quad
	for (unsigned int i = 0; i<sentence.length(); i++)
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
			vertexData[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left
			vertexData[index].texture = D3DXVECTOR2(Font_[letter].left, 0.0f);
			index++;

			vertexData[index].position = D3DXVECTOR3((drawX + Font_[letter].size), (drawY - 16), 0.0f);  // Bottom right
			vertexData[index].texture = D3DXVECTOR2(Font_[letter].right, 1.0f);
			index++;

			vertexData[index].position = D3DXVECTOR3(drawX, (drawY - 16), 0.0f);  // Bottom left
			vertexData[index].texture = D3DXVECTOR2(Font_[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexData[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left
			vertexData[index].texture = D3DXVECTOR2(Font_[letter].left, 0.0f);
			index++;

			vertexData[index].position = D3DXVECTOR3(drawX + Font_[letter].size, drawY, 0.0f);  // Top right
			vertexData[index].texture = D3DXVECTOR2(Font_[letter].right, 0.0f);
			index++;

			vertexData[index].position = D3DXVECTOR3((drawX + Font_[letter].size), (drawY - 16), 0.0f);  // Bottom right
			vertexData[index].texture = D3DXVECTOR2(Font_[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel
			drawX = drawX + Font_[letter].size + 1.0f;
		}
	}

	return;
}

float Font::GetRenderSize(string text)
{
	int letterID;
	float totalWidth;
	
	// Initialise vars
	totalWidth = 0;

	// Loop through the sentence and calculate width
	for (unsigned int i = 0; i < text.length(); i++)
	{
		// Convert to char to ascii ID
		letterID = ((int)text[i]) - 32;

		// If the letter is a space
		if (letterID == 0)
		{
			// Adjust accordingly
			totalWidth += 3.0f;
		}
		else
		{
			// Adjust by the width of the letter
			totalWidth += (Font_[letterID].size + 1);
		}
	}

	return totalWidth;
}