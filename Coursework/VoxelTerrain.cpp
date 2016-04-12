#include "VoxelTerrain.h"

VoxelTerrain::VoxelTerrain()
{
}

VoxelTerrain::~VoxelTerrain()
{
}

#include "WindowManager.h"
void VoxelTerrain::Initialise()
{
	//======================
	// Create Block Manager
	//======================

	BlockManager_ = new BlockManager;
	BlockManager_->Initialise();

	In = new Text;
	In->Initialise(WindowManager::Instance()->GetHWND(), "shruti", "shruti.dds", 95);
	In->CreateText("Inside Chunk", Vector2(200, 200), YELLOW, Alignment::CENTRE);
	Out = new Text;
	Out->Initialise(WindowManager::Instance()->GetHWND(), "shruti", "shruti.dds", 95);
	Out->CreateText("Outside Chunk", Vector2(200, 20), YELLOW, Alignment::CENTRE);

	//=================
	// Generate Chunks
	//=================
	
	CreateChunks();
	LinkBlocks();
	GenerateTerrain();
	RefreshTerrain();
	
	OutputToDebug("Terrain Generated");
}

void VoxelTerrain::Frame()
{
	//==============
	// Update Chunk
	//==============

	typedef std::map<std::string, Chunk>::iterator it_type;
	for (it_type iterator = ChunkMap_.begin(); iterator != ChunkMap_.end(); iterator++)
	{
		// Update chunk
		iterator->second.Frame();
	}
}

void VoxelTerrain::Render()
{
	//==============
	// Render Chunk
	//==============

	typedef std::map<std::string, Chunk>::iterator it_type;
	for (it_type iterator = ChunkMap_.begin(); iterator != ChunkMap_.end(); iterator++)
	{
		// Check if the chunk is active
		if (iterator->second.IsVisible())
		{
			// Render it
			iterator->second.Render();
		}
	}

	Chunk* chunk = GetChunk(Camera::Instance()->GetTransform()->GetPosition().x,
		Camera::Instance()->GetTransform()->GetPosition().y,
		Camera::Instance()->GetTransform()->GetPosition().z);

	if (chunk)
	{
		In->Render();
		//OutputToDebug("Found an old chunk");
	}
	else
	{
		Out->Render();
		// Create a new chunk
		//OutputToDebug("New chunk found");
	}
}

void VoxelTerrain::CreateChunks()
{
	// Create Initial World
	for (int x = 0; x < 1; x++)
	{
		for (int y = 0; y < 1; y++)
		{
			for (int z = 0; z < 1; z++)
			{
				Chunk chunk;
				chunk.Initialise(x,y,z);
				chunk.SetBlocks("air");

				ChunkMap_[GetKey(x,y,z)] = chunk;
			}
		}
	}
}

void VoxelTerrain::LinkBlocks()
{
	// Loop through active chunks
	typedef std::map<std::string, Chunk>::iterator it_type;
	for (it_type iterator = ChunkMap_.begin(); iterator != ChunkMap_.end(); iterator++)
	{
		int chunkX, chunkY, chunkZ;
		int blockX, blockY, blockZ;

		// Get current chunk
		chunkX = iterator->second.GetPosition().x;
		chunkY = iterator->second.GetPosition().y;
		chunkZ = iterator->second.GetPosition().z;

		iterator->second.SetBlocks("dirt");

		// Loop through blocks in chunk
		for (int x = 0; x < CHUNK_BLOCKS; x++)
		{
			for (int y = 0; y < CHUNK_BLOCKS; y++)
			{
				for (int z = 0; z < CHUNK_BLOCKS; z++)
				{
					// Calculate block index in world space
					blockX = chunkX + x;
					blockY = chunkY + y;
					blockZ = chunkZ + z;

					SetBlockNeighbours(blockX, blockY, blockZ);
				}
			}
		}
	}




	////===============================================
	//// Link each block to its surrounding neighbours
	////===============================================

	//for (int x = 0; x < TERRAIN_WIDTH; x++)
	//{
	//	for (int y = 0; y < TERRAIN_HEIGHT; y++)
	//	{
	//		for (int z = 0; z < TERRAIN_DEPTH; z++)
	//		{
	//			SetBlockNeighbours(x, y, z);
	//		}
	//	}
	//}
}

