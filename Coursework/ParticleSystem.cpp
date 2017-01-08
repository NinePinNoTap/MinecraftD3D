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
bool ParticleSystem::initialise(string textureFilename)
{
	//=======================
	// create Particle Model
	//=======================

	// create Model
	m_model = new Model;

	// Give the model a blank mesh
	m_model->addMesh(new Mesh3D);

	// create Material
	Material* createdMaterial = new Material;
	m_result = createdMaterial->setBaseTexture(textureFilename);
	if (!m_result)
	{
		return false;
	}
	m_model->addMaterial(createdMaterial);

	//======================
	// initialise Particles
	//======================

	// set the random deviation of where the particles can be located when emitted
	m_deviation.x = 1.0f;
	m_deviation.y = 0.5f;
	m_deviation.z = 1.0f;

	// Define particle properties
	m_velocity = D3DXVECTOR3(0.25, 0.25, 0);
	m_particleSize = 0.1f;
	m_particlesPerSecond = 250.0f;
	m_maxParticles = 10000;

	// Define emission rate
	m_accumulatedTime = 0.0f;

	//======================
	// initialise Transform
	//======================

	m_transform = new Transform;
	if (!m_transform)
	{
		return false;
	}

	return true;
}

// terminate
void ParticleSystem::terminate()
{
	GameObject::terminate();

	// Clear particles
	m_particles.clear();

	return;
}

// update
bool ParticleSystem::update()
{
	// Increment the frame time
	m_accumulatedTime += PerformanceManager::getInstance()->getDeltaTime() / 1000.0f;

	// Check if it is time to emit a new particle or not
	if (m_accumulatedTime > (1000.0f / m_particlesPerSecond))
	{
		// Emit new particles
		emitParticle();

		// Reset timer
		m_accumulatedTime = 0.0f;
	}
	
	// Make sure we have particles to update
	if (m_particles.empty())
	{
		return true;
	}

	// update the position of the particles
	updateParticles();

	// update the dynamic vertex buffer with the new position of each particle
	m_result = updateBuffers();
	if (!m_result)
	{
		return false;
	}

	return true;
}

// Particles
void ParticleSystem::emitParticle()
{
	ParticleType createdParticle;

	//=================
	// create Particle
	//=================

	// generate a random position
	createdParticle.position.x = (((float)rand() - (float)rand()) / RAND_MAX) * m_deviation.x;
	createdParticle.position.y = (((float)rand() - (float)rand()) / RAND_MAX) * m_deviation.y;
	createdParticle.position.z = (((float)rand() - (float)rand()) / RAND_MAX) * m_deviation.z;

	// Define movement velocity
	createdParticle.velocity = m_velocity;
	createdParticle.lifetime = 0.0f;
	createdParticle.maxLifetime = 5000.0f;

	// Add to the list
	m_particles.push_back(createdParticle);

	return;
}

void ParticleSystem::updateParticles()
{
	float frameTime;

	// get delta time
	frameTime = PerformanceManager::getInstance()->getDeltaTime() / 1000.0f;

	// update all particles
	for (std::list<ParticleType>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
	{
		// Apply velocity
		it->position += it->velocity * frameTime;
		it->lifetime += frameTime;
	}

	// Sort active particles
	m_particles.sort();

	return;
}

bool ParticleSystem::updateBuffers()
{
	VertexData* particleMesh;
	unsigned long* indices;
	int vertexCount, indexCount, index;

	// Define counters
	vertexCount = m_particles.size() * 6;
	indexCount = vertexCount;

	// create buffer space
	particleMesh = new VertexData[vertexCount];
	if (!particleMesh)
	{
		return false;
	}

	// Now build the vertex array from the particle list array
	// Each particle is a quad made out of two triangles
	index = 0;

	for (std::list<ParticleType>::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
	{
		// Bottom left
		particleMesh[index].position = D3DXVECTOR3(it->position.x - m_particleSize, it->position.y - m_particleSize, it->position.z);
		particleMesh[index].texture = D3DXVECTOR2(0.0f, 1.0f);
		index++;

		// Top left
		particleMesh[index].position = D3DXVECTOR3(it->position.x - m_particleSize, it->position.y + m_particleSize, it->position.z);
		particleMesh[index].texture = D3DXVECTOR2(0.0f, 0.0f);
		index++;

		// Bottom right
		particleMesh[index].position = D3DXVECTOR3(it->position.x + m_particleSize, it->position.y - m_particleSize, it->position.z);
		particleMesh[index].texture = D3DXVECTOR2(1.0f, 1.0f);
		index++;

		// Bottom right
		particleMesh[index].position = D3DXVECTOR3(it->position.x + m_particleSize, it->position.y - m_particleSize, it->position.z);
		particleMesh[index].texture = D3DXVECTOR2(1.0f, 1.0f);
		index++;

		// Top left
		particleMesh[index].position = D3DXVECTOR3(it->position.x - m_particleSize, it->position.y + m_particleSize, it->position.z);
		particleMesh[index].texture = D3DXVECTOR2(0.0f, 0.0f);
		index++;

		// Top right
		particleMesh[index].position = D3DXVECTOR3(it->position.x + m_particleSize, it->position.y + m_particleSize, it->position.z);
		particleMesh[index].texture = D3DXVECTOR2(1.0f, 0.0f);
		index++;
	}

	// create index buffer
	indices = new unsigned long[indexCount];
	if (!indices)
	{
		return false;
	}

	for (int i = 0; i < indexCount; i++)
	{
		indices[i] = i;
	}
	
	// update Mesh
	m_model->getMesh()->setIndexCount(indexCount);
	m_model->getMesh()->setVertexCount(vertexCount);
	m_model->getMesh()->setMesh(particleMesh, indices);
	m_result = m_model->getMesh()->build();
	if (!m_result)
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

	cameraPosition = Camera::getInstance()->getTransform()->getPosition();

	// Calculate distance from camera to this particle
	particleDistance = distance(position, cameraPosition);

	// Calculate distance from camera to other particle
	otherDistance = distance(other.position, cameraPosition);

	return particleDistance > otherDistance;
}