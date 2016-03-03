#pragma once

#include <d3d11.h>
#include <d3dx10math.h>

#include "GameObject.h"
#include "Texture.h"


class ParticleSystem : public GameObject
{
private:
	struct ParticleType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 velocity;
		bool active;
	};

	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
	};

public:
	ParticleSystem();
	ParticleSystem(const ParticleSystem&);
	~ParticleSystem();

	// Initialising
	bool Initialise(WCHAR*);

	// Shutdown
	void Shutdown();

	// Frame
	virtual bool Frame(float);
	bool Render();

	// Getters
	int GetIndexCount();
	Material* GetMaterial();

private:	
	// Initialising
	bool InitialiseParticles();
	bool InitialiseBuffers();
	
	// Buffers
	bool UpdateBuffers();

	// Creating particles
	void EmitParticles(float);
	void UpdateParticles(float);
	void KillParticles();
	
	// Emission Variables
	D3DXVECTOR3 Deviation_;
	D3DXVECTOR3 Velocity_;
	float AccumulatedTime_;
	float ParticleSize_;
	float ParticlesPerSecond_;
	int MaxParticles_;
	int CurrentParticleCount_;

	// Vector
	ParticleType* ParticleList_;

	// Counters
	int VertexCount_;
	int IndexCount_;

	// Buffers
	ID3D11Buffer* VertexBuffer_;
	ID3D11Buffer* IndexBuffer_;
	VertexType* Vertices_;

	// Material
	Material* Material_;
};


