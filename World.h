#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <cstdint>
#include <cmath>
#include "Chunk.h"
#include "TreeGeneration.h"
#include "Camera.h"
#include "Tools.h"

class World {
private:
    TreeGeneration treeGen;

public:
    std::unordered_map<uint64_t, Chunk> world;
    int WORLD_SIZE = 20;

    void GenerateWorld();
    void CreateChunk(int chunkX, int chunkZ);
    void DrawWorld(float drawDistance, Shader& shader, Camera camera);

    Chunk* GetChunkPtr(int chunkX, int chunkZ) {
        auto it = world.find(Tools::GetKey(chunkX, chunkZ));
        return (it != world.end()) ? &(it->second) : nullptr;
    }
};

#endif
