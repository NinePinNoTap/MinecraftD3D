#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>

#include "BoundingBox.h"
#include "Rect.h"
#include "Vector2.h"
#include "ShaderBuffers.h"
#include "VertexData.h"

using namespace std;

// Calculate the distance between two points
float Distance(D3DXVECTOR3 a, D3DXVECTOR3 b);

// Normalises a vector
D3DXVECTOR3 Normalise(D3DXVECTOR3 a, D3DXVECTOR3 b);

// Rounds a number to x decimal places
void Round(float& number, int decimalplaces = 0);

// Convert a string to a float
std::string ToStr(float number);

// Check to see if we are focused on this ApplicationManager
bool WindowActive();

// Keeps the mouse in the middle of the screen
void LockMouseToCenter();

// Outputs a string to the output window
void OutputToDebug(std::string text);

// Loop a value between two limits
void Wrap(int& value, int min, int max);
void Wrap(float& value, float min, float max);

// Keep between two limits
void Clamp(int& value, int min, int max);
void Clamp(float& value, float min, float max);

// Outputs a messagebox with reference
void OutputErrorMessage(string message, char* data);

// Rotates a point around a center pivot
D3DXVECTOR3 RotateAroundPoint(D3DXVECTOR3 point, D3DXVECTOR3 center, float angle);

// Calculates a angle in degrees between two points
float AngleBetweenPoints(Vector2 A, Vector2 B);

// Checks if a value is within a range
bool RangeCheck(int value, int min, int max);
bool RangeCheck(float value, float min, float max);

// Checks if the position is inside the bounding box
bool CheckCollision(BoundingBox box, D3DXVECTOR3 position);

// Converts a 3D position to 2D screen space
D3DXVECTOR2 ConvertToScreenSpace(D3DXVECTOR3 pos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, Rect2D WindowSize);

// Transposes each matrix in the matrix buffer
void TransposeMatrix(MatrixBuffer& matrix);

// Calculates the tangent, binormal for a face
void CalculateTangentBinormal(VertexData vertex1, VertexData vertex2, VertexData vertex3, D3DXVECTOR3& tangent, D3DXVECTOR3& binormal);

// Calculates a hard normal for a face
void CalculateHardNormal(D3DXVECTOR3 tangent, D3DXVECTOR3 binormal, D3DXVECTOR3& normal);

// Convert a D3DXVECTOR3 into a string
string GetKey(float x, float y, float z);

// Get index within a 3D array
int GetIndex(int i, int j, int k, int maxJ, int maxK);

// Outputs a delay in MS
void OutputTimeDelay(string dataName, float before, float after);

// Gets rid of trailing zero
void TrimString(string& str);

// Rounds vector nearest whole number
void RoundVector3(D3DXVECTOR3& vector);
