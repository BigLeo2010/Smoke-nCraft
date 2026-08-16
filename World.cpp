#define GLM_ENABLE_EXPERIMENTAL
#include "World.h"

void World::GenerateWorld() {
	world.reserve(WORLD_SIZE * WORLD_SIZE);

	for (int x = 0; x < WORLD_SIZE; x++)
	{
		for (int z = 0; z < WORLD_SIZE; z++)
		{
			uint64_t key = Tools::GetKey(x, z);

			world.emplace(std::piecewise_construct,
				std::forward_as_tuple(key),
				std::forward_as_tuple(x, 0, z));

			world.at(key).FillChunkData();
		}
	}

	for (auto& [key, chunk] : world)
	{
		treeGen.GenerateChunkTrees(chunk, world);
	}

	for (auto& [key, chunk] : world)
	{
		chunk.BuildChunkMesh();
		chunk.UploadChunkToGPU();
	}
}

void World::CreateChunk(int chunkX, int chunkZ) {
	uint64_t key = Tools::GetKey(chunkX, chunkZ);

	if (world.find(key) != world.end()) return;

	world.emplace(std::piecewise_construct,
		std::forward_as_tuple(key),
		std::forward_as_tuple(chunkX, 0, chunkZ));

	Chunk& newChunk = world.at(key);
	newChunk.FillChunkData();

	treeGen.GenerateChunkTrees(newChunk, world);

	newChunk.BuildChunkMesh();
	newChunk.UploadChunkToGPU();
}


void World::DrawWorld(float drawDistance, Shader& shader, Camera camera) {
	float maxRadius = drawDistance * 16.0f;
	float maxRadiusSquared = maxRadius * maxRadius;

	for (auto& [key, chunk] : world)
	{
		float chunkCenterX = (float)(chunk.chunkX * 16) + 8.0f;
		float chunkCenterZ = (float)(chunk.chunkZ * 16) + 8.0f;

		// Считаем квадрат расстояния до игрока
		float deltaX = chunkCenterX - camera.Position.x;
		float deltaZ = chunkCenterZ - camera.Position.z;
		float distanceSquared = (deltaX * deltaX) + (deltaZ * deltaZ);

		// Сравниваем с квадратом радиуса загрузки
		if (distanceSquared <= maxRadiusSquared) {
			chunk.DrawChunk(shader);
		}
	}
}