void VoxelTerrain::SetBlockNeighbours(int x, int y, int z)
{
	//===========================
	// Apply neighbours to block
	//===========================

	Block* blockCurrent;

	// Get current block
	blockCurrent = GetBlock(x, y, z);

	if (!blockCurrent)
		return;

	// Set block neighbours
	blockCurrent->SetNeighbour(Direction::Up, GetBlock(x, y + 1, z));
	blockCurrent->SetNeighbour(Direction::Down, GetBlock(x, y - 1, z));

	blockCurrent->SetNeighbour(Direction::Left, GetBlock(x - 1, y, z));
	blockCurrent->SetNeighbour(Direction::Right, GetBlock(x + 1, y, z));

	blockCurrent->SetNeighbour(Direction::Forward, GetBlock(x, y, z + 1));
	blockCurrent->SetNeighbour(Direction::Backward, GetBlock(x, y, z - 1));
}

void VoxelTerrain::GenerateTerrain()
{
	//PerlinNoiseGenerator perlinNoise;

	//// Generate a random seed for the noise to use
	//perlinNoise.SetSeed(100);

	//for (int x = 0; x < TERRAIN_WIDTH; x++)
	//{
	//	for (int z = 0; z < TERRAIN_DEPTH; z++)
	//	{
	//		double a = (double)z / (double)TERRAIN_WIDTH * 2;
	//		double b = (double)x / (double)TERRAIN_DEPTH * 2;

	//		float noise = perlinNoise.CreateNoise(a, b, 0.8f);
	//		float height = floor(noise);
	//		//height += TERRAIN_BASE_HEIGHT;

	//		// Loop through y dimension
	//		for (int y = 0; y < height; y++)
	//		{
	//			if (GetBlock(x, y, z) == 0)
	//			{
	//				continue;
	//			}

	//			if (y < 8)
	//			{
	//				int r = rand() % 50;
	//				if (r < 3)
	//				{
	//					GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("diamond"));
	//				}
	//				else
	//				{
	//					GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("sand"));
	//				}
	//			}
	//			else if (y < 12)
	//			{
	//				GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("stone"));
	//			}
	//			else if (y < 14)
	//			{
	//				GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("sand"));
	//			}
	//			else
	//			{
	//				GetBlock(x, y, z)->CopyFrom(BlockManager::Instance()->GetBlock("dirt"));
	//			}
	//		}
	//	}
	//}
}

void VoxelTerrain::RefreshTerrain()
{
	//========================
	// Refresh Visible Blocks
	//========================

	typedef std::map<std::string, Chunk>::iterator it_type;
	for (it_type iterator = ChunkMap_.begin(); iterator != ChunkMap_.end(); iterator++)
	{
		iterator->second.RefreshVisible();
	}
}

// Getters
Chunk* VoxelTerrain::GetChunk(int x, int y, int z)
{
	float chunkX, chunkY, chunkZ;

	// Calculate chunk index
	chunkX = x / (float)CHUNK_SIZE;
	chunkY = y / (float)CHUNK_SIZE;
	chunkZ = z / (float)CHUNK_SIZE;

	chunkX = floor(chunkX);
	chunkY = floor(chunkY);
	chunkZ = floor(chunkZ);

	// Try and get chunk from map
	if (ChunkMap_.count(GetKey(chunkX, chunkY, chunkZ)))
	{
		return &ChunkMap_[GetKey(chunkX, chunkY, chunkZ)];
	}

	return 0;
}

Block* VoxelTerrain::GetBlock(int x, int y, int z)
{
	Chunk* chunk = 0;
	Block* block = 0;
	int blockX, blockY, blockZ;

	// Try and get chunk
	chunk = GetChunk(x, y, z);
	if (!chunk)
	{
		return 0;
	}

	// Calculate block index
	blockX = x - chunk->GetPosition().x;
	blockY = y - chunk->GetPosition().y;
	blockZ = z - chunk->GetPosition().z;

	// Try and get block
	block = chunk->GetBlock(blockX, blockY, blockZ);
	if (!block)
	{
		return 0;
	}

	return block;
}