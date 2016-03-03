#include "Utilities.h"

float Distance(D3DXVECTOR3 a, D3DXVECTOR3 b)
{
	float x, y, z;

	x = b.x - a.x;
	y = b.y - a.y;
	z = b.z - a.z;

	return sqrt(x*x + y*y + z*z);
}

D3DXVECTOR3 Normalise(D3DXVECTOR3 a, D3DXVECTOR3 b)
{
	float length = Distance(a, b);

	D3DXVECTOR3 Difference = b - a;

	float x = Difference.x / length;
	float y = Difference.y / length;
	float z = Difference.z / length;

	return D3DXVECTOR3(x, y, z);
}

void Round(float& number, int decimalplaces)
{
	int ten = 10 ^ decimalplaces;

	number = roundf(number * ten) / ten;
}

std::string ToStr(float number)
{
	std::stringstream sstr;
	sstr << number;
	return sstr.str();
}

bool WindowActive()
{
	return GetActiveWindow() == GetForegroundWindow();
}

void LockMouseToCenter()
{
	int middleX = GetSystemMetrics(SM_CXSCREEN) / 2;
	int middleY = GetSystemMetrics(SM_CYSCREEN) / 2;

	SetCursorPos(middleX, middleY);
}

void OutputToDebug(std::string text)
{
	int len;
	int slength = (int)text.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, text.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, text.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;

	OutputDebugString(r.c_str());
}

void Wrap(int& value, int min, int max)
{
	int difference;

	if (value < min)
	{
		difference = min - abs(value);
		value = max - difference;
	}
	else if (value > max)
	{
		difference = value - max;
		value = min + difference;
	}
}

void Wrap(float& value, float min, float max)
{
	float difference;

	if (value < min)
	{
		difference = min - abs(value);
		value = max - difference;
	}
	else if (value > max)
	{
		difference = value - max;
		value = min + difference;
	}
}

void Clamp(float& value, float min, float max)
{
	if (value < min)
		value = min;
	else if (value > max)
		value = max;
}

void OutputErrorMessage(string message, char* data)
{
	message += data;

	int len;
	int slength = (int)message.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, message.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, message.c_str(), slength, buf, len);
	std::wstring result(buf);
	delete[] buf;

	MessageBox(NULL, result.c_str(), L"ERROR", MB_OK);
}

D3DXVECTOR3 RotateAroundPoint(D3DXVECTOR3 point, D3DXVECTOR3 center, float angle)
{
	float rotatedX = cos(angle) * (point.x - center.x) - sin(angle) * (point.y - center.y) + center.x;
	float rotatedY = sin(angle) * (point.x - center.x) + cos(angle) * (point.y - center.y) + center.y;
	
	return D3DXVECTOR3(rotatedX, rotatedY, point.z);
}

float AngleBetweenPoints(Vector2 A, Vector2 B)
{
	float deltaX = B.x - A.x;
	float deltaY = B.y - A.y;

	return D3DXToDegree(atan2(deltaY, deltaX));
}

bool RangeCheck(float value, float min, float max)
{
	return value >= min && value <= max;
}

bool CheckCollision(BoundingBox box, D3DXVECTOR3 position)
{
	bool insideX = RangeCheck(position.x, box.left, box.right);
	bool insideY = RangeCheck(position.y, box.bottom, box.top);
	bool insideZ = RangeCheck(position.z, box.front, box.back);

	return insideX && insideY && insideZ;
}

D3DXVECTOR2 ConvertToScreenSpace(D3DXVECTOR3 pos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ScreenResolution WindowSize)
{
	// Transform the coordinates using the view and projection matrix
	D3DXVec3TransformCoord(&pos, &pos, &viewMatrix);
	D3DXVec3TransformCoord(&pos, &pos, &projectionMatrix);

	// Convert the coordinates into screen space
	pos.x = WindowSize.width * (pos.x + 1.0) / 2.0;
	pos.y = WindowSize.height * (1.0 - ((pos.y + 1.0) / 2.0));

	return D3DXVECTOR2(pos.x, pos.y);
}

bool CheckScreenSpace(D3DXVECTOR3 pos, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ScreenResolution windowSize)
{
	// Convert the 3D position to screen space
	D3DXVECTOR2 ConvertedPos = ConvertToScreenSpace(pos, viewMatrix, projectionMatrix, windowSize);
	float halfWidth = windowSize.width;
	float halfHeight = windowSize.height;

	// Check if its within the limits of screen
	bool xCheck = RangeCheck(ConvertedPos.x, -halfWidth, halfWidth);
	bool yCheck = RangeCheck(ConvertedPos.y, -halfHeight, halfHeight);

	// Create the frustum matrix from the view matrix and updated projection matrix.
	D3DXMATRIX frustrumMatrix;

	D3DXMatrixMultiply(&frustrumMatrix, &viewMatrix, &projectionMatrix);

	// Create a plane representing the screen
	D3DXPLANE viewPlane;
	viewPlane.a = frustrumMatrix._14 + frustrumMatrix._13;
	viewPlane.b = frustrumMatrix._24 + frustrumMatrix._23;
	viewPlane.c = frustrumMatrix._34 + frustrumMatrix._33;
	viewPlane.d = frustrumMatrix._44 + frustrumMatrix._43;
	D3DXPlaneNormalize(&viewPlane, &viewPlane);

	// Return whether its within the bounds of the screen and is on the screen
	return xCheck && yCheck && D3DXPlaneDotCoord(&viewPlane, &pos) >= 0.0f;
}

bool CheckScreenSpace(D3DXVECTOR3 position, BoundingBox box, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix, ScreenResolution windowSize)
{
	// Create the positions in world space
	D3DXVECTOR3 leftPos = position + D3DXVECTOR3(box.left, 0, 0);
	D3DXVECTOR3 rightPos = position + D3DXVECTOR3(box.right, 0, 0);
	D3DXVECTOR3 upPos = position + D3DXVECTOR3(0, box.top, 0);
	D3DXVECTOR3 downPos = position + D3DXVECTOR3(0, box.bottom, 0);
	D3DXVECTOR3 frontPos = position + D3DXVECTOR3(0, 0, box.front);
	D3DXVECTOR3 backPos = position + D3DXVECTOR3(0, 0, box.back);

	// Check each position against the camera
	bool Left = CheckScreenSpace(leftPos, viewMatrix, projectionMatrix, windowSize);
	bool Right = CheckScreenSpace(rightPos, viewMatrix, projectionMatrix, windowSize);
	bool Up = CheckScreenSpace(upPos, viewMatrix, projectionMatrix, windowSize);
	bool Down = CheckScreenSpace(downPos, viewMatrix, projectionMatrix, windowSize);
	bool Front = CheckScreenSpace(frontPos, viewMatrix, projectionMatrix, windowSize);
	bool Back = CheckScreenSpace(backPos, viewMatrix, projectionMatrix, windowSize);
	bool Center = CheckScreenSpace(position, viewMatrix, projectionMatrix, windowSize);

	return Left || Right || Up || Down || Front || Back || Center;
}

void TransposeMatrix(MatrixCBuffer& matrix)
{
	// Prepare matrices for the shader
	D3DXMatrixTranspose(&matrix.world, &matrix.world);
	D3DXMatrixTranspose(&matrix.view, &matrix.view);
	D3DXMatrixTranspose(&matrix.projection, &matrix.projection);
	D3DXMatrixTranspose(&matrix.reflection, &matrix.reflection);
}