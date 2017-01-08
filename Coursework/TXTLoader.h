#pragma once

#include <d3dx11.h>
#include <d3dx10math.h>

#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>

#include "Model.h"

using namespace Config;

class TXTloader
{
public:
	TXTloader();
	~TXTloader();

	bool loadModel(const char* filename, Model& model);

private:
	bool m_result;
};