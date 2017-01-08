#include "Font.h"

Font::Font()
{
	// initialise pointers to null
	m_font = 0;
}

Font::Font(const Font& other)
{
}

Font::~Font()
{
}

// Initialising
bool Font::initialise(const string fontFilename, int letterCount)
{
	ifstream fin;
	int i;
	char temp;

	// create the font spacing buffer
	m_font = new FontType[letterCount];
	if (!m_font)
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

		fin >> m_font[i].left;
		fin >> m_font[i].right;
		fin >> m_font[i].width;
		fin >> m_font[i].height;
	}

	// Close the file
	fin.close();

	return true;
}

// terminate
void Font::terminate()
{
	// Release the font data array
	if (m_font)
	{
		delete[] m_font;
		m_font = 0;
	}

	return;
}

// Building
void Font::buildVertexArray(VertexData* vertexData, string sentence, float drawX, float drawY)
{
	int index;
	int letter;

	// initialise the index to the vertex array
	index = 0;

	// Draw each letter onto a quad
	for (unsigned int i = 0; i<sentence.length(); i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels
		if (letter == 0)
		{
			drawX = drawX + m_font[letter].width;
		}
		else
		{
			// First triangle in quad.
			vertexData[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left
			vertexData[index].texture = D3DXVECTOR2(m_font[letter].left, 0.0f);
			index++;

			vertexData[index].position = D3DXVECTOR3((drawX + m_font[letter].width), (drawY - m_font[letter].height), 0.0f);  // Bottom right
			vertexData[index].texture = D3DXVECTOR2(m_font[letter].right, 1.0f);
			index++;

			vertexData[index].position = D3DXVECTOR3(drawX, (drawY - m_font[letter].height), 0.0f);  // Bottom left
			vertexData[index].texture = D3DXVECTOR2(m_font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexData[index].position = D3DXVECTOR3(drawX, drawY, 0.0f);  // Top left
			vertexData[index].texture = D3DXVECTOR2(m_font[letter].left, 0.0f);
			index++;

			vertexData[index].position = D3DXVECTOR3(drawX + m_font[letter].width, drawY, 0.0f);  // Top right
			vertexData[index].texture = D3DXVECTOR2(m_font[letter].right, 0.0f);
			index++;

			vertexData[index].position = D3DXVECTOR3((drawX + m_font[letter].width), (drawY - m_font[letter].height), 0.0f);  // Bottom right
			vertexData[index].texture = D3DXVECTOR2(m_font[letter].right, 1.0f);
			index++;

			// update the x location for drawing by the size of the letter and one pixel
			drawX = drawX + m_font[letter].width + 1.0f;
		}
	}

	return;
}

float Font::getRenderLength(string text)
{
	int letterID;
	float totalWidth;
	
	// initialise vars
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
			totalWidth += m_font[letterID].width;
		}
		else
		{
			// Adjust by the width of the letter
			totalWidth += (m_font[letterID].width + 1);
		}
	}

	return totalWidth;
}