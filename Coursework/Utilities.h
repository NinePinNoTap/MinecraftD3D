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
float distance(D3DXVECTOR3 a, D3DXVECTOR3 b);

// Normalises a vector
D3DXVECTOR3 normalise(D3DXVECTOR3 a, D3DXVECTOR3 b);

// Rounds a number to x decimal places
void roundFloat(float& number, int decimalplaces = 0);

// Convert a string to a float
std::string toString(float number);

// Check to see if we are focused on this application
bool windowActive();

// Keeps the mouse in the middle of the screen
void lockMouseToCenter();

// Outputs a string to the output window
void outputToDebug(std::string text);

// Loop a value between two limits
void wrap(int& value, int min, int max);
void wrap(float& value, float min, float max);

// Keep between two limits
void clamp(int& value, int min, int max);
void clamp(float& value, float min, float max);

// Outputs a messagebox with reference
void outputErrorMessage(string message, char* data);

// rotates a point around a center pivot
D3DXVECTOR3 rotateAroundPoint(D3DXVECTOR3 point, D3DXVECTOR3 center, float angle);

// Calculates a angle in degrees between two points
float angleBetweenPoints(Vector2 A, Vector2 B);

// Checks if a value is within a range
bool rangeCheck(int value, int min, int max);
bool rangeCheck(float value, float min, float max);

// Checks if the position is inside the bounding box
bool checkCollision(BoundingBox box, float position);
bool checkCollision(BoundingBox box, D3DXVECTOR2 position);
bool checkCollision(BoundingBox box, D3DXVECTOR3 position);

// Converts a 3D position to 2D screen space
D3DXVECTOR2 convert3dPointToScreenSpace(D3DXVECTOR3 pos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, Rect2D WindowSize);

// Transposes each matrix in the matrix buffer
void transposeMatrixBuffer(MatrixBuffer& matrix);

// Calculates the tangent, binormal for a face
void calculateTangentBinormal(VertexData vertex1, VertexData vertex2, VertexData vertex3, D3DXVECTOR3& tangent, D3DXVECTOR3& binormal);

// Calculates a hard normal for a face
void calculateHardNormal(D3DXVECTOR3 tangent, D3DXVECTOR3 binormal, D3DXVECTOR3& normal);

// Convert a D3DXVECTOR3 into a string
string getKey(float x, float y);
string getKey(float x, float y, float z);
D3DXVECTOR3 getKey(string key);

// get index within a 3D array
int getIndex(int i, int j, int k, int maxJ, int maxK);

// Outputs a delay in MS
void outputTimeDelay(string dataName, float before, float after);

// gets rid of trailing zero
void trimString(string& str);

// Rounds vector nearest whole number
void roundVector(D3DXVECTOR2& v2);

// Rounds vector nearest whole number
void roundVector(D3DXVECTOR3& v3);

// generates 3D simplex noise
int getNoise(int x, int y, int z, float scale, int max);
int getNoise(int x, int y, int z, float scale, int max, float power);