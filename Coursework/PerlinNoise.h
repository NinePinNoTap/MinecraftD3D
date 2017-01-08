#ifndef _PerlinNoise_H
#define _PerlinNoise_H

#include <iostream>
#include <cmath>
#include <random>
#include <algorithm>

class PerlinNoise
{
public:
	PerlinNoise();
	~PerlinNoise();

	// generate a new hash map
	void setSeed(int Seed = -1);
	
	// getters
	double createNoise(double x, double y, double z);
	double fade(double t);
	double lerp(double t, double a, double b);
	double gradient(int hash, double x, double y, double z);

private:
	std::vector<int> m_hashMap;
};

#endif