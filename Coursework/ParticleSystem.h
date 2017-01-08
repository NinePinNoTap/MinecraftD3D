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
	bool initialise(string textureFilename);

	// terminate
	void terminate();

	// update
	bool update();

private:	
	// Buffers
	bool updateBuffers();

	// Creating particles
	void emitParticle();
	void updateParticles();
	
	// Emission Variables
	D3DXVECTOR3 m_deviation;
	D3DXVECTOR3 m_velocity;
	float m_accumulatedTime;
	float m_particleSize;
	float m_particlesPerSecond;
	int m_maxParticles;
	
	// List of active particles
	std::list<ParticleType> m_particles;
};


