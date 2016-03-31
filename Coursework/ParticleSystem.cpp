#include "ParticleSystem.h"
#include "DirectXManager.h"

ParticleSystem::ParticleSystem() : GameObject()
{
	// Initialise the pointers to null
	Material_ = 0;
	ParticleList_ = 0;
	Vertices_ = 0;
	VertexBuffer_ = 0;
	IndexBuffer_ = 0;
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
	// Load the texture 
	Material_ = new Material;
	Result_ = Material_->SetBaseTexture(textureFilename);
	if (!Result_)
	{
		return false;
	}

	// Initialise the particle WindowManager
	Result_ = InitialiseParticles();
	if (!Result_)
	{
		return false;
	}

	// Create the buffers
	Result_ = InitialiseBuffers();
	if (!Result_)
	{
		return false;
	}

	// Create the transform
	Transform_ = new Transform;
	if (!Transform_)
	{
		return false;
	}

	return true;
}

bool ParticleSystem::InitialiseParticles()
{
	int i;

	// Set the random deviation of where the particles can be located when emitted
	Deviation_.x = 1.0f;
	Deviation_.y = 0.5f;
	Deviation_.z = 1.0f;

	// Set the velocities of particles in the x/y/z axis
	Velocity_ = D3DXVECTOR3(0.25, 0.25, 0);

	// Set the physical size of the particles
	ParticleSize_ = 0.1f;

	// Set the number of particles to emit per second
	ParticlesPerSecond_ = 250.0f;

	// Set the maximum number of particles allowed in the particle WindowManager
	MaxParticles_ = 10000;

	// Create the particle list.
	ParticleList_ = new ParticleType[MaxParticles_];
	if (!ParticleList_) { return false; }

	// Initialise the particle list.
	for (i = 0; i<MaxParticles_; i++)
	{
		ParticleList_[i].active = false;
	}

	// Initialise the current particle count to zero since none are emitted yet
	CurrentParticleCount_ = 0;

	// Clear the initial accumulated time for the particle per second emission rate
	AccumulatedTime_ = 0.0f;

	return true;
}

bool ParticleSystem::InitialiseBuffers()
{
	unsigned long* indices;
	int i;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT Result_;

	// Set the maximum number of vertices in the vertex array
	VertexCount_ = MaxParticles_ * 6;

	// Set the maximum number of indices in the index array
	IndexCount_ = VertexCount_;

	// Create the vertex array for the particles that will be rendered
	Vertices_ = new VertexType[VertexCount_];
	if (!Vertices_) { return false; }

	// Create the index array
	indices = new unsigned long[IndexCount_];
	if (!indices) { return false; }

	// Initialise vertex array to zeros at first
	memset(Vertices_, 0, (sizeof(VertexType)* VertexCount_));

	// Initialise the index array
	for (i = 0; i<IndexCount_; i++)
	{
		indices[i] = i;
	}

	// Set up the description of the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType)* VertexCount_;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data
	vertexData.pSysMem = Vertices_;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer
	Result_ = DirectXManager::Instance()->GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &VertexBuffer_);
	if (FAILED(Result_)) { return false; }

	// Set up the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long)* IndexCount_;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	Result_ = DirectXManager::Instance()->GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &IndexBuffer_);
	if (FAILED(Result_)) { return false; }

	// Release the index array since it is no longer needed
	delete[] indices;
	indices = 0;

	return true;
}

// Shutdown
void ParticleSystem::Shutdown()
{
	// Release the buffers
	if (IndexBuffer_)
	{
		IndexBuffer_->Release();
		IndexBuffer_ = 0;
	}

	if (VertexBuffer_)
	{
		VertexBuffer_->Release();
		VertexBuffer_ = 0;
	}

	// Release the particle list
	if (ParticleList_)
	{
		delete[] ParticleList_;
		ParticleList_ = 0;
	}

	// Release the texture
	if (Material_)
	{
		Material_->Shutdown();
		delete Material_;
		Material_ = 0;
	}

	return;
}

// Frame
bool ParticleSystem::Frame(float frameTime)
{
	// Release old particles
	KillParticles();

	// Emit new particles
	EmitParticles(frameTime);
	
	// Update the position of the particles
	UpdateParticles(frameTime);

	// Update the dynamic vertex buffer with the new position of each particle
	Result_ = UpdateBuffers();
	if (!Result_) { return false; }

	return true;
}

bool ParticleSystem::Render()
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::Instance()->GetDeviceContext()->IASetVertexBuffers(0, 1, &VertexBuffer_, &stride, &offset);

	// Set the index buffer to active in the InputManager assembler so it can be rendered
	DirectXManager::Instance()->GetDeviceContext()->IASetIndexBuffer(IndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer
	DirectXManager::Instance()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}

