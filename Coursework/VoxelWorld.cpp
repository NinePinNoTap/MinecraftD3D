#include "VoxelWorld.h"

const int ChunkHeight = 8;

VoxelWorld::VoxelWorld()
{
}

VoxelWorld::~VoxelWorld()
{
}

void VoxelWorld::Initialise()
{
	//======================
	// Create Block Manager
	//======================

	BlockManager_ = new BlockManager;
	BlockManager_->Initialise();

	//===============================
	// Initialise Build and Updating
	//===============================

	BuildThread_ = thread(&VoxelWorld::BuildChunksInBuildList, this);

	Player_ = new Player;
	Player_->Initialise();
	Player_->SetHeight(2.0f);

	GenerateLocalChunks();
}

void VoxelWorld::Frame()
{
	Player_->Frame();

	//=========================
	// Handle Chunk Generation
	//=========================

	//GenerateLocalChunks();

	//======================
	// Update Active Chunks
	//======================

	HandleChunks();
}

void VoxelWorld::Render()
{
	//======================
	// Render Active Chunks
	//======================

	for (it_wc it = Map_.begin(); it != Map_.end(); it++)
	{
		it->second->Render();
	}
}

void VoxelWorld::AddChunkToBuildList(D3DXVECTOR3 chunkIndex)
{
	string chunkKey;

	// Generate a key for the chunk
	chunkKey = GetKey(chunkIndex.x, chunkIndex.y, chunkIndex.z);

	// Search for the chunk
	it_wc iterator = Map_.find(chunkKey);
	if (iterator == Map_.end())
	{
		// Couldn't find the chunk so check if its currently in the build list
		if (find(BuildList_.begin(), BuildList_.end(), chunkIndex) == BuildList_.end())
		{
			OutputToDebug("New Chunk Found : " + chunkKey);

			// Add to build list
			BuildList_.push_back(chunkIndex);
		}
	}
	else
	{
		// Chunk was found in the map
		// We should look to load it
	}
}

void VoxelWorld::BuildChunksInBuildList()
{
	while (true)
	{
		// Make sure we have chunks to build
		if (BuildList_.empty())
		{
			continue;
		}

		// Get current working chunk
		D3DXVECTOR3 chunkIndex = BuildList_.front();

		//================
		// Generate Chunk
		//================

		// Create World Chunk and Generate Terrain
		Map_[GetKey(chunkIndex.x, chunkIndex.y, chunkIndex.z)] = new ChunkColumn;
		Map_[GetKey(chunkIndex.x, chunkIndex.y, chunkIndex.z)]->Initialise(chunkIndex.x, chunkIndex.z, ChunkHeight);

		// Clean Up
		BuildList_.erase(BuildList_.begin());
	}
}

void VoxelWorld::HandleChunks()
{
	//===============
	// Update Chunks
	//===============

	for (it_wc it = Map_.begin(); it != Map_.end(); it++)
	{
		it->second->Frame();
	}
}

void VoxelWorld::GenerateLocalChunks()
{
	D3DXVECTOR3 chunkIndex, offsetPosition;
	D3DXMATRIX rotationMatrix;

	//================
	// Check Movement
	//================

	// Calculate the chunk we are in
	chunkIndex.x = floor(Camera::Instance()->GetTransform()->GetPosition().x / (float)CHUNK_SIZE);
	chunkIndex.y = 0;
	chunkIndex.z = floor(Camera::Instance()->GetTransform()->GetPosition().z / (float)CHUNK_SIZE);

	// Check if we have moved chunk since last check
	if (chunkIndex == LastChunkPosition_)
	{
		return;
	}

	// Store the new chunk we are in
	LastChunkPosition_ = chunkIndex;

	//================
	// Chunk Building
	//================

	// Build current chunk
	AddChunkToBuildList(chunkIndex);

	// Build chunks around us
	for (int i = 0; i < 9; i++)
	{
		// Rotate forward vector by angle
		D3DXMatrixRotationYawPitchRoll(&rotationMatrix, D3DXToRadian(i * 45), 0, 0);
		D3DXVec3TransformCoord(&offsetPosition, &FORWARD_VECTOR, &rotationMatrix);
		RoundVector(offsetPosition);

		// Calculate new chunk index
		AddChunkToBuildList(chunkIndex + offsetPosition);
	}
}

void VoxelWorld::SetBlock(int x, int y, int z, string blockName)
{
	Block* currentBlock;

	// Get the block
	currentBlock = GetBlock(x, y, z);

	// Make sure its a valid block
	if (!currentBlock)
		return;

	// Update the block
	currentBlock->CopyFrom(BlockManager_->GetBlock(blockName));
}

Block* VoxelWorld::GetBlock(int x, int y, int z)
{
	float chunkX, chunkY, chunkZ, blockX, blockY, blockZ;
	Chunk* currentChunk;
	Block* currentBlock;

	//=======================
	// Calculate World Chunk
	//=======================

	chunkX = x / (float)CHUNK_SIZE;
	chunkZ = z / (float)CHUNK_SIZE;

	chunkX = floor(chunkX);
	chunkZ = floor(chunkZ);

	//=======================
	// Calculate Chunk Index
	//=======================

	chunkY = y / (float)CHUNK_SIZE;
	chunkY = floor(chunkY);

	// Find the chunk
	it_wc it = Map_.find(GetKey(chunkX, 0.0f, chunkZ));
	if (it == Map_.end())
	{
		return 0;
	}

	// Get the chunk
	currentChunk = it->second->GetChunk(chunkY);
	if (!currentChunk)
	{
		return 0;
	}

	//===========
	// Get Block
	//===========

	// Calculate block index
	blockX = x - currentChunk->GetPosition().x;
	blockY = y - currentChunk->GetPosition().y;
	blockZ = z - currentChunk->GetPosition().z;

	// Try and get block
	currentBlock = currentChunk->GetBlock(blockX, blockY, blockZ);
	if (!currentBlock)
	{
		return 0;
	}

	return currentBlock;
}