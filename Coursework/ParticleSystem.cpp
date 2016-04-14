#include "ParticleSystem.h"
#include "DirectXManager.h"

ParticleSystem::ParticleSystem() : GameObject()
{
}

ParticleSystem::ParticleSystem(const ParticleSystem& other)
{
}

ParticleSystem::~ParticleSystem()
{
}

// Initialising
bool ParticleSystem::Initialise(string textureFilename)
{
	//=======================
	// Create Particle Model
	//=======================

	// Create Model
	Model_ = new Model;

	// Give the model a blank mesh
	Model_->AddMesh(new Mesh3D);

	// Create Material
	Material* createdMaterial = new Material;
	Result_ = createdMaterial->SetBaseTexture(textureFilename);
	if (!Result_)
	{
		return false;
	}
	Model_->AddMaterial(createdMaterial);

	//======================
	// Initialise Particles
	//======================

	// Set the random deviation of where the particles can be located when emitted
	Deviation_.x = 1.0f;
	Deviation_.y = 0.5f;
	Deviation_.z = 1.0f;

	// Define particle properties
	Velocity_ = D3DXVECTOR3(0.25, 0.25, 0);
	ParticleSize_ = 0.1f;
	ParticlesPerSecond_ = 250.0f;
	MaxParticles_ = 10000;

	// Define emission rate
	AccumulatedTime_ = 0.0f;

	//======================
	// Initialise Transform
	//======================

	Transform_ = new Transform;
	if (!Transform_)
	{
		return false;
	}

	return true;
}

// Shutdown
void ParticleSystem::Shutdown()
{
	GameObject::Shutdown();

	// Clear particles
	ParticleList_.clear();

	return;
}

// Frame
bool ParticleSystem::Frame()
{
	// Increment the frame time
	AccumulatedTime_ += PerformanceManager::Instance()->GetDeltaTime() / 1000.0f;

	// Check if it is time to emit a new particle or not
	if (AccumulatedTime_ > (1000.0f / ParticlesPerSecond_))
	{
		// Emit new particles
		EmitParticle();

		// Reset timer
		AccumulatedTime_ = 0.0f;
	}
	
	// Make sure we have particles to update
	if (ParticleList_.empty())
	{
		return true;
	}

	// Update the position of the particles
	UpdateParticles();

	// Update the dynamic vertex buffer with the new position of each particle
	Result_ = UpdateBuffers();
	if (!Result_)
	{
		return false;
	}

	return true;
}

// Particles
void ParticleSystem::EmitParticle()
{
	ParticleType createdParticle;

	//=================
	// Create Particle
	//=================

	// Generate a random position
	createdParticle.position.x = (((float)rand() - (float)rand()) / RAND_MAX) * Deviation_.x;
	createdParticle.position.y = (((float)rand() - (float)rand()) / RAND_MAX) * Deviation_.y;
	createdParticle.position.z = (((float)rand() - (float)rand()) / RAND_MAX) * Deviation_.z;

	// Define movement velocity
	createdParticle.velocity = Velocity_;
	createdParticle.lifetime = 0.0f;
	createdParticle.maxLifetime = 5000.0f;

	// Add to the list
	ParticleList_.push_back(createdParticle);

	return;
}

void ParticleSystem::UpdateParticles()
{
	float frameTime;

	// Get delta time
	frameTime = PerformanceManager::Instance()->GetDeltaTime() / 1000.0f;

	// Update all particles
	for (std::list<ParticleType>::iterator it = ParticleList_.begin(); it != ParticleList_.end(); it++)
	{
		// Apply velocity
		it->position += it->velocity * frameTime;
		it->lifetime += frameTime;
	}

	// Sort active particles
	ParticleList_.sort();

	return;
}

bool ParticleSystem::UpdateBuffers()
{
	VertexData* particleMesh;
	unsigned long* indices;
	int vertexCount, indexCount, index;

	// Define counters
	vertexCount = ParticleList_.size() * 6;
	indexCount = vertexCount;

	// Create buffer space
	particleMesh = new VertexData[vertexCount];
	if (!particleMesh)
	{
		return false;
	}

	// Now build the vertex array from the particle list array
	// Each particle is a quad made out of two triangles
	index = 0;

	for (std::list<ParticleType>::iterator it = ParticleList_.begin(); it != ParticleList_.end(); ++it)
	{
		// Bottom left
		particleMesh[index].position = D3DXVECTOR3(it->position.x - ParticleSize_, it->position.y - ParticleSize_, it->position.z);
		particleMesh[index].texture = D3DXVECTOR2(0.0f, 1.0f);
		index++;

		// Top left
		particleMesh[index].position = D3DXVECTOR3(it->position.x - ParticleSize_, it->position.y + ParticleSize_, it->position.z);
		particleMesh[index].texture = D3DXVECTOR2(0.0f, 0.0f);
		index++;

		// Bottom right
		particleMesh[index].position = D3DXVECTOR3(it->position.x + ParticleSize_, it->position.y - ParticleSize_, it->position.z);
		particleMesh[index].texture = D3DXVECTOR2(1.0f, 1.0f);
		index++;

		// Bottom right
		particleMesh[index].position = D3DXVECTOR3(it->position.x + ParticleSize_, it->position.y - ParticleSize_, it->position.z);
		particleMesh[index].texture = D3DXVECTOR2(1.0f, 1.0f);
		index++;

		// Top left
		particleMesh[index].position = D3DXVECTOR3(it->position.x - ParticleSize_, it->position.y + ParticleSize_, it->position.z);
		particleMesh[index].texture = D3DXVECTOR2(0.0f, 0.0f);
		index++;

		// Top right
		particleMesh[index].position = D3DXVECTOR3(it->position.x + ParticleSize_, it->position.y + ParticleSize_, it->position.z);
		particleMesh[index].texture = D3DXVECTOR2(1.0f, 0.0f);
		index++;
	}

	// Create index buffer
	indices = new unsigned long[indexCount];
	if (!indices)
	{
		return false;
	}

	for (int i = 0; i < indexCount; i++)
	{
		indices[i] = i;
	}
	
	// Update Mesh
	Model_->GetMesh()->SetIndexCount(indexCount);
	Model_->GetMesh()->SetVertexCount(vertexCount);
	Model_->GetMesh()->SetMesh(particleMesh, indices);
	Result_ = Model_->GetMesh()->Build();
	if (!Result_)
	{
		return false;
	}

	return true;
}

bool ParticleSystem::ParticleType::operator<(const ParticleType& other)
{
	D3DXVECTOR3 cameraPosition;
	float particleDistance;
	float otherDistance;

	cameraPosition = Camera::Instance()->GetTransform()->GetPosition();

	// Calculate distance from camera to this particle
	particleDistance = Distance(position, cameraPosition);

	// Calculate distance from camera to other particle
	otherDistance = Distance(other.position, cameraPosition);

	return particleDistance > otherDistance;
}