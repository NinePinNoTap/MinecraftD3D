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

class TXTLoader
{
public:
	TXTLoader();
	~TXTLoader();

	bool LoadModel(const char* filename, Model& model);

private:
	bool Result_;
};