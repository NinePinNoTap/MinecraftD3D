#pragma once

#include <d3d11.h>
#include <d3dx10math.h>
#include <list>

#include "Camera.h"
#include "GameObject.h"
#include "PerformanceManager.h"
#include "Texture.h"

class ParticleSystem : public GameObject
{
private:
	struct ParticleType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 velocity;
		float lifetime;
		float maxLifetime;
		bool operator < (const ParticleType& other);
	};

public:
	ParticleSystem();
	ParticleSystem(const ParticleSystem&);
	~ParticleSystem();

	// Initialising
	bool Initialise(string textureFilename);

	// Shutdown
	void Shutdown();

	// Frame
	bool Frame();

private:	
	// Buffers
	bool UpdateBuffers();

	// Creating particles
	void EmitParticle();
	void UpdateParticles();
	
	// Emission Variables
	D3DXVECTOR3 Deviation_;
	D3DXVECTOR3 Velocity_;
	float AccumulatedTime_;
	float ParticleSize_;
	float ParticlesPerSecond_;
	int MaxParticles_;
	
	// List of active particles
	std::list<ParticleType> ParticleList_;
};