// Particles
void ParticleSystem::EmitParticles(float frameTime)
{
	bool emitParticle, found;
	D3DXVECTOR3 position;
	D3DXVECTOR3 velocity;
	int index, i, j;

	// Increment the frame time
	AccumulatedTime_ += frameTime;

	// Set emit particle to false for now
	emitParticle = false;
	
	// Check if it is time to emit a new particle or not
	if(AccumulatedTime_ > (1000.0f / ParticlesPerSecond_))
	{
		AccumulatedTime_ = 0.0f;
		emitParticle = true;
	}

	// If there are particles to emit then emit one per frame
	if((emitParticle == true) && (CurrentParticleCount_ < (MaxParticles_ - 1)))
	{
		CurrentParticleCount_++;

		// Now generate the randomized particle properties
		position.x = (((float)rand()-(float)rand())/RAND_MAX) * Deviation_.x;
		position.y = (((float)rand()-(float)rand())/RAND_MAX) * Deviation_.y;
		position.z = (((float)rand()-(float)rand())/RAND_MAX) * Deviation_.z;

		velocity = Velocity_;

		// Now since the particles need to be rendered from back to front for blending we have to sort the particle array
		// We will sort using Z depth so we need to find where in the list the particle should be inserted
		index = 0;
		found = false;
		while(!found)
		{
			if( (!ParticleList_[index].active) || (ParticleList_[index].position.z < position.z))
			{
				found = true;
			}
			else
			{
				index++;
			}
		}

		// Now that we know the location to insert into we need to copy the array over by one position from the index to make room for the new particle
		i = CurrentParticleCount_;
		j = i - 1;

		while(i != index)
		{
			ParticleList_[i] = ParticleList_[j];
			i--;
			j--;
		}

		// Now insert it into the particle array in the correct depth order
		ParticleList_[index].position = position;
		ParticleList_[index].velocity = velocity;
		ParticleList_[index].active   = true;
	}

	return;
}

void ParticleSystem::UpdateParticles(float frameTime)
{
	// Each frame we update all the particles by making them move downwards using their position, velocity, and the frame time
	for(int i=0; i<CurrentParticleCount_; i++)
	{
		ParticleList_[i].position.x -= (ParticleList_[i].velocity.x * frameTime * 0.001f);
		ParticleList_[i].position.y -= (ParticleList_[i].velocity.y * frameTime * 0.001f);
		ParticleList_[i].position.z -= (ParticleList_[i].velocity.z * frameTime * 0.001f);
	}

	return;
}

void ParticleSystem::KillParticles()
{
	int i, j;

	float waterHeight = 350.0f;

	// Kill all the particles that have gone below a certain height range
	for(i=0; i<MaxParticles_; i++)
	{
		if((ParticleList_[i].active) && (ParticleList_[i].position.y < 350.0f))
		{
			ParticleList_[i].active = false;
			CurrentParticleCount_--;

			// Now shift all the live particles back up the array to erase the destroyed particle and keep the array sorted correctly
			for(j=i; j<MaxParticles_-1; j++)
			{
				ParticleList_[j] = ParticleList_[j+1];
			}
		}
	}

	return;
}

bool ParticleSystem::UpdateBuffers()
{
	HRESULT Result_;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	// Initialise vertex array to zeros at first
	memset(Vertices_, 0, (sizeof(VertexType) * VertexCount_));

	// Now build the vertex array from the particle list array.  Each particle is a quad made out of two triangles
	int index = 0;
	D3DXVECTOR3 pos;

	for(int i=0; i<CurrentParticleCount_; i++)
	{
		pos = ParticleList_[i].position;

		// Bottom left
		Vertices_[index].position = D3DXVECTOR3(pos.x - ParticleSize_, pos.y - ParticleSize_, pos.z);
		Vertices_[index].texture = D3DXVECTOR2(0.0f, 1.0f);
		index++;

		// Top left
		Vertices_[index].position = D3DXVECTOR3(pos.x - ParticleSize_, pos.y + ParticleSize_, pos.z);
		Vertices_[index].texture = D3DXVECTOR2(0.0f, 0.0f);
		index++;

		// Bottom right
		Vertices_[index].position = D3DXVECTOR3(pos.x + ParticleSize_, pos.y - ParticleSize_, pos.z);
		Vertices_[index].texture = D3DXVECTOR2(1.0f, 1.0f);
		index++;

		// Bottom right
		Vertices_[index].position = D3DXVECTOR3(pos.x + ParticleSize_, pos.y - ParticleSize_, pos.z);
		Vertices_[index].texture = D3DXVECTOR2(1.0f, 1.0f);
		index++;

		// Top left
		Vertices_[index].position = D3DXVECTOR3(pos.x - ParticleSize_, pos.y + ParticleSize_, pos.z);
		Vertices_[index].texture = D3DXVECTOR2(0.0f, 0.0f);
		index++;

		// Top right
		Vertices_[index].position = D3DXVECTOR3(pos.x + ParticleSize_, pos.y + ParticleSize_, pos.z);
		Vertices_[index].texture = D3DXVECTOR2(1.0f, 0.0f);
		index++;
	}
	
	// Lock the vertex buffer
	Result_ = DirectXManager::Instance()->GetDeviceContext()->Map(VertexBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if(FAILED(Result_)) { return false; }

	// Get a pointer to the data in the vertex buffer
	verticesPtr = (VertexType*)mappedResource.pData;

	// Copy the data into the vertex buffer
	memcpy(verticesPtr, (void*)Vertices_, (sizeof(VertexType) * VertexCount_));

	// Unlock the vertex buffer
	DirectXManager::Instance()->GetDeviceContext()->Unmap(VertexBuffer_, 0);

	return true;
}

// Getters

int ParticleSystem::GetIndexCount()
{
	return IndexCount_;
}

// Material
Material* ParticleSystem::GetMaterial()
{
	return Material_;
}